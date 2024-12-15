#ifndef UTILITY_HPP
#define UTILITY_HPP

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

std::vector<AssemblyInstruction> compile_READ(SymbolsTable& symbolsTable, const std::unique_ptr<ReadCommand>& cmd);

std::vector<AssemblyInstruction> compile_WRITE(SymbolsTable& symbolsTable, const std::unique_ptr<WriteCommand>& cmd);

#endif // UTILITY_HPP
