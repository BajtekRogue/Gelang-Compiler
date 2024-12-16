#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"

std::vector<AssemblyInstruction> compile_ASSIGN(SymbolsTable& symbolsTable, const std::unique_ptr<AssignCommand>& cmd){
    std::vector<AssemblyInstruction> result;

    // addressOfResult is the memory address where the result of the expression will be stored
    auto [instructions, addressOfResult] = compile_EXPRESSION(symbolsTable, cmd->expression);
    result.insert(result.end(), instructions.begin(), instructions.end());

    Identifier& identifier = *(cmd->identifier);
    std::string id = identifier.id;
    // If the identifier is a variable
    if(identifier.isVariable()){

        // If variable is not declared, throw an error
        if(!symbolsTable.isVariableDeclared(id)){
            throw std::runtime_error("Variable '" + id + "' not declared but is used in ASSIGN command");
        }

        // Get memory address of the variable and store the result of the expression there
        ll address = symbolsTable.getMemoryAddress_variable(id);
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, addressOfResult));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, address));

        // Mark the variable as initialized
        symbolsTable.markAsInitialized(id);

        return result;
    }

    // If the identifier is an array
    ArrayAccess arrayAccess = identifier.getArrayAccess();

    // If array is not declared, throw an error
    if(!symbolsTable.isArrayDeclared(id)){
        throw std::runtime_error("Array '" + id + "' not declared but is used in ASSIGN command");
    }

    // If array is accessed by index
    if(arrayAccess.isByIndex()){

        ll index = arrayAccess.getIndex();

        // If index is out of bounds, throw an error
        if(!symbolsTable.isInsideArray(id, index)){
            throw std::runtime_error("Trying to assign '" + id + "' at index " + std::to_string(index) + " which is out of bounds");
        }

        // Get memory address of array at the index and store the result of the expression there
        ll address = symbolsTable.getMemoryAddress_at(id, index);
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, addressOfResult));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, address));

        // Mark the array as initialized at that index
        symbolsTable.markAsInitialized_at(id, index);

        return result;
    }

    // If array is accessed by variable
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // If index is not declared, throw an error
    if(!symbolsTable.isVariableDeclared(indexIdentifier)){
        throw std::runtime_error("Variable '" + indexIdentifier + "' not declared but is used as index in ASSIGN command");
    }

    // If index is declared but not initialized, throw an error
    if(!symbolsTable.isVariableInitialized(indexIdentifier)){
        throw std::runtime_error("Variable '" + indexIdentifier + "' not initialized but is used as index in ASSIGN command");
    }

    // Get memory address of index and store the result of the expression there. Account for the offset of the array
    ll indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
    ll arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

    result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddress));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddress));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, 1));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, addressOfResult));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::STOREI, 1));

    // Can't mark the array as initialized here, as we don't know the index yet
    return result;
}