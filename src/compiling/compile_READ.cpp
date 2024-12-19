#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"


std::vector<AssemblyInstruction> compile_READ(SymbolsTable& symbolsTable, const std::unique_ptr<ReadCommand>& cmd) {
    std::vector<AssemblyInstruction> result;
    Identifier* identifier = cmd->identifier.get();
    std::string id = identifier->id;

    validateUseOfVariable(symbolsTable, *(cmd->identifier), "READ", false);

    result.push_back(AssemblyInstruction(AssemblyInstructionType::NULL_INSTRUCTION, "READ " + id));

    // READ a variable
    if(identifier->isVariable()){

        // Get its memory address and store the input there
        ll address = symbolsTable.getMemoryAddress_variable(id);
        result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, address));
        symbolsTable.markAsInitialized(id);
        return result;
    }

    // READ an array
    ArrayAccess& arrayAccess = identifier->arrayAccess.value();

    // If array is accessed by index
    if(arrayAccess.isByIndex()){

        ll index = arrayAccess.getIndex();

        // Get memory address of array at the index and store the input there
        ll address = symbolsTable.getMemoryAddress_at(id, index);
        result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, address));

        return result;
    }

    // If array is accessed by variable
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // Get memory address of index and store the input there. Account for the offset of the array
    ll indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
    ll arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

    result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddress));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddress));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, 1));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, 0));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::STOREI, 1));
    
    return result;
}