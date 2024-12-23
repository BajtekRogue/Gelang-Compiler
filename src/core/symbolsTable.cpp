#include "symbolsTable.hpp"

SymbolsTable::SymbolsTable() {}

SymbolsTable::SymbolsTable(std::string procedureIdentifier, ll startingMemoryAddres) : procedureIdentifier(procedureIdentifier), lastMemoryAddress(startingMemoryAddres) {}

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
    throw std::runtime_error("Array `" + identifier + "` not declared but someone is trying to access its memory address");
}

ll SymbolsTable::getMemoryAddress_start(const std::string& identifier) const {
    for(const auto& arr : arrays){
        if(arr.identifier == identifier){
            return memoryAddresses_arrays.at(arr).first;
        }
    }
    throw std::runtime_error("Array `" + identifier + "` not declared but someone is trying to access its memory address");
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
    throw std::runtime_error("Array `" + identifier + "` not declared but someone is trying to access its memory address");
}

void SymbolsTable::addVariable(const std::unique_ptr<Variable>& var) {
    __addVariable(var, lastMemoryAddress + 1);
}

void SymbolsTable::validateIterator(const std::string& identifier) const{
    auto it = std::find_if(parameters.begin(), parameters.end(),
                           [&identifier](const auto& pair) { return pair.first == identifier; });
    if (it != parameters.end()) {
        throw std::logic_error("Parameter with identifier '" + identifier + "' already exists but is being used as an iterator");
    }

    if (isVariableDeclared(identifier)) {
        throw std::logic_error("Variable '" + identifier + "' already exists but is being used as iterator");
    }

    if(isArrayDeclared(identifier)){
        throw std::logic_error("Array with identifier '" + identifier + "' already exists but is being used as iterator");
    }
}

void SymbolsTable::addIterator(const std::string& identifier) {
    validateIterator(identifier);
    variables.insert(identifier);
    initialized_variables[identifier] = true;
    memoryAddresses_variables[identifier] = lastMemoryAddress + 1;
    lastMemoryAddress++;
    iterators.insert(identifier);
}

void SymbolsTable::removeIterator(const std::string& identifier) {
    if (!isVariableDeclared(identifier)) {
        throw std::runtime_error("Iterator '" + identifier + "' does not exist but someone is trying to remove it");
    }

    variables.erase(identifier);
    initialized_variables.erase(identifier);
    memoryAddresses_variables.erase(identifier);
    iterators.erase(identifier);
    memoryAddresses_forLoopBounds.erase(identifier);
}

bool SymbolsTable::isIterator(const std::string& identifier) const {
    return iterators.find(identifier) != iterators.end();
}

void SymbolsTable::addForLoopBound(const std::string& identifier, const std::string& boundIdentifier) {
    lastMemoryAddress++;
    forLoopBounds[identifier] = boundIdentifier;
    memoryAddresses_forLoopBounds[identifier] = memoryAddresses_variables.at(identifier) + 1;
}

ll SymbolsTable::getMemoryAddress_forLoopBound(const std::string& identifier) const {
    return memoryAddresses_forLoopBounds.at(identifier);
}

void SymbolsTable::addProcedure(const std::string& identifier, const std::vector<ParameterType>& parameters, const std::vector<ll>& parametersMemoryAddresses, ll returnAddress) {
    if (isProcedureDeclared(identifier)) {
        throw std::logic_error("Procedure '" + identifier + "' already exists");
    }

    for(size_t i = 0; i < parameters.size(); i++){
        procedures_toCall[identifier].push_back(parameters[i]);
        proceduresParametersMemoryAddresses_toCall[identifier].push_back(parametersMemoryAddresses[i]);
    }

    proceduresReturnAddresses_toCall[identifier] = returnAddress;
}

bool SymbolsTable::isProcedureDeclared(const std::string& identifier) const {
    return procedures_toCall.find(identifier) != procedures_toCall.end();
}

const std::vector<ParameterType>& SymbolsTable::getProcedureParameters(const std::string& identifier) const {
    return procedures_toCall.at(identifier);
}

const std::vector<ll>& SymbolsTable::getProcedureParametersMemoryAddresses(const std::string& identifier) const {
    return proceduresParametersMemoryAddresses_toCall.at(identifier);
}

ll SymbolsTable::getProcedureReturnAddress(const std::string& identifier) const {
    if(identifier == procedureIdentifier){
        return returnAddress;
    }
    return proceduresReturnAddresses_toCall.at(identifier);
}

ll SymbolsTable::getLastMemoryAddress() const {
    return lastMemoryAddress;
}

