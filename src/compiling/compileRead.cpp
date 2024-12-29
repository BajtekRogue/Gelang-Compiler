#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utility.hpp"


std::vector<AssemblyInstruction> compileRead(SymbolsTable& symbolsTable, const std::unique_ptr<ReadCommand>& cmd) {
    std::vector<AssemblyInstruction> result;

    Identifier* identifier = cmd->identifier.get();
    std::string id = identifier->id;

    // Check if the variable is used correctly
    validateUseOfVariable(symbolsTable, *(cmd->identifier), cmd->lineNumber, false);
    if(symbolsTable.isIterator(id)){
        ErrorHandler::readingIntoIterator(id, cmd->lineNumber);
    }

    result.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "READ " + identifier->toString()));

    // READ a variable
    if(identifier->isVariable()){

        // If variable is local
        if(!symbolsTable.isParameter(id)){
            int64_t address = symbolsTable.getMemoryAddressVariable(id);

            result.push_back(AssemblyInstruction(Instruction::GET, address));
            symbolsTable.markAsInitialized(id);

            return result;
        }

        // If variable is a parameter
        int64_t address = symbolsTable.getMemoryAddressPointerParameter(id);

        result.push_back(AssemblyInstruction(Instruction::GET, 0));
        result.push_back(AssemblyInstruction(Instruction::STOREI, address));

        return result;
    }

    // READ an array
    ArrayAccess& arrayAccess = identifier->arrayAccess.value();

    // If array is accessed by index
    if(arrayAccess.isByIndex()){

        int64_t index = arrayAccess.getIndex();

        // If the array is local
        if(!symbolsTable.isParameter(id)){
            int64_t address = symbolsTable.getMemoryAddressAt(id, index);
            result.push_back(AssemblyInstruction(Instruction::GET, address));

            return result;
        }

        // If the array is a parameter
        int64_t address = symbolsTable.getMemoryAddressPointerParameter(id);

        // If index is 0
        if(index == 0){
            result.push_back(AssemblyInstruction(Instruction::GET, 0));
            result.push_back(AssemblyInstruction(Instruction::STOREI, address));
            return result;
        }

        result.push_back(AssemblyInstruction(Instruction::SET, index));
        result.push_back(AssemblyInstruction(Instruction::ADD, address));
        result.push_back(AssemblyInstruction(Instruction::STORE, Memory::arrayVariableAssign));
        result.push_back(AssemblyInstruction(Instruction::GET, 0));
        result.push_back(AssemblyInstruction(Instruction::STOREI, Memory::arrayVariableAssign));

        return result;
    }

    // If array is accessed by variable
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // If both array and the variable are local
    if(!symbolsTable.isParameter(id) && !symbolsTable.isParameter(indexIdentifier)){
        
        int64_t indexAddress = symbolsTable.getMemoryAddressVariable(indexIdentifier);
        int64_t arrayStartAddress = symbolsTable.getMemoryAddressStart(id) + symbolsTable.getOffset(id);

        result.push_back(AssemblyInstruction(Instruction::SET, arrayStartAddress));
        result.push_back(AssemblyInstruction(Instruction::ADD, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE, Memory::arrayVariableAssign));
        result.push_back(AssemblyInstruction(Instruction::GET, 0));
        result.push_back(AssemblyInstruction(Instruction::STOREI, Memory::arrayVariableAssign));

        return result;
    }

    // If array is local but the index is a parameter
    if(!symbolsTable.isParameter(id) && symbolsTable.isParameter(indexIdentifier)){

        int64_t indexAddress = symbolsTable.getMemoryAddressPointerParameter(indexIdentifier);
        int64_t arrayStartAddress = symbolsTable.getMemoryAddressStart(id) + symbolsTable.getOffset(id);

        result.push_back(AssemblyInstruction(Instruction::SET, arrayStartAddress));
        result.push_back(AssemblyInstruction(Instruction::ADDI, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE, Memory::arrayVariableAssign));
        result.push_back(AssemblyInstruction(Instruction::GET, 0));
        result.push_back(AssemblyInstruction(Instruction::STOREI, Memory::arrayVariableAssign));

        return result;
    }
    
    // If array is a parameter but the index is local
    if(symbolsTable.isParameter(id) && !symbolsTable.isParameter(indexIdentifier)){

        int64_t indexAddress = symbolsTable.getMemoryAddressVariable(indexIdentifier);
        int64_t arrayAddress = symbolsTable.getMemoryAddressPointerParameter(id);

        result.push_back(AssemblyInstruction(Instruction::LOAD, arrayAddress));
        result.push_back(AssemblyInstruction(Instruction::ADD, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE, Memory::arrayVariableAssign));
        result.push_back(AssemblyInstruction(Instruction::GET, 0));
        result.push_back(AssemblyInstruction(Instruction::STOREI, Memory::arrayVariableAssign));

        return result;
    }

    // If both array and the index are parameters
    if(symbolsTable.isParameter(id) && symbolsTable.isParameter(indexIdentifier)){

        int64_t indexAddress = symbolsTable.getMemoryAddressPointerParameter(indexIdentifier);
        int64_t arrayAddress = symbolsTable.getMemoryAddressPointerParameter(id);

        result.push_back(AssemblyInstruction(Instruction::LOAD, arrayAddress));
        result.push_back(AssemblyInstruction(Instruction::ADDI, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE, Memory::arrayVariableAssign));
        result.push_back(AssemblyInstruction(Instruction::GET, 0));
        result.push_back(AssemblyInstruction(Instruction::STOREI, Memory::arrayVariableAssign));

        return result;
    }
    return result;
}
