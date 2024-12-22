
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "languageStructs.hpp"
#include "symbolsTable.hpp"
#include "assembling.hpp"
#include <vector>
#include <string>

typedef long long ll;


// utility.cpp
/**
 * @brief Get the `Value` to `destination` address. Assumes variable is used correctly
 * 
 * @param symbolsTable symbols table for the current scope
 * @param val value to get
 * @param destination address to store the value
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process
 */
std::vector<AssemblyInstruction> getValueToDestinationAddress(SymbolsTable& symbolsTable, const Value& val, ll destination);

/**
 * @brief Checks if the variable is used correctly
 * 
 * @param symbolsTable symbols table for the current scope
 * @param identifier identifier to check
 * @param commandName command name to display an error message
 * @param mustBeInitialized enforces that the variable must be initialized
 * @param arrayAsPointer if the array should be treated as a pointer
 */
void validateUseOfVariable(SymbolsTable& symbolsTable, const Identifier& identifier, const std::string commandName, bool mustBeInitialized, bool arrayAsPointer = false);


/**
 * @brief Counts number of real instructions in the given vector of assembly instructions
 * 
 * @param instructions instructions to count
 * @return `ll` number of real instructions
 */
ll countRealInstructions(const std::vector<AssemblyInstruction>& instructions);

/**
 * @brief Get the Start Label object
 * 
 * @param label 
 * @return `std::string` start label 
 */
std::string getStartLabel(std::string label);

/**
 * @brief Get the End Label object
 * 
 * @param label 
 * @return `std::string` end label 
 */
std::string getEndLabel(std::string label);

/**
 * @brief Extracts the loop counter from loop label
 * 
 * @param label the loop label
 * @return `ll` number of the loop
 */
ll extractLoopLabel(const std::string& label);

/**
 * @brief Fixes jump addresses when compiling UNTIL loops
 * 
 * @param code code of the loop
 * @param loopSize size of loop code
 * @param conditonSize size of condition code
 */
void fixUntilJump(std::vector<AssemblyInstruction>& code, ll loopSize, ll conditonSize);

/**
 * @brief Find the next multiple of `1000` bigger than `n`
 */
ll findNext1000(ll n);

/**
 * @brief Get the address of `Value` to `destination` address. Assumes variable is used correctly
 * 
 * @param symbolsTable symbols table for the current scope
 * @param val value to get
 * @param destination address to store the value
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process 
 */
std::vector<AssemblyInstruction> getAddressToDestinationAddress(SymbolsTable& symbolsTable, const Value& val, ll destination);

/**
 * @brief Fixes procedure jump addresses
 * 
 * @param code code of the compiled program, to fix
 * @param proceduresTables all procedures tables (including main)
 */
void fixProcedureCallsJumps(std::vector<AssemblyInstruction>& code, const std::vector<SymbolsTable>& proceduresTables);

#endif // UTILITY_HPP