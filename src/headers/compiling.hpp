
#ifndef COMPILING_HPP
#define COMPILING_HPP

#include "languageStructs.hpp"
#include "symbolsTable.hpp"
#include "assembling.hpp"
#include <vector>
#include <string>
#include <memory>

typedef long long ll;

// compileCommand.cpp
/**
 * @brief Compiles a command and returns the generated assembly code
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cmd command to compile
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process 
 */
std::vector<AssemblyInstruction> compileCommand(SymbolsTable& symbolsTable, std::unique_ptr<Command>& cmd);


/**
 * @brief Helper function to compile a vector of commands
 * 
 * @param symbolsTable symbols table for the current scope
 * @param commands commands to compile
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process 
 */
std::vector<AssemblyInstruction> compileAll(SymbolsTable& symbolsTable, std::vector<std::unique_ptr<Command>>& commands);


// compileRead.cpp
/**
 * @brief Compiles READ command
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cmd command to compile
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process 
 */
std::vector<AssemblyInstruction> compileRead(SymbolsTable& symbolsTable, const std::unique_ptr<ReadCommand>& cmd);


// compileWrite.cpp
/**
 * @brief Compiles WRITE command
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cmd command to compile
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process 
 */
std::vector<AssemblyInstruction> compileWrite(SymbolsTable& symbolsTable, const std::unique_ptr<WriteCommand>& cmd);


// compileAssign.cpp
/**
 * @brief Compiles ASSIGN command
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cmd command to compile
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process 
 */
std::vector<AssemblyInstruction> compileAssign(SymbolsTable& symbolsTable, const std::unique_ptr<AssignCommand>& cmd);


// compileExpression.cpp
/**
 * @brief Compiles an expression and stores the result in the accumulator
 * 
 * @param symbolsTable symbols table for the current scope
 * @param expr given expression
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process 
 */
std::vector<AssemblyInstruction> compileExpression(SymbolsTable& symbolsTable, const std::unique_ptr<Expression>& expr);


// compileCondition.cpp
/**
 * @brief Compiles a condition and returns the generated assembly code
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cond condition to compile
 * @param jumpAddress address to jump to if the condition is false
 * @return `std::pair<std::vector<AssemblyInstruction>, std::optional<bool>>` instructions generated in the process and the value of the condition if it is known during compile time
 */
std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> compileCondition(SymbolsTable& symbolsTable, const std::unique_ptr<Condition>& cond, ll jumpAddress);


// optimalization.cpp
/**
 * @brief Checks if the program contains a FOR loop
 * 
 * @param program program to check
 * @return if the program contains a FOR loop, false otherwise
 */
bool isThereForLoop(std::unique_ptr<Program>& program);

/**
 * @brief Detects all "SET x" instructions that will be used more than once or in a loop. Then it initializes them at the beginning of the program and sotres the constants in the memory. Allowed number of constants is 500.
 * 
 * @param code compiled code to optimize
 * @param isOnePresent if the constant 1 is present in the code due to FOR loops
 * @return `std::vector<AssemblyInstruction>` optimized code 
 */
std::vector<AssemblyInstruction> initilizedConstants(std::vector<AssemblyInstruction> code, bool isOnePresent);

#endif //COMPILING_HPP