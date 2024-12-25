#ifndef SYMBOLS_TABLE_HPP
#define SYMBOLS_TABLE_HPP

#include "languageStructs.hpp"
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>
#include <algorithm>
#include <cinttypes>


// make sure arrays are indexed from 0
struct Array_offset{
    std::string identifier;
    int64_t size;
    int64_t offset;

    Array_offset(const std::string& id, int64_t start, int64_t end){
        identifier = id;
        offset = -start;
        size = end - start + 1;
    } 

    bool operator==(const Array_offset& other) const {
        return identifier == other.identifier && offset == other.offset && size == other.size;

    }
    
    bool operator<(const Array_offset& other) const {
        if (identifier != other.identifier) {
            return identifier < other.identifier;
        }else if (offset != other.offset) {
            return offset < other.offset;
        }else{
            return size < other.size;
        }
    }

    bool isInside(int64_t index) const {
        return index >= -offset && index < size - offset;
    }
};

// hash function for Array_offset
namespace std {
    template <>
    struct hash<Array_offset> {
        std::size_t operator()(const Array_offset& k) const {
            return ((std::hash<std::string>()(k.identifier)
                    ^ (std::hash<int64_t>()(k.offset) << 1)) >> 1)
                    ^ (std::hash<int64_t>()(k.size) << 1);
        }
    };
}

class SymbolsTable {
public:
    SymbolsTable();
    SymbolsTable(std::string procedureIdentifier, int64_t startingMemoryAddres);

    bool isVariableDeclared(const std::string& identifier) const;
    bool isVariableInitialized(const std::string& identifier) const;
    void markAsInitialized(const std::string& identifier);
    int64_t getMemoryAddress_variable(const std::string& identifier) const;

    bool isArrayDeclared(const std::string& identifier) const;
    int64_t getMemoryAddress_at(const std::string& identifier, int64_t index) const;
    int64_t getMemoryAddress_start(const std::string& identifier) const;
    bool isInsideArray(const std::string& identifier, int64_t index) const;
    int64_t get_offset(const std::string& identifier) const;
    
    void addVariable(const std::unique_ptr<Variable>& var);

    void validateIterator(const std::string& identifier) const;
    void addIterator(const std::string& identifier);
    void removeIterator(const std::string& identifier);
    bool isIterator(const std::string& identifier) const;
    void addForLoopBound(const std::string& identifier, const std::string& boundIdentifier);
    int64_t getMemoryAddress_forLoopBound(const std::string& identifier) const;
    
    void addProcedure(const std::string& identifier, const std::vector<ParameterType>& parameters, const std::vector<int64_t>& parametersMemoryAddresses, int64_t returnAddress);
    bool isProcedureDeclared(const std::string& identifier) const;
    const std::vector<ParameterType>& getProcedureParameters(const std::string& identifier) const;
    const std::vector<int64_t>& getProcedureParametersMemoryAddresses(const std::string& identifier) const;
    int64_t getProcedureReturnAddress(const std::string& identifier) const;

    void addParameter(const std::string& identifier, ParameterType type);
    ParameterType getParameterType(const std::string& identifier) const;
    int64_t getMemoryAddressPointer_parameter(const std::string& identifier) const;

    int64_t getLastMemoryAddress() const;
    
    bool isLocalVariable(const std::string& identifier) const;
    bool isParameter(const std::string& identifier) const;

    void printSymbols() const;

    void setReturnAddress();
    int64_t getReturnAddress() const;

    std::vector<int64_t> getOwnParametersMemoryAddresses() const;
    std::string getOwnIdentifier() const;

private:
    std::string _procedureIdentifier;

    std::unordered_set<std::string> _variables;
    std::unordered_map<std::string, bool> _initializedVariables;
    std::unordered_map<std::string, int64_t> _memoryAddressesVariables;

    std::unordered_set<Array_offset> _arrays;
    std::unordered_map<Array_offset, std::pair<int64_t, int64_t>> _memoryAddressesArrays;

    std::unordered_set<std::string> _iterators;
    std::unordered_map<std::string, std::string> _forLoopBounds;
    std::unordered_map<std::string, int64_t> _memoryAddressesForLoopBounds;

    std::unordered_map<std::string, std::vector<ParameterType>> _proceduresToCall;
    std::unordered_map<std::string, std::vector<int64_t>> _proceduresParametersMemoryAddressesToCall;
    std::unordered_map<std::string, int64_t> _proceduresReturnAddressesToCall;

    std::vector<std::pair<std::string, ParameterType>> _parameters;
    std::unordered_map<std::string, int64_t> _memoryAddressesPointersParameters;

    int64_t _lastMemoryAddress;
    int64_t _returnAddress;

    void _addVariable(const std::unique_ptr<Variable>& var, int64_t memoryAddress);

};

#endif // SYMBOLS_TABLE_HPP