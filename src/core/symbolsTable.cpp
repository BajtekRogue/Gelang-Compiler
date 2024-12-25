#include "symbolsTable.hpp"

SymbolsTable::SymbolsTable() {}

SymbolsTable::SymbolsTable(std::string procedureIdentifier, int64_t startingMemoryAddres) : _procedureIdentifier(procedureIdentifier), _lastMemoryAddress(startingMemoryAddres) {}

bool SymbolsTable::isVariableDeclared(const std::string& identifier) const{
    return _variables.find(identifier) != _variables.end();
}

bool SymbolsTable::isVariableInitialized(const std::string& identifier) const {
    return _initializedVariables.at(identifier);
}

void SymbolsTable::markAsInitialized(const std::string& identifier) {
    _initializedVariables[identifier] = true;
}

int64_t SymbolsTable::getMemoryAddress_variable(const std::string& identifier) const {
    return _memoryAddressesVariables.at(identifier);
}

bool SymbolsTable::isArrayDeclared(const std::string& identifier) const{
    for(const auto& arr : _arrays){
        if(arr.identifier == identifier){
            return true;
        }
    }
    return false;
}

int64_t SymbolsTable::getMemoryAddress_at(const std::string& identifier, int64_t index) const {
    for(const auto& arr : _arrays){
        if(arr.identifier == identifier){
            return _memoryAddressesArrays.at(arr).first + index + arr.offset;
        }
    }
    throw std::runtime_error("Array `" + identifier + "` not declared but someone is trying to access its memory address");
}

int64_t SymbolsTable::getMemoryAddress_start(const std::string& identifier) const {
    for(const auto& arr : _arrays){
        if(arr.identifier == identifier){
            return _memoryAddressesArrays.at(arr).first;
        }
    }
    throw std::runtime_error("Array `" + identifier + "` not declared but someone is trying to access its memory address");
}

bool SymbolsTable::isInsideArray(const std::string& identifier, int64_t index) const {
    for(const auto& arr : _arrays){
        if(arr.identifier == identifier){
            return arr.isInside(index);
        }
    }
    return false;
}

int64_t SymbolsTable::get_offset(const std::string& identifier) const {
    for(const auto& arr : _arrays){
        if(arr.identifier == identifier){
            return arr.offset;
        }
    }
    throw std::runtime_error("Array `" + identifier + "` not declared but someone is trying to access its memory address");
}

void SymbolsTable::addVariable(const std::unique_ptr<Variable>& var) {
    _addVariable(var, _lastMemoryAddress + 1);
}

