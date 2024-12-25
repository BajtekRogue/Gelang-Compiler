#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "utlity.hpp"


namespace LabelCounters {
    int64_t ifCounter = 0;
    int64_t whileCounter = 0;
    int64_t repeatCounter = 0;
    int64_t forCounter = 0;
    int64_t procedureCounter = 0;
}

namespace Arithmetic {
    bool multiplication = false;
    bool division = false;
    bool modulo = false;
}

std::vector<AssemblyInstruction> getValueToDestinationAddress(SymbolsTable& symbolsTable, const Value& val, int64_t destination){

    std::vector<AssemblyInstruction> result;

    // If it is a number
    if(val.isNumber()){

        int64_t num = val.asNumber();
        
        // If 0 clear the accumulator because it is cheaper
        if(num == 0){
            result.push_back(AssemblyInstruction(Instruction::SUB, 0));
        }
        else{
            result.push_back(AssemblyInstruction(Instruction::SET, num));
        }

        // If destination is not the accumulator, store it
        if(destination != 0){
            result.push_back(AssemblyInstruction(Instruction::STORE, destination));
        }

        return result;
    }

    // If it is an identifier
    Identifier identifier = val.asIdentifier();
    std::string id = identifier.id;

    // If it is an integer parameter
    if(symbolsTable.isParameter(id) && symbolsTable.getParameterType(id) == ParameterType::Integer){

        int64_t parameterAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        // If destination is the accumulator, load the pointer
        if(destination == 0){
            result.push_back(AssemblyInstruction(Instruction::LOADI, parameterAddress));
            return result;
        }

        // Otherwise load the pointer and store its dereferenced value
        result.push_back(AssemblyInstruction(Instruction::LOADI, parameterAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE, destination));
        return result;
    }

    // Check if identifier is a variable
    if(identifier.isVariable()){

        int64_t currentAddress = symbolsTable.getMemoryAddress_variable(id);

        // If destination is current address, do nothing
        if(destination == currentAddress){
            return result;
        }

        // If destination is the accumulator, load it
        if(destination == 0){
            result.push_back(AssemblyInstruction(Instruction::LOAD, currentAddress));
            return result;
        }

        // Otherwise load and store it
        result.push_back(AssemblyInstruction(Instruction::LOAD, currentAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE, destination));
        return result;
    }

    // If it is an array
    ArrayAccess arrayAccess = identifier.getArrayAccess();

    // If array is accessed by index but is not a parameter
    if(arrayAccess.isByIndex() && !symbolsTable.isParameter(id)){

        int64_t index = arrayAccess.getIndex();

        // Get memory address of array at the index and ouput the value there
        int64_t currentAddress = symbolsTable.getMemoryAddress_at(id, index);

        // If destination is current address, do nothing
        if(destination == currentAddress){
            return result;
        }

        // If destination is the accumulator, load it
        if(destination == 0){
            result.push_back(AssemblyInstruction(Instruction::LOAD, currentAddress));
            return result;
        }

        // Otherwise load and store it
        result.push_back(AssemblyInstruction(Instruction::LOAD, currentAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE, destination));
        return result;
    }

    // If it is accessed by index but is a parameter
    if(arrayAccess.isByIndex() && symbolsTable.isParameter(id)){

        int64_t index = arrayAccess.getIndex();
        int64_t arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        // If index is 0 we can just load the pointer
        if(index == 0){
            result.push_back(AssemblyInstruction(Instruction::LOADI, arrayAddress));

            // If destination is not the accumulator, store it
            if(destination != 0){
                result.push_back(AssemblyInstruction(Instruction::STORE, destination));
            }

            return result;
        }

        // Otherwise load the pointer and add the index
        result.push_back(AssemblyInstruction(Instruction::SET, index));
        result.push_back(AssemblyInstruction(Instruction::ADD, arrayAddress));
        result.push_back(AssemblyInstruction(Instruction::LOADI, 0));

        // If destination is not the accumulator, store it
        if(destination != 0){
            result.push_back(AssemblyInstruction(Instruction::STORE, destination));
        }

        return result;
    }

    // If the array is accessed by a variable
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // If both array and the variable are local
    if(!symbolsTable.isParameter(id) && !symbolsTable.isParameter(indexIdentifier)){

        int64_t indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
        int64_t arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

        // Load into the accumulator the value at the index of the array
        result.push_back(AssemblyInstruction(Instruction::SET, arrayStartAddress));
        result.push_back(AssemblyInstruction(Instruction::ADD, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::LOADI, 0));

        // If destination is not the accumulator, store it
        if(destination != 0){
            result.push_back(AssemblyInstruction(Instruction::STORE, destination));
        }

        return result;
    }

    // If array is local but the index is a parameter
    if(!symbolsTable.isParameter(id) && symbolsTable.isParameter(indexIdentifier)){

        int64_t indexAddress = symbolsTable.getMemoryAddressPointer_parameter(indexIdentifier);
        int64_t arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

        // Load into the accumulator the value at the index of the array
        result.push_back(AssemblyInstruction(Instruction::SET, arrayStartAddress));
        result.push_back(AssemblyInstruction(Instruction::ADDI, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::LOADI, 0));

        // If destination is not the accumulator, store it
        if(destination != 0){
            result.push_back(AssemblyInstruction(Instruction::STORE, destination));
        }

        return result;       
    }
    
    // If array is a parameter but the index is local
    if(symbolsTable.isParameter(id) && !symbolsTable.isParameter(indexIdentifier)){

        int64_t indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
        int64_t arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        // Load into the accumulator the value at the index of the array
        result.push_back(AssemblyInstruction(Instruction::LOAD, arrayAddress));
        result.push_back(AssemblyInstruction(Instruction::ADD, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::LOADI, 0));

        // If destination is not the accumulator, store it
        if(destination != 0){
            result.push_back(AssemblyInstruction(Instruction::STORE, destination));
        }

        return result;
    }

    // If both array and the index are parameters
    if(symbolsTable.isParameter(id) && symbolsTable.isParameter(indexIdentifier)){

        int64_t indexAddress = symbolsTable.getMemoryAddressPointer_parameter(indexIdentifier);
        int64_t arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        // Load into the accumulator the value at the index of the array
        result.push_back(AssemblyInstruction(Instruction::LOAD, arrayAddress));
        result.push_back(AssemblyInstruction(Instruction::ADDI, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::LOADI, 0));

        // If destination is not the accumulator, store it
        if(destination != 0){
            result.push_back(AssemblyInstruction(Instruction::STORE, destination));
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

        int64_t index = arrayAccess.getIndex();

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

bool isRealInstruction(AssemblyInstruction ins){
    switch (ins.instruction) {
        case Instruction::GET:
        case Instruction::PUT:
        case Instruction::LOAD:
        case Instruction::STORE:
        case Instruction::LOADI:
        case Instruction::STOREI:
        case Instruction::ADD:
        case Instruction::SUB:
        case Instruction::ADDI:
        case Instruction::SUBI:
        case Instruction::SET:
        case Instruction::HALF:
        case Instruction::JUMP:
        case Instruction::JPOS:
        case Instruction::JZERO:
        case Instruction::JNEG:
        case Instruction::RTRN:
        case Instruction::HALT:
            return true;
        default:
            return false;
    }
}

int64_t countRealInstructions(const std::vector<AssemblyInstruction>& instructions){
    int64_t count = 0;
    for (const auto& instruction : instructions) {
        if (isRealInstruction(instruction)) {
            count++;
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

int64_t extractLoopLabel(const std::string& label) {
    size_t first = label.find('_');
    size_t second = label.find('_', first + 1);
    std::string number = label.substr(first + 1, second - first - 1);
    return std::stoll(number);
}

void fixUntilJump(std::vector<AssemblyInstruction>& code, int64_t loopSize, int64_t conditonSize){
    int64_t jumpSize = -loopSize - conditonSize + 1;
    int64_t lastIndex = code.size() - 1;
    int64_t supLastIndex = code.size() - 2;

    switch(code[supLastIndex].instruction){
        case Instruction::JUMP:
        case Instruction::JPOS:
        case Instruction::JZERO:
        case Instruction::JNEG:
            code[supLastIndex].address = jumpSize + 1;
            code[lastIndex].address = jumpSize;
            break;
        default:
            code[lastIndex].address= jumpSize;
            break;
    }
}

int64_t findNext1000(int64_t n){
    return (n / 1000 + 1) * 1000;
}

std::vector<AssemblyInstruction> getAddressToDestinationAddress(SymbolsTable& symbolsTable, const Value& val, int64_t destination){

    std::vector<AssemblyInstruction> result;
    Identifier identifier = val.asIdentifier();

    // If variable is local
    if(symbolsTable.isVariableDeclared(identifier.id)){
        
        int64_t varAddress = symbolsTable.getMemoryAddress_variable(identifier.id);

        result.push_back(AssemblyInstruction(Instruction::SET, varAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE, destination));

        return result;
    }

    // If variable is a parameter
    if(symbolsTable.isParameter(identifier.id) && symbolsTable.getParameterType(identifier.id) == ParameterType::Integer){
        
        int64_t paramAddress = symbolsTable.getMemoryAddressPointer_parameter(identifier.id);

        result.push_back(AssemblyInstruction(Instruction::LOAD, paramAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE, destination));

        return result;
    }

    // If variable is a local array
    if(symbolsTable.isArrayDeclared(identifier.id)){
        
        int64_t arrayAddress = symbolsTable.getMemoryAddress_start(identifier.id) + symbolsTable.get_offset(identifier.id);

        result.push_back(AssemblyInstruction(Instruction::SET, arrayAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE, destination));

        return result;
    }

    // If variable is a parameter array
    if(symbolsTable.isParameter(identifier.id) && symbolsTable.getParameterType(identifier.id) == ParameterType::Array){
        
        int64_t arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(identifier.id);

        result.push_back(AssemblyInstruction(Instruction::LOAD, arrayAddress));
        result.push_back(AssemblyInstruction(Instruction::STORE, destination));

        return result;
    }
    return result;
}

void fixProcedureCallsJumps(std::vector<AssemblyInstruction>& code){

    int realIdx = 0;
    std::unordered_map<std::string, int64_t> procedureStartAddresses;

    for(size_t j = 0; j < code.size(); j++){

        // Keep count of real instructions
        if(isRealInstruction(code[j])){
            realIdx++;
        }

        // If it is a procedure label, save its address
        if(code[j].instruction == Instruction::LABEL_PROCEDURE){
            procedureStartAddresses[code[j].getLabel()] = realIdx + 1;
        }

        // If it is a SET of the return, set the address to the return address
        if(code[j].instruction == Instruction::LABEL_INSTRUCTION && code[j].getLabel().find("Setting return address and jumping to ") != std::string::npos){
            code[j+1].address = realIdx + 3;
        }

        // If it is a JUMP to a procedure, set the address to the start of the procedure
        if(code[j].instruction == Instruction::JUMP && code[j].hasLabel()){
            code[j].address = procedureStartAddresses[code[j].getLabel()] - realIdx;
        }
    }
}

