
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "languageStructs.hpp"
#include "symbolsTable.hpp"
#include "assembling.hpp"
#include <vector>
#include <string>
#include <cinttypes>

const int64_t MEMORY_START = 1000;
const int64_t MEMORY_ARRAY_VARIABLE_ASSIGN = 28;
const int64_t MEMORY_ONE = 69;
const int64_t MEMORY_CONSTANTS = 300;
const int64_t MEMORY_RETURN_MULTIPLICATION = 56;
const int64_t MEMORY_RETURN_DIVISION = 47;
const int64_t MEMORY_RETURN_MODULO = 35;


// counter to display in the labels
namespace LabelCounters {
    inline int64_t ifCounter = 0;
    inline int64_t whileCounter = 0;
    inline int64_t repeatCounter = 0;
    inline int64_t forCounter = 0;
    inline int64_t procedureCounter = 0;
};

namespace Arithmetic {
    inline bool multiplication = false;
    inline bool division = false;
    inline bool modulo = false;
}

namespace Colors {
    inline const std::string reset   = "\033[1;0m";
    inline const std::string black   = "\033[1;30m";
    inline const std::string red     = "\033[1;31m";
    inline const std::string green   = "\033[1;32m";
    inline const std::string yellow  = "\033[1;33m";
    inline const std::string blue    = "\033[1;34m";
    inline const std::string magenta = "\033[1;35m";
    inline const std::string cyan    = "\033[1;36m";
    inline const std::string white   = "\033[1;37m";
}


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
 * @param commandName command name to display an error message
 * @param mustBeInitialized enforces that the variable must be initialized
 * @param arrayAsPointer if the array should be treated as a pointer
 */
void validateUseOfVariable(SymbolsTable& symbolsTable, const Identifier& identifier, const std::string commandName, bool mustBeInitialized, bool arrayAsPointer = false);

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
 * @brief Compiles the multiplication operation. Assume that to calculate `x * y` we have `p[1] = x` and `p[2] = y`. The return address of this procedure is `MEMORY_RETURN_MULTIPLICATION`. Time complexity is `O(log(min{|x|, |y|}))`.
 * @return `std::vector<AssemblyInstruction>` procedure code 
 */
std::vector<AssemblyInstruction> generateMultiplication();

/**
 * @brief Compiles the division operation. Assume that to calculate `x / y` the value of `x` is in the accumulator and the value of `y` is in the memory at address `p[1]`. The return address of this procedure is `MEMORY_RETURN_DIVISION`.
 * @return `std::vector<AssemblyInstruction>` procedure code 
 */
std::vector<AssemblyInstruction> generateDivision();

/**
 * @brief Compiles the multiplication operation. Assume that to calculate `x % y` the value of `x` is in the accumulator and the value of `y` is in the memory at address `p[1]`. The return address of this procedure is `MEMORY_RETURN_MODULO`.
 * @return `std::vector<AssemblyInstruction>` procedure code 
 */
std::vector<AssemblyInstruction> generateModulo();


// optimalization.cpp
/**
 * @brief Detects all `SET x` instructions that will be used more than once or in a loop or in a procedure that is called more than once. Then it initializes them at the beginning of the program and sotres the constants in the memory. Allowed number of constants is `700`.
 * 
 * @param code compiled code to optimize
 * @param isOnePresent if the constant `1` is present in the code due to `FOR` loops
 * @return `std::vector<AssemblyInstruction>` optimized code 
 */
std::vector<AssemblyInstruction> cacheConstants(std::vector<AssemblyInstruction> code, bool isOnePresent);


#endif // UTILITY_HPP