void SymbolsTable::validateIterator(const std::string& identifier) const{
    auto it = std::find_if(_parameters.begin(), _parameters.end(),
                           [&identifier](const auto& pair) { return pair.first == identifier; });
    if (it != _parameters.end()) {
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
    _variables.insert(identifier);
    _initializedVariables[identifier] = true;
    _memoryAddressesVariables[identifier] = _lastMemoryAddress + 1;
    _lastMemoryAddress++;
    _iterators.insert(identifier);
}

void SymbolsTable::removeIterator(const std::string& identifier) {
    if (!isVariableDeclared(identifier)) {
        throw std::runtime_error("Iterator '" + identifier + "' does not exist but someone is trying to remove it");
    }

    _variables.erase(identifier);
    _initializedVariables.erase(identifier);
    _memoryAddressesVariables.erase(identifier);
    _iterators.erase(identifier);
    _memoryAddressesForLoopBounds.erase(identifier);
}

bool SymbolsTable::isIterator(const std::string& identifier) const {
    return _iterators.find(identifier) != _iterators.end();
}

void SymbolsTable::addForLoopBound(const std::string& identifier, const std::string& boundIdentifier) {
    _lastMemoryAddress++;
    _forLoopBounds[identifier] = boundIdentifier;
    _memoryAddressesForLoopBounds[identifier] = _memoryAddressesVariables.at(identifier) + 1;
}

int64_t SymbolsTable::getMemoryAddress_forLoopBound(const std::string& identifier) const {
    return _memoryAddressesForLoopBounds.at(identifier);
}

void SymbolsTable::addProcedure(const std::string& identifier, const std::vector<ParameterType>& parameters, const std::vector<int64_t>& parametersMemoryAddresses, int64_t returnAddress) {
    if (isProcedureDeclared(identifier)) {
        throw std::logic_error("Procedure '" + identifier + "' already exists");
    }

    for(size_t i = 0; i < parameters.size(); i++){
        _proceduresToCall[identifier].push_back(parameters[i]);
        _proceduresParametersMemoryAddressesToCall[identifier].push_back(parametersMemoryAddresses[i]);
    }

    _proceduresReturnAddressesToCall[identifier] = returnAddress;
}

bool SymbolsTable::isProcedureDeclared(const std::string& identifier) const {
    return _proceduresToCall.find(identifier) != _proceduresToCall.end();
}

const std::vector<ParameterType>& SymbolsTable::getProcedureParameters(const std::string& identifier) const {
    return _proceduresToCall.at(identifier);
}

const std::vector<int64_t>& SymbolsTable::getProcedureParametersMemoryAddresses(const std::string& identifier) const {
    return _proceduresParametersMemoryAddressesToCall.at(identifier);
}

int64_t SymbolsTable::getProcedureReturnAddress(const std::string& identifier) const {
    if(identifier == _procedureIdentifier){
        return _returnAddress;
    }
    return _proceduresReturnAddressesToCall.at(identifier);
}

int64_t SymbolsTable::getLastMemoryAddress() const {
    return _lastMemoryAddress;
}

void SymbolsTable::addParameter(const std::string& identifier, ParameterType type) {
    auto it = std::find_if(_parameters.begin(), _parameters.end(),
                           [&identifier](const auto& pair) { return pair.first == identifier; });

    if (it != _parameters.end()) {
        throw std::logic_error("Parameter with identifier '" + identifier + "' already exists");
    }

    _parameters.push_back({identifier, type});
    _memoryAddressesPointersParameters[identifier] = _lastMemoryAddress + 1;
    _lastMemoryAddress++;
}

ParameterType SymbolsTable::getParameterType(const std::string& identifier) const {
    auto it = std::find_if(_parameters.begin(), _parameters.end(),
                           [&identifier](const auto& pair) { return pair.first == identifier; });

    if (it == _parameters.end()) {
        throw std::runtime_error("Parameter with identifier '" + identifier + "' does not exist but someone is trying to access its type");
    }

    return it->second;
}


int64_t SymbolsTable::getMemoryAddressPointer_parameter(const std::string& identifier) const {
    return _memoryAddressesPointersParameters.at(identifier);
}

bool SymbolsTable::isLocalVariable(const std::string& identifier) const {
    return isVariableDeclared(identifier) || isIterator(identifier);
}

bool SymbolsTable::isParameter(const std::string& identifier) const {
    return std::find_if(_parameters.begin(), _parameters.end(),
                        [&identifier](const auto& pair) { return pair.first == identifier; }) != _parameters.end();
}


void SymbolsTable::_addVariable(const std::unique_ptr<Variable>& var, int64_t memoryAddress) {
    auto it = std::find_if(_parameters.begin(), _parameters.end(),
                           [&var](const auto& pair) { return pair.first == var->identifier; });

    if (it != _parameters.end()) {
        throw std::logic_error("Parameter with identifier '" + var->identifier + "' already exists but is being declared as a variable");
    }

    if(var->type == VariableType::Integer){

        if (isVariableDeclared(var->identifier)) {
            throw std::logic_error("Variable '" + var->identifier + "' already exists");
        }

        if(isArrayDeclared(var->identifier)){
            throw std::logic_error("Array with identifier '" + var->identifier + "' already exists");
        }

        _variables.insert(var->identifier);
        _initializedVariables[var->identifier] = false;  
        _memoryAddressesVariables[var->identifier] = memoryAddress;     
        _lastMemoryAddress = memoryAddress;

    }
    else{

        int64_t start = var->arrayBounds.value().first;
        int64_t end = var->arrayBounds.value().second;

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
        
        _arrays.insert(arr);
        _memoryAddressesArrays[arr] = {memoryAddress, memoryAddress + arr.size - 1};
        _lastMemoryAddress = memoryAddress + arr.size - 1;
    }
}


void SymbolsTable::printSymbols() const {
    std::cout << "Procedure: " << _procedureIdentifier << "\n";

    if(!_parameters.empty()){
        std::cout << "Parameters:\n";
        for(const auto& [id, type] : _parameters){
            std::cout << id << (type == ParameterType::Integer ? " (Integer)" : " (Array)") << " at p[" << _memoryAddressesPointersParameters.at(id) << "], ";
        }
        std::cout << "\n";
    }

    if(!_variables.empty()){
        std::cout << "Variables:\n";
        for(const auto& var : _variables){
            std::cout << var << " at p[" << _memoryAddressesVariables.at(var) << "], ";
        }
        std::cout << "\n";
    }

    if(!_arrays.empty()){
        std::cout << "Arrays:\n";
        for(const auto& arr : _arrays){
            std::cout << arr.identifier << "[0:" << arr.size - 1 << "] , offset: " << arr.offset << " at p[" << _memoryAddressesArrays.at(arr).first << "], ";
        }
        std::cout << "\n";
    }

    if(!_proceduresToCall.empty()){
        std::cout << "Procedures:\n";
        for(const auto& [proc, parameters] : _proceduresToCall){
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
    _returnAddress = _lastMemoryAddress + 1;
}

int64_t SymbolsTable::getReturnAddress() const {
    return _returnAddress;
}

std::vector<int64_t> SymbolsTable::getOwnParametersMemoryAddresses() const {
    std::vector<int64_t> addresses;
    for(const auto& [id, type] : _parameters){
        addresses.push_back(_memoryAddressesPointersParameters.at(id));
    }
    return addresses;
}

std::string SymbolsTable::getOwnIdentifier() const {
    return _procedureIdentifier;
}