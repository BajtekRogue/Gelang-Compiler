#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"

namespace LabelCounters {
    ll ifCounter = 0;
    ll whileCounter = 0;
    ll repeatCounter = 0;
    ll forCounter = 0;
    ll procedureCounter = 0;
}

std::vector<AssemblyInstruction> getValueToDestinationAddress(SymbolsTable& symbolsTable, const Value& val, ll destination){
    std::vector<AssemblyInstruction> result;

    // If it is a number
    if(val.isNumber()){

        ll num = val.asNumber();
        
        // If 0 clear the accumulator because it is cheaper
        if(num == 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 0));
        }else{
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, num));
        }

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
            return result;
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
            return result;
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


ll countRealInstructions(const std::vector<AssemblyInstruction>& instructions){
    ll count = 0;
    for (const AssemblyInstruction& instruction : instructions) {
        switch (instruction.type) {
            case AssemblyInstructionType::GET:
            case AssemblyInstructionType::PUT:
            case AssemblyInstructionType::LOAD:
            case AssemblyInstructionType::STORE:
            case AssemblyInstructionType::LOADI:
            case AssemblyInstructionType::STOREI:
            case AssemblyInstructionType::ADD:
            case AssemblyInstructionType::SUB:
            case AssemblyInstructionType::ADDI:
            case AssemblyInstructionType::SUBI:
            case AssemblyInstructionType::SET:
            case AssemblyInstructionType::HALF:
            case AssemblyInstructionType::JUMP:
            case AssemblyInstructionType::JPOS:
            case AssemblyInstructionType::JZERO:
            case AssemblyInstructionType::JNEG:
            case AssemblyInstructionType::RTRN:
            case AssemblyInstructionType::HALT:
                count++;
                break;
            default:
                break;
        }
    }
    return count;
}

std::string getStartLabel(std::string label){
    return "_" + label + "_: ";
}

std::string getEndLabel(std::string label){
    return "_" + label + "_.";
}

ll extractLoopLabel(const std::string& label) {
    size_t first = label.find('_');
    size_t second = label.find('_', first + 1);
    std::string number = label.substr(first + 1, second - first - 1);
    return std::stoll(number);
}


void fixUntilJump(std::vector<AssemblyInstruction>& code, ll loopSize, ll conditonSize){
    ll jumpSize = -loopSize - conditonSize + 1;
    ll lastIndex = code.size() - 1;
    ll supLastIndex = code.size() - 2;

    switch(code[supLastIndex].type){
        case AssemblyInstructionType::JUMP:
        case AssemblyInstructionType::JPOS:
        case AssemblyInstructionType::JZERO:
        case AssemblyInstructionType::JNEG:
            code[supLastIndex].address = jumpSize +1;
            code[lastIndex].address = jumpSize;
            break;
        default:
            code[lastIndex].address= jumpSize;
            break;
    }
}