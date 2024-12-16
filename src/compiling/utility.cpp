#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"

// Loads value to the destination address
std::vector<AssemblyInstruction> getValueToDestinationAddress(SymbolsTable& symbolsTable, const Value& val, ll destination){
    std::vector<AssemblyInstruction> result;

    // If it is a number
    if(val.isNumber()){

        ll num = val.asNumber();
        result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, num));

        // If destination is not the accumulator, store it
        if(destination != 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, destination));
        }

        return result;
    }

    // If it is an identifier
    Identifier identifier = val.asIdentifier();
    std::string id = identifier.id;

    // Check if identifier is a variable
    if(identifier.isVariable()){

        ll currentAddress = symbolsTable.getMemoryAddress_variable(id);

        // If destination is current address, do nothing
        if(destination == currentAddress){
            return {};
        }

        // If destination is the accumulator, load it
        if(destination == 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, currentAddress));
            return result;
        }

        // Otherwise load and store it
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, currentAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, destination));
        return result;
    }

    // If it is an array
    ArrayAccess arrayAccess = identifier.getArrayAccess();

    // If array is accessed by index
    if(arrayAccess.isByIndex()){

        ll index = arrayAccess.getIndex();

        // Get memory address of array at the index and ouput the value there
        ll currentAddress = symbolsTable.getMemoryAddress_at(id, index);

        // If destination is current address, do nothing
        if(destination == currentAddress){
            return {};
        }

        // If destination is the accumulator, load it
        if(destination == 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, currentAddress));
            return result;
        }

        // Otherwise load and store it
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, currentAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, destination));
        return result;
    }

    // If the array is accessed by a variable
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    ll indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
    ll arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

    // Load into the accumulator the value at the index of the array
    result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddress));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddress));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::LOADI, 0));

    // If destination is not the accumulator, store it
    if(destination != 0){
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, destination));
    }
    
    return result;
}

void validateUseOfVariable(SymbolsTable& symbolsTable, const Identifier& identifier, const std::string commandName, bool mustBeInitialized){
    
    // If it is an identifier
    std::string id = identifier.id;

    // Check if identifier is a variable
    if(identifier.isVariable()){

        // If identifier is not declared, throw an error
        if(!symbolsTable.isVariableDeclared(id)){
            throw std::runtime_error("Variable '" + id + "' not declared but is used in " + commandName);
        }

        // If variable is not initialized, throw an error
        if(mustBeInitialized && !symbolsTable.isVariableInitialized(id)){
            throw std::runtime_error("Variable '" + id + "' not initialized but is used in " + commandName);
        }

        return;
    }

    // If it is an array
    ArrayAccess arrayAccess = identifier.getArrayAccess();

    // If array is not declared, throw an error
    if(!symbolsTable.isArrayDeclared(id)){
        throw std::runtime_error("Array '" + id + "' not declared but is used in " + commandName);
    }

    // If array is accessed by index
    if(arrayAccess.isByIndex()){

        ll index = arrayAccess.getIndex();

        // If index is out of bounds, throw an error
        if(!symbolsTable.isInsideArray(id, index)){
            throw std::runtime_error("Index '" + std::to_string(index) + "' which is out of bounds for the array '" + id + "' used in " + commandName);
        }

        // If array is not initialized, throw an error
        if(mustBeInitialized && !symbolsTable.isArrayInitialized_at(id, index)){
            throw std::runtime_error("Array '" + id + "' at index " + std::to_string(index) + " not initialized but is used in " + commandName);
        }

        return;
    }

    // If the array is accessed by a variable
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // If index is not declared, throw an error
    if(!symbolsTable.isVariableDeclared(indexIdentifier)){
        throw std::runtime_error("Variable '" + indexIdentifier + "' not declared but is used as index in " + commandName);
    }

    return;
}