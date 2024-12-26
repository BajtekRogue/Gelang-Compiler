#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utility.hpp"


std::vector<AssemblyInstruction> compileAssign(SymbolsTable& symbolsTable, const std::unique_ptr<AssignCommand>& cmd){
    std::vector<AssemblyInstruction> result;

    Identifier& identifier = *(cmd->identifier);
    std::string id = identifier.id;

    // Check if the variable is used correctly
    validateUseOfVariable(symbolsTable, identifier, "ASSIGN", false);
    if(symbolsTable.isIterator(id)){
        throw std::logic_error("Cannot assign to iterator " + id);
    }
    result.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, identifier.toString() + " := " + cmd->expression->toString()));

    // If the identifier is a variable
    if(identifier.isVariable()){

        std::vector<AssemblyInstruction> expressionInstructions = compileExpression(symbolsTable, cmd->expression);
        result.insert(result.end(), expressionInstructions.begin(), expressionInstructions.end());

        // If it is a parameter, store the result in its memory address
        if(symbolsTable.isParameter(id)){
            int64_t parameterAddress = symbolsTable.getMemoryAddressPointer_parameter(id);
            result.push_back(AssemblyInstruction(Instruction::STOREI, parameterAddress));

            return result;
        }

        // Get memory address of the variable and store the result of the expression there
        int64_t address = symbolsTable.getMemoryAddress_variable(id);
        result.push_back(AssemblyInstruction(Instruction::STORE, address));
        symbolsTable.markAsInitialized(id);

        return result;
    }

    // If the identifier is an array
    ArrayAccess arrayAccess = identifier.getArrayAccess();

    // If array is accessed by index but is not a parameter
    if(arrayAccess.isByIndex() && !symbolsTable.isParameter(id)){

        int64_t index = arrayAccess.getIndex();
        int64_t address = symbolsTable.getMemoryAddress_at(id, index);

        std::vector<AssemblyInstruction> expressionInstructions = compileExpression(symbolsTable, cmd->expression);

        result.insert(result.end(), expressionInstructions.begin(), expressionInstructions.end());
        result.push_back(AssemblyInstruction(Instruction::STORE, address));

        return result;
    }

    // If array is accessed by index but is a parameter
    if(arrayAccess.isByIndex() && symbolsTable.isParameter(id)){

        int64_t index = arrayAccess.getIndex();
        int64_t arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        // If index is 0
        if(index == 0){
            std::vector<AssemblyInstruction> expressionInstructions = compileExpression(symbolsTable, cmd->expression);

            result.insert(result.end(), expressionInstructions.begin(), expressionInstructions.end());
            result.push_back(AssemblyInstruction(Instruction::STOREI, arrayAddress));

            return result;
        }

        // Load into the accumulator the value at the index of the array
        result.push_back(AssemblyInstruction(Instruction::SET, index));
        result.push_back(AssemblyInstruction(Instruction::ADD, arrayAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE,  MEMORY_ARRAY_VARIABLE_ASSIGN));

        std::vector<AssemblyInstruction> expressionInstructions = compileExpression(symbolsTable, cmd->expression);

        result.insert(result.end(), expressionInstructions.begin(), expressionInstructions.end());
        result.push_back(AssemblyInstruction(Instruction::STOREI,  MEMORY_ARRAY_VARIABLE_ASSIGN));

        return result;
    }


    // If array is accessed by variable
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // If both array and the variable are local
    if(!symbolsTable.isParameter(id) && !symbolsTable.isParameter(indexIdentifier)){

        int64_t indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
        int64_t arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

        result.push_back(AssemblyInstruction(Instruction::SET, arrayStartAddress));
        result.push_back(AssemblyInstruction(Instruction::ADD, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE,  MEMORY_ARRAY_VARIABLE_ASSIGN));

        std::vector<AssemblyInstruction> expressionInstructions = compileExpression(symbolsTable, cmd->expression);

        result.insert(result.end(), expressionInstructions.begin(), expressionInstructions.end());
        result.push_back(AssemblyInstruction(Instruction::STOREI,  MEMORY_ARRAY_VARIABLE_ASSIGN));

        return result;
    }

    // If array is local but the index is a parameter
    if(!symbolsTable.isParameter(id) && symbolsTable.isParameter(indexIdentifier)){

        int64_t indexAddress = symbolsTable.getMemoryAddressPointer_parameter(indexIdentifier);
        int64_t arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

        result.push_back(AssemblyInstruction(Instruction::SET, arrayStartAddress));
        result.push_back(AssemblyInstruction(Instruction::ADDI, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE,  MEMORY_ARRAY_VARIABLE_ASSIGN));

        std::vector<AssemblyInstruction> expressionInstructions = compileExpression(symbolsTable, cmd->expression);

        result.insert(result.end(), expressionInstructions.begin(), expressionInstructions.end());
        result.push_back(AssemblyInstruction(Instruction::STOREI,  MEMORY_ARRAY_VARIABLE_ASSIGN));

        return result;
    }

    // If array is a parameter but the index is local
    if(symbolsTable.isParameter(id) && !symbolsTable.isParameter(indexIdentifier)){

        int64_t indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
        int64_t arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        result.push_back(AssemblyInstruction(Instruction::LOAD, arrayAddress));
        result.push_back(AssemblyInstruction(Instruction::ADD, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE,  MEMORY_ARRAY_VARIABLE_ASSIGN));

        std::vector<AssemblyInstruction> expressionInstructions = compileExpression(symbolsTable, cmd->expression);

        result.insert(result.end(), expressionInstructions.begin(), expressionInstructions.end());
        result.push_back(AssemblyInstruction(Instruction::STOREI,  MEMORY_ARRAY_VARIABLE_ASSIGN));

        return result;
    }

    // If both array and the index are parameters
    if(symbolsTable.isParameter(id) && symbolsTable.isParameter(indexIdentifier)){
        
        int64_t indexAddress = symbolsTable.getMemoryAddressPointer_parameter(indexIdentifier);
        int64_t arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        result.push_back(AssemblyInstruction(Instruction::LOAD, arrayAddress));
        result.push_back(AssemblyInstruction(Instruction::ADDI, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE,  MEMORY_ARRAY_VARIABLE_ASSIGN));

        std::vector<AssemblyInstruction> expressionInstructions = compileExpression(symbolsTable, cmd->expression);

        result.insert(result.end(), expressionInstructions.begin(), expressionInstructions.end());
        result.push_back(AssemblyInstruction(Instruction::STOREI,  MEMORY_ARRAY_VARIABLE_ASSIGN));

        return result;
    }
    return result;
}