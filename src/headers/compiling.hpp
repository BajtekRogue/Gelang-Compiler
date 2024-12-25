
#ifndef COMPILING_HPP
#define COMPILING_HPP

#include "languageStructs.hpp"
#include "symbolsTable.hpp"
#include "assembling.hpp"
#include <vector>
#include <string>
#include <memory>
#include <cinttypes>


// codeGenerator.cpp
/**
 * @brief Compiles the parsed program into GeAssembly code
 * 
 * @param program the parsed program
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process 
 */
std::vector<AssemblyInstruction> compile(std::unique_ptr<Program>& program);


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


// compileIf.cpp
/**
 * @brief Compiles IF command
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cmd command to compile
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process 
 */
std::vector<AssemblyInstruction> compileIf(SymbolsTable& symbolsTable, const std::unique_ptr<IfCommand>& cmd);

// compileIfElse.cpp
/**
 * @brief Compiles IF ELSE command
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cmd command to compile
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process 
 */
std::vector<AssemblyInstruction> compileIfElse(SymbolsTable& symbolsTable, const std::unique_ptr<IfElseCommand>& cmd);


// compileCondition.cpp
/**
 * @brief Compiles a condition and returns the generated assembly code
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cond condition to compile
 * @param jumpAddress address to jump to if the condition is false
 * @return `std::pair<std::vector<AssemblyInstruction>, std::optional<bool>>` instructions generated in the process and the value of the condition if it is known during compile time
 */
std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> compileCondition(SymbolsTable& symbolsTable, const std::unique_ptr<Condition>& cond, int64_t jumpAddress);


// compileWhile.cpp
/**
 * @brief Compile WHILE loop
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cmd command to compile
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process
 */
std::vector<AssemblyInstruction> compileWhile(SymbolsTable& symbolsTable, const std::unique_ptr<WhileCommand>& cmd);

// compileRepeat.cpp
/**
 * @brief Compile REPEAT loop
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cmd command to compile
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process
 */
std::vector<AssemblyInstruction> compileRepeat(SymbolsTable& symbolsTable, const std::unique_ptr<RepeatCommand>& cmd);

// compileForTo.cpp
/**
 * @brief Compile FOR TO loop
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cmd command to compile
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process
 */
std::vector<AssemblyInstruction> compileForTo(SymbolsTable& symbolsTable, const std::unique_ptr<ForToCommand>& cmd);


// compileForDownto.cpp
/**
 * @brief Compile FOR DOWNTO loop
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cmd command to compile
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process
 */
std::vector<AssemblyInstruction> compileForDownto(SymbolsTable& symbolsTable, const std::unique_ptr<ForDowntoCommand>& cmd);


// compileProcedureCall.cpp
/**
 * @brief Compile procedure call
 * 
 * @param symbolsTable symbols table for the current scope
 * @param cmd command to compile
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process
 */
std::vector<AssemblyInstruction> compileProcedureCall(SymbolsTable& symbolsTable, const std::unique_ptr<ProcedureCallCommand>& cmd);


#endif //COMPILING_HPP