void SymbolsTable::addParameter(const std::string& identifier, ParameterType type) {
    auto it = std::find_if(parameters.begin(), parameters.end(),
                           [&identifier](const auto& pair) { return pair.first == identifier; });

    if (it != parameters.end()) {
        throw std::logic_error("Parameter with identifier '" + identifier + "' already exists");
    }

    parameters.push_back({identifier, type});
    memoryAddressesPointers_parameters[identifier] = lastMemoryAddress + 1;
    lastMemoryAddress++;
}

ParameterType SymbolsTable::getParameterType(const std::string& identifier) const {
    auto it = std::find_if(parameters.begin(), parameters.end(),
                           [&identifier](const auto& pair) { return pair.first == identifier; });

    if (it == parameters.end()) {
        throw std::runtime_error("Parameter with identifier '" + identifier + "' does not exist but someone is trying to access its type");
    }

    return it->second;
}


ll SymbolsTable::getMemoryAddressPointer_parameter(const std::string& identifier) const {
    return memoryAddressesPointers_parameters.at(identifier);
}

bool SymbolsTable::isLocalVariable(const std::string& identifier) const {
    return isVariableDeclared(identifier) || isIterator(identifier);
}

bool SymbolsTable::isParameter(const std::string& identifier) const {
    return std::find_if(parameters.begin(), parameters.end(),
                        [&identifier](const auto& pair) { return pair.first == identifier; }) != parameters.end();
}


void SymbolsTable::__addVariable(const std::unique_ptr<Variable>& var, ll memoryAddress) {
    auto it = std::find_if(parameters.begin(), parameters.end(),
                           [&var](const auto& pair) { return pair.first == var->identifier; });

    if (it != parameters.end()) {
        throw std::logic_error("Parameter with identifier '" + var->identifier + "' already exists but is being declared as a variable");
    }

    if(var->type == VariableType::Integer){

        if (isVariableDeclared(var->identifier)) {
            throw std::logic_error("Variable '" + var->identifier + "' already exists");
        }

        if(isArrayDeclared(var->identifier)){
            throw std::logic_error("Array with identifier '" + var->identifier + "' already exists");
        }

        variables.insert(var->identifier);
        initialized_variables[var->identifier] = false;  
        memoryAddresses_variables[var->identifier] = memoryAddress;     
        lastMemoryAddress = memoryAddress;

    }else{

        ll start = var->arrayBounds.value().first;
        ll end = var->arrayBounds.value().second;

        if(start > end){
            throw std::logic_error("Invalid array bounds for variable '" + var->identifier + "'");
        }

        if (isArrayDeclared(var->identifier)) {
            throw std::logic_error("Array '" + var->identifier + "' already exists");
        }

        if(isVariableDeclared(var->identifier)){
            throw std::logic_error("Variable with identifier '" + var->identifier + "' already exists");
        }

        Array_offset arr(var->identifier, start, end);
        
        arrays.insert(arr);
        memoryAddresses_arrays[arr] = {memoryAddress, memoryAddress + arr.size - 1};
        lastMemoryAddress = memoryAddress + arr.size - 1;
    }
}


void SymbolsTable::printSymbols() const {
    std::cout << "Procedure: " << procedureIdentifier << "\n";

    if(!parameters.empty()){
        std::cout << "Parameters:\n";
        for(const auto& [id, type] : parameters){
            std::cout << id << (type == ParameterType::Integer ? " (Integer)" : " (Array)") << " at p[" << memoryAddressesPointers_parameters.at(id) << "], ";
        }
        std::cout << "\n";
    }

    if(!variables.empty()){
        std::cout << "Variables:\n";
        for(const auto& var : variables){
            std::cout << var << " at p[" << memoryAddresses_variables.at(var) << "], ";
        }
        std::cout << "\n";
    }

    if(!arrays.empty()){
        std::cout << "Arrays:\n";
        for(const auto& arr : arrays){
            std::cout << arr.identifier << "[0:" << arr.size - 1 << "] , offset: " << arr.offset << " at p[" << memoryAddresses_arrays.at(arr).first << "], ";
        }
        std::cout << "\n";
    }

    if(!procedures_toCall.empty()){
        std::cout << "Procedures:\n";
        for(const auto& [proc, parameters] : procedures_toCall){
            std::cout << proc << "< ";
            for(const auto& param : parameters){
                std::cout << (param == ParameterType::Integer ? ("Integer") : "Array") << ", ";
            }
            std::cout << ">\n";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
}

void SymbolsTable::setReturnAddress() {
    returnAddress = lastMemoryAddress + 1;
}

ll SymbolsTable::getReturnAddress() const {
    return returnAddress;
}

std::vector<ll> SymbolsTable::getOwnParametersMemoryAddresses() const {
    std::vector<ll> addresses;
    for(const auto& [id, type] : parameters){
        addresses.push_back(memoryAddressesPointers_parameters.at(id));
    }
    return addresses;
}

std::string SymbolsTable::getOwnIdentifier() const {
    return procedureIdentifier;
}