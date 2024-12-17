#ifndef ASSEMBLING_HPP
#define ASSEMBLING_HPP

#include "languageStructs.hpp"
#include "symbolsTable.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <variant>
#include <optional>
#include <unordered_map>
#include <fstream>


typedef long long ll;

const ll MEMORY_START = 1000;
const ll MEMORY_CONST_ASSIGN = 641;
const ll MEMORY_ARRAY_VARIABLE_ASSIGN = 404;
const ll MEMORY_EXPRESSION_RESULT = 314;

enum class AssemblyInstructionType{
    GET,
    PUT,
    LOAD,
    STORE,
    LOADI,
    STOREI,
    ADD,
    SUB,
    ADDI,
    SUBI,
    SET,
    HALF,
    JUMP,
    JPOS,
    JZERO,
    JNEG,
    RTRN,
    HALT
};

static const std::unordered_map<AssemblyInstructionType, std::string> instructionNames = {
        {AssemblyInstructionType::GET,    "GET"},
        {AssemblyInstructionType::PUT,    "PUT"},
        {AssemblyInstructionType::LOAD,   "LOAD"},
        {AssemblyInstructionType::STORE,  "STORE"},
        {AssemblyInstructionType::LOADI,  "LOADI"},
        {AssemblyInstructionType::STOREI, "STOREI"},
        {AssemblyInstructionType::ADD,    "ADD"},
        {AssemblyInstructionType::SUB,    "SUB"},
        {AssemblyInstructionType::ADDI,   "ADDI"},
        {AssemblyInstructionType::SUBI,   "SUBI"},
        {AssemblyInstructionType::SET,    "SET"},
        {AssemblyInstructionType::HALF,   "HALF"},
        {AssemblyInstructionType::JUMP,   "JUMP"},
        {AssemblyInstructionType::JPOS,   "JPOS"},
        {AssemblyInstructionType::JZERO,  "JZERO"},
        {AssemblyInstructionType::JNEG,   "JNEG"},
        {AssemblyInstructionType::RTRN,   "RTRN"},
        {AssemblyInstructionType::HALT,   "HALT"}
};

struct AssemblyInstruction {
    AssemblyInstructionType type;
    std::optional<ll> address;

    AssemblyInstruction(AssemblyInstructionType t) : type(t) {}
    AssemblyInstruction(AssemblyInstructionType t, ll addr) : type(t), address(addr) {}

    std::string print() const {
        auto it = instructionNames.find(type);
        if (it != instructionNames.end()) {
            return it->second + (address ? " " + std::to_string(*address) : "");
        }else{
            return "UNKNOWN";
        }
    }
};


// codeGenerator.cpp
std::vector<AssemblyInstruction> compile(std::unique_ptr<Program>& program);

// utility.cpp
/**
 * @brief Get the Value To Destination Address object. Assumes variable is used correctly
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
 */
void validateUseOfVariable(SymbolsTable& symbolsTable, const Identifier& identifier, const std::string commandName, bool mustBeInitialized);

// compile_COMMAND.cpp
std::vector<AssemblyInstruction> compile_COMMAND(SymbolsTable& symbolsTable, std::unique_ptr<Command>& cmd);
std::vector<AssemblyInstruction> compile_ALL(SymbolsTable& symbolsTable, std::vector<std::unique_ptr<Command>>& commands);

// compile_READ.cpp
std::vector<AssemblyInstruction> compile_READ(SymbolsTable& symbolsTable, const std::unique_ptr<ReadCommand>& cmd);

// compile_WRITE.cpp
std::vector<AssemblyInstruction> compile_WRITE(SymbolsTable& symbolsTable, const std::unique_ptr<WriteCommand>& cmd);

// compile_ASSIGN.cpp
std::vector<AssemblyInstruction> compile_ASSIGN(SymbolsTable& symbolsTable, const std::unique_ptr<AssignCommand>& cmd);

// compile_EXPRESSION.cpp
std::pair<std::vector<AssemblyInstruction>, ll> compile_EXPRESSION(SymbolsTable& symbolsTable, const std::unique_ptr<Expression>& expr);

// compile_CONDITION.cpp
std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> compile_CONDITION(SymbolsTable& symbolsTable, const std::unique_ptr<Condition>& cond, ll jumpAddress);



#endif // ASSEMBLING_HPP
