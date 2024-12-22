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

typedef long long ll;


// make sure arrays are indexed from 0
struct Array_offset{
    std::string identifier;
    ll size;
    ll offset;

    Array_offset(const std::string& id, ll start, ll end){
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

    bool isInside(ll index) const {
        return index >= -offset && index < size - offset;
    }
};

// hash function for Array_offset
namespace std {
    template <>
    struct hash<Array_offset> {
        std::size_t operator()(const Array_offset& k) const {
            return ((std::hash<std::string>()(k.identifier)
                    ^ (std::hash<ll>()(k.offset) << 1)) >> 1)
                    ^ (std::hash<ll>()(k.size) << 1);
        }
    };
}

class SymbolsTable {
public:
    SymbolsTable();
    SymbolsTable(std::string procedureIdentifier, ll startingMemoryAddres);

    bool isVariableDeclared(const std::string& identifier) const;
    bool isVariableInitialized(const std::string& identifier) const;
    void markAsInitialized(const std::string& identifier);
    ll getMemoryAddress_variable(const std::string& identifier) const;

    bool isArrayDeclared(const std::string& identifier) const;
    ll getMemoryAddress_at(const std::string& identifier, ll index) const;
    ll getMemoryAddress_start(const std::string& identifier) const;
    bool isInsideArray(const std::string& identifier, ll index) const;
    ll get_offset(const std::string& identifier) const;
    
    void addVariable(const std::unique_ptr<Variable>& var);

    void validateIterator(const std::string& identifier) const;
    void addIterator(const std::string& identifier);
    void removeIterator(const std::string& identifier);
    bool isIterator(const std::string& identifier) const;
    void addForLoopBound(const std::string& identifier, const std::string& boundIdentifier);
    ll getMemoryAddress_forLoopBound(const std::string& identifier) const;
    
    void addProcedure(const std::string& identifier, const std::vector<ParameterType>& parameters, const std::vector<ll>& parametersMemoryAddresses, ll returnAddress);
    bool isProcedureDeclared(const std::string& identifier) const;
    const std::vector<ParameterType>& getProcedureParameters(const std::string& identifier) const;
    const std::vector<ll>& getProcedureParametersMemoryAddresses(const std::string& identifier) const;
    ll getProcedureReturnAddress(const std::string& identifier) const;

    void addParameter(const std::string& identifier, ParameterType type);
    ParameterType getParameterType(const std::string& identifier) const;
    ll getMemoryAddressPointer_parameter(const std::string& identifier) const;

    ll getLastMemoryAddress() const;
    
    bool isLocalVariable(const std::string& identifier) const;
    bool isParameter(const std::string& identifier) const;

    void printSymbols() const;

    void setReturnAddress();
    ll getReturnAddress() const;

    std::vector<ll> getOwnParametersMemoryAddresses() const;
    std::string getOwnIdentifier() const;

private:
    std::string procedureIdentifier;

    std::unordered_set<std::string> variables;
    std::unordered_map<std::string, bool> initialized_variables;
    std::unordered_map<std::string, ll> memoryAddresses_variables;

    std::unordered_set<Array_offset> arrays;
    std::unordered_map<Array_offset, std::pair<ll, ll>> memoryAddresses_arrays;

    std::unordered_set<std::string> iterators;
    std::unordered_map<std::string, std::string> forLoopBounds;
    std::unordered_map<std::string, ll> memoryAddresses_forLoopBounds;

    std::unordered_map<std::string, std::vector<ParameterType>> procedures_toCall;
    std::unordered_map<std::string, std::vector<ll>> proceduresParametersMemoryAddresses_toCall;
    std::unordered_map<std::string, ll> proceduresReturnAddresses_toCall;

    std::vector<std::pair<std::string, ParameterType>> parameters;
    std::unordered_map<std::string, ll> memoryAddressesPointers_parameters;

    ll lastMemoryAddress;
    ll returnAddress;

    void __addVariable(const std::unique_ptr<Variable>& var, ll memoryAddress);

};

#endif // SYMBOLS_TABLE_HPP