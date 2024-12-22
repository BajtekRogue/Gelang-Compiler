#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "utlity.hpp"


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

    // If it is an integer parameter
    if(symbolsTable.isParameter(id) && symbolsTable.getParameterType(id) == ParameterType::Integer){

        ll parameterAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        // If destination is the accumulator, load the pointer
        if(destination == 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::LOADI, parameterAddress));
            return result;
        }

        // Otherwise load the pointer and store its dereferenced value
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOADI, parameterAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, parameterAddress));
        return result;
    }

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

    // If array is accessed by index but is not a parameter
    if(arrayAccess.isByIndex() && !symbolsTable.isParameter(id)){

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

    // If it is accessed by index but is a parameter
    if(arrayAccess.isByIndex() && symbolsTable.isParameter(id)){

        ll index = arrayAccess.getIndex();
        ll arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        // If index is 0 we can just load the pointer
        if(index == 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::LOADI, arrayAddress));

            // If destination is not the accumulator, store it
            if(destination != 0){
                result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, destination));
            }

            return result;
        }

        // Otherwise load the pointer and add the index
        result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, index));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::ADDI, arrayAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, 0));

        // If destination is not the accumulator, store it
        if(destination != 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, destination));
        }

        return result;
    }

    // If the array is accessed by a variable
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // If both array and the variable are local
    if(!symbolsTable.isParameter(id) && !symbolsTable.isParameter(indexIdentifier)){

        ll indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
        ll arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

        // Load into the accumulator the value at the index of the array
        result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, 0));

        // If destination is not the accumulator, store it
        if(destination != 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, destination));
        }

        return result;
    }

    // If array is local but the index is a parameter
    if(!symbolsTable.isParameter(id) && symbolsTable.isParameter(indexIdentifier)){

        ll indexAddress = symbolsTable.getMemoryAddressPointer_parameter(indexIdentifier);
        ll arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

        // Load into the accumulator the value at the index of the array
        result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::ADDI, indexAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, 0));

        // If destination is not the accumulator, store it
        if(destination != 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, destination));
        }

        return result;       
    }
    
    // If array is a parameter but the index is local
    if(symbolsTable.isParameter(id) && !symbolsTable.isParameter(indexIdentifier)){

        ll indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
        ll arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        // Load into the accumulator the value at the index of the array
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOADI, arrayAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, 0));

        // If destination is not the accumulator, store it
        if(destination != 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, destination));
        }

        return result;
    }

    // If both array and the index are parameters
    if(symbolsTable.isParameter(id) && symbolsTable.isParameter(indexIdentifier)){

        ll indexAddress = symbolsTable.getMemoryAddressPointer_parameter(indexIdentifier);
        ll arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        // Load into the accumulator the value at the index of the array
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOADI, arrayAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::ADDI, indexAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, 0));

        // If destination is not the accumulator, store it
        if(destination != 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, destination));
        }

        return result;
    }

    return result;
}


void validateUseOfVariable(SymbolsTable& symbolsTable, const Identifier& identifier, const std::string commandName, bool mustBeInitialized, bool arrayAsPointer){
    
    std::string id = identifier.id;

    // If it is an array as pointer
    if(arrayAsPointer){
        
        // If it is a parameter, assume declared and initialized
        if(symbolsTable.isParameter(id)){
            return;
        }
        
        // If array is not declared, throw an error
        if(!symbolsTable.isArrayDeclared(id)){
            throw std::logic_error("Array '" + id + "' not declared but is used as a pointer in " + commandName);
        }

        return;
    }

    // Check if identifier is a variable
    if(identifier.isVariable()){

        // If it is a parameter, assume declared and initialized
        if(symbolsTable.isParameter(id)){

            // If parameter is an array, throw an error
            if(symbolsTable.getParameterType(id) == ParameterType::Array){
                throw std::logic_error("Parameter '" + id + "' is an array but is used as a variable in " + commandName);
            }
            return;
        }

        // If it is an array but is used as variable, throw an error
        if(symbolsTable.isArrayDeclared(id)){
            throw std::logic_error("Array '" + id + "' is used as a variable in " + commandName);
        }

        // If identifier is not declared, throw an error
        if(!symbolsTable.isVariableDeclared(id)){
            throw std::logic_error("Variable '" + id + "' not declared but is used in " + commandName);
        }

        // If variable is not initialized, throw an error
        if(mustBeInitialized && !symbolsTable.isVariableInitialized(id)){
            throw std::logic_error("Variable '" + id + "' not initialized but is used in " + commandName);
        }

        return;
    }

    // If it is an array
    ArrayAccess arrayAccess = identifier.getArrayAccess();

    // If it is a integer parameter, throw an error
    if(symbolsTable.isParameter(id) && symbolsTable.getParameterType(id) == ParameterType::Integer){
        throw std::logic_error("Parameter '" + id + "' is an integer but is used as an array in " + commandName);
    }

    // If array is not declared, throw an error
    if(!symbolsTable.isArrayDeclared(id) && !symbolsTable.isParameter(id)){
        throw std::logic_error("Array '" + id + "' not declared but is used in " + commandName);
    }

    // If variable is used as an array, throw an error
    if(symbolsTable.isVariableDeclared(id)){
        throw std::logic_error("Variable '" + id + "' is used as an array in " + commandName);
    }

    // If array is accessed by index
    if(arrayAccess.isByIndex()){

        ll index = arrayAccess.getIndex();

        // If index is out of bounds, throw an error
        // Cannot check if this is correct for parameters arrays
        if(!symbolsTable.isInsideArray(id, index) && !symbolsTable.isParameter(id)){
            throw std::logic_error("Index '" + std::to_string(index) + "' which is out of bounds for the array '" + id + "' used in " + commandName);
        }

        return;
    }

    // If the array is accessed by a variable
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // If index is a integer parameter, it is ok
    if(symbolsTable.isParameter(indexIdentifier) && symbolsTable.getParameterType(indexIdentifier) == ParameterType::Integer){
        return;
    }

    // If index is an array parameter, throw an error
    if(symbolsTable.isParameter(indexIdentifier) && symbolsTable.getParameterType(indexIdentifier) == ParameterType::Array){
        throw std::logic_error("Parameter '" + indexIdentifier + "' is an array but is used as an index in " + commandName);
    }

    // If index is an array variable, throw an error
    if(symbolsTable.isArrayDeclared(indexIdentifier)){
        throw std::logic_error("Array '" + indexIdentifier + "' is used as an index in " + commandName);
    }

    // If index is not declared, throw an error
    if(!symbolsTable.isVariableDeclared(indexIdentifier)){
        throw std::logic_error("Variable '" + indexIdentifier + "' not declared but is used as index in " + commandName);
    }

    // If index is not initialized, throw an error
    if(!symbolsTable.isVariableInitialized(indexIdentifier)){
        throw std::logic_error("Variable '" + indexIdentifier + "' not initialized but is used as index in " + commandName);
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

ll findNext1000(ll n){
    return (n / 1000 + 1) * 1000;
}

std::vector<AssemblyInstruction> getAddressToDestinationAddress(SymbolsTable& symbolsTable, const Value& val, ll destination){
    //TODO: Implement
    // this will only be used to set parameters pointers in procedures properly
}

void fixProcedureCallsJumps(std::vector<AssemblyInstruction>& code, const std::vector<SymbolsTable>& proceduresTables){

    //TODO: Implement
    // this will be called at the end of compilation and is supposed to set jump and return addresses of procedure calls properly



}