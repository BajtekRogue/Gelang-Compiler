
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "languageStructs.hpp"
#include "symbolsTable.hpp"
#include "assembling.hpp"
#include <vector>
#include <string>
#include <cinttypes>



// utility.cpp
/**
 * @brief Get the `Value` to `destination` address. Assumes variable is used correctly
 * 
 * @param symbolsTable symbols table for the current scope
 * @param val value to get
 * @param destination address to store the value
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process
 */
std::vector<AssemblyInstruction> getValueToDestinationAddress(SymbolsTable& symbolsTable, const Value& val, int64_t destination);

/**
 * @brief Checks if the variable is used correctly
 * 
 * @param symbolsTable symbols table for the current scope
 * @param identifier identifier to check
 * @param lineNumber line number in case error is thrown
 * @param mustBeInitialized enforces that the variable must be initialized
 * @param arrayAsPointer if the array should be treated as a pointer
 */
void validateUseOfVariable(SymbolsTable& symbolsTable, const Identifier& identifier, int lineNumber, bool mustBeInitialized, bool arrayAsPointer = false);

/**
 * @brief Checks if a instruction is real
 */
bool isRealInstruction(AssemblyInstruction ins);

/**
 * @brief Counts number of real instructions in the given vector of assembly instructions
 * 
 * @param instructions instructions to count
 * @return `ll` number of real instructions
 */
int64_t countRealInstructions(const std::vector<AssemblyInstruction>& instructions);

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
int64_t extractLoopLabel(const std::string& label);

/**
 * @brief Fixes jump addresses when compiling UNTIL loops
 * 
 * @param code code of the loop
 * @param loopSize size of loop code
 * @param conditonSize size of condition code
 */
void fixUntilJump(std::vector<AssemblyInstruction>& code, int64_t loopSize, int64_t conditonSize);

/**
 * @brief Find the next multiple of `1000` bigger than `n`
 */
int64_t findNext1000(int64_t n);

/**
 * @brief Get the address of `Value` to `destination` address. Assumes variable is used correctly
 * 
 * @param symbolsTable symbols table for the current scope
 * @param val value to get
 * @param destination address to store the value
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process 
 */
std::vector<AssemblyInstruction> getAddressToDestinationAddress(SymbolsTable& symbolsTable, const Value& val, int64_t destination);

/**
 * @brief Fixes procedure jump addresses
 * 
 * @param code code of the compiled program, to fix
 * @param proceduresTables all procedures tables (including main)
 */
void fixProcedureCallsJumps(std::vector<AssemblyInstruction>& code);


// arithmetic.cpp
/**
 * @brief Compiles the multiplication operation. Assume that to calculate `x * y` we have `p[Memory::multiplicationLeft] = x` and `p[Memory::multiplicationRight] = y`. The return address of this procedure is `Memory::returnMultiplication`. Time complexity is `O(log(min{|x|, |y|}))`.
 * @return `std::vector<AssemblyInstruction>` procedure code 
 */
std::vector<AssemblyInstruction> generateMultiplication();

/**
 * @brief Compiles the division operation. Assume that to calculate `x / y` we have `p[Memory::divisionLeft] = x` and `p[Memory::divisionRight] = y`. It will return the quotient if `p[Memory::divisionFlag] = 0` and the remainder otherwise. The return address of this procedure is `Memory::returnDivision`. Time complexity is `O(log(|y|))`.
 * @return `std::vector<AssemblyInstruction>` procedure code 
 */
std::vector<AssemblyInstruction> generateDivision();


// optimalization.cpp
/**
 * @brief Detects all `SET x` instructions that will be used more than once or in a loop or in a procedure that is called more than once. Then it initializes them at the beginning of the program and sotres the constants in the memory. Allowed number of constants is `700`.
 * 
 * @param code compiled code to optimize
 * @return `std::vector<AssemblyInstruction>` optimized code 
 */
std::vector<AssemblyInstruction> cacheConstants(std::vector<AssemblyInstruction> code);


#endif // UTILITY_HPP