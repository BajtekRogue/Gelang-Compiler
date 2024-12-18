#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"

std::vector<AssemblyInstruction> compile_ASSIGN(SymbolsTable& symbolsTable, const std::unique_ptr<AssignCommand>& cmd){
    std::vector<AssemblyInstruction> result;

    // Check if the variable is used correctly
    validateUseOfVariable(symbolsTable, *(cmd->identifier), "ASSIGN", false);

    Identifier& identifier = *(cmd->identifier);
    std::string id = identifier.id;

    // If the identifier is a variable
    if(identifier.isVariable()){

        // Get memory address of the variable and store the result of the expression there
        ll address = symbolsTable.getMemoryAddress_variable(id);

        std::vector<AssemblyInstruction> expressionInstructions = compile_EXPRESSION(symbolsTable, cmd->expression);
        result.insert(result.end(), expressionInstructions.begin(), expressionInstructions.end());

        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, address));

        // Mark the variable as initialized
        symbolsTable.markAsInitialized(id);

        return result;
    }

    // If the identifier is an array
    ArrayAccess arrayAccess = identifier.getArrayAccess();

    // If array is accessed by index
    if(arrayAccess.isByIndex()){

        ll index = arrayAccess.getIndex();

        // Get memory address of array at the index and store the result of the expression there
        ll address = symbolsTable.getMemoryAddress_at(id, index);

        std::vector<AssemblyInstruction> expressionInstructions = compile_EXPRESSION(symbolsTable, cmd->expression);
        result.insert(result.end(), expressionInstructions.begin(), expressionInstructions.end());

        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, address));

        return result;
    }

    // If array is accessed by variable
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // Get memory address of index and store the result of the expression there. Account for the offset of the array
    ll indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
    ll arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

    result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddress));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddress));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE,  MEMORY_ARRAY_VARIABLE_ASSIGN));

    std::vector<AssemblyInstruction> expressionInstructions = compile_EXPRESSION(symbolsTable, cmd->expression);
    result.insert(result.end(), expressionInstructions.begin(), expressionInstructions.end());
    
    result.push_back(AssemblyInstruction(AssemblyInstructionType::STOREI,  MEMORY_ARRAY_VARIABLE_ASSIGN));

    return result;
}