#include "symbolsTable.hpp"

SymbolsTable::SymbolsTable(ll startingMemoryAddres) : lastMemoryAddress(startingMemoryAddres) {}

bool SymbolsTable::isVariableDeclared(const std::string& identifier) const{
    return variables.find(identifier) != variables.end();
}

bool SymbolsTable::isVariableInitialized(const std::string& identifier) const {
    return initialized_variables.at(identifier);
}

void SymbolsTable::markAsInitialized(const std::string& identifier) {
    initialized_variables[identifier] = true;
}

ll SymbolsTable::getMemoryAddress_variable(const std::string& identifier) const {
    return memoryAddresses_variables.at(identifier);
}

bool SymbolsTable::isArrayDeclared(const std::string& identifier) const{
    for(const auto& arr : arrays){
        if(arr.identifier == identifier){
            return true;
        }
    }
    return false;
}

ll SymbolsTable::getMemoryAddress_at(const std::string& identifier, ll index) const {
    for(const auto& arr : arrays){
        if(arr.identifier == identifier){
            return memoryAddresses_arrays.at(arr).first + index + arr.offset;
        }
    }
    throw std::runtime_error("Array not declared");
}

ll SymbolsTable::getMemoryAddress_start(const std::string& identifier) const {
    for(const auto& arr : arrays){
        if(arr.identifier == identifier){
            return memoryAddresses_arrays.at(arr).first;
        }
    }
    throw std::runtime_error("Array not declared");
}

bool SymbolsTable::isInsideArray(const std::string& identifier, ll index) const {
    for(const auto& arr : arrays){
        if(arr.identifier == identifier){
            return arr.isInside(index);
        }
    }
    return false;
}

ll SymbolsTable::get_offset(const std::string& identifier) const {
    for(const auto& arr : arrays){
        if(arr.identifier == identifier){
            return arr.offset;
        }
    }
    throw std::runtime_error("Array not declared");
}

void SymbolsTable::addVariable(const std::unique_ptr<Variable>& var) {
    __addVariable(var, lastMemoryAddress + 1);
}

void SymbolsTable::__addVariable(const std::unique_ptr<Variable>& var, ll memoryAddress) {

    if(var->type == VariableType::Integer){

        if (isVariableDeclared(var->identifier)) {
            throw std::runtime_error("Variable '" + var->identifier + "' already exists");
        }

        if(isArrayDeclared(var->identifier)){
            throw std::runtime_error("Array with identifier '" + var->identifier + "' already exists");
        }

        variables.insert(var->identifier);
        initialized_variables[var->identifier] = false;  
        memoryAddresses_variables[var->identifier] = memoryAddress;     
        lastMemoryAddress = memoryAddress;

    }else{

        ll start = var->arrayBounds.value().first;
        ll end = var->arrayBounds.value().second;

        if(start > end){
            throw std::runtime_error("Invalid array bounds for variable '" + var->identifier + "'");
        }

        if (isArrayDeclared(var->identifier)) {
            throw std::runtime_error("Array '" + var->identifier + "' already exists");
        }

        if(isVariableDeclared(var->identifier)){
            throw std::runtime_error("Variable with identifier '" + var->identifier + "' already exists");
        }

        Array_offset arr(var->identifier, start, end);
        
        arrays.insert(arr);
        memoryAddresses_arrays[arr] = {memoryAddress, memoryAddress + arr.size - 1};
        lastMemoryAddress = memoryAddress + arr.size - 1;
    }
}


void SymbolsTable::printSymbols() const {
    std::cout << "Variables:\n";
    for(const auto& var : variables){
        std::cout << var << " ";
    }
    std::cout << "\n";

    std::cout << "Arrays:\n";
    for(const auto& arr : arrays){
        std::cout << arr.identifier << "[0:" << arr.size - 1 << "] , offset: " << arr.offset << "\n";
    }
    std::cout << "\n";
}