#ifndef SYMBOLS_TABLE_HPP
#define SYMBOLS_TABLE_HPP

#include "languageStructs.hpp"
#include <unordered_map>
#include <set>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

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
    SymbolsTable(ll startingMemoryAddres);

    bool isVariableDeclared(const std::string& identifier) const;
    bool isVariableInitialized(const std::string& identifier) const;
    void markAsInitialized(const std::string& identifier);
    ll getMemoryAddress_variable(const std::string& identifier) const;

    bool isArrayDeclared(const std::string& identifier) const;
    bool isArrayInitialized_at(const std::string& identifier, ll index) const;
    void markAsInitialized_at(const std::string& identifier, ll index);
    ll getMemoryAddress_at(const std::string& identifier, ll index) const;
    ll getMemoryAddress_start(const std::string& identifier) const;
    bool isInsideArray(const std::string& identifier, ll index) const;
    ll get_offset(const std::string& identifier) const;
    
    void addVariable(const std::unique_ptr<Variable>& var);
    void printSymbols() const;

private:
    std::set<std::string> variables;
    std::unordered_map<std::string, bool> initialized_variables;
    std::unordered_map<std::string, ll> memoryAddresses_variables;

    std::set<Array_offset> arrays;
    std::unordered_map<Array_offset, std::vector<bool>> initialized_arrays;
    std::unordered_map<Array_offset, std::pair<ll, ll>> memoryAddresses_arrays;

    ll lastMemoryAddress;

    void __addVariable(const std::unique_ptr<Variable>& var, ll memoryAddress);

};

#endif // SYMBOLS_TABLE_HPP