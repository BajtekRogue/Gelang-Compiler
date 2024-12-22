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
const ll MEMORY_ARRAY_VARIABLE_ASSIGN = 28;
const ll MEMORY_ONE = 69;
const ll MEMORY_CONSTANTS = 500;


// utility.cpp
// counter to display in the labels
namespace LabelCounters {
    extern ll ifCounter;
    extern ll whileCounter;
    extern ll repeatCounter;
    extern ll forCounter;
    extern ll procedureCounter;
};


enum class AssemblyInstructionType{

    // These instruction exist in the GeAssembly
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
    HALT,

    // These instructions are used to help with the compilation but do not exist in the GeAssembly
    LABEL_IF,
    LABEL_ELSE,
    LABEL_ENDIF,
    LABEL_WHILE,
    LABEL_ENDWHILE,
    LABEL_REPEAT,
    LABEL_UNTIL,
    LABEL_FOR_UP,
    LABEL_FOR_DOWN,
    LABEL_ENDFOR,
    LABEL_PROCEDURE,
    LABEL_ENDPROCEDURE,
    LABEL_MAIN,
    LABEL_INSTRUCTION
};

static const std::unordered_map<AssemblyInstructionType, std::string> instructionNames = {
        // existing GeAssembly instructions
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
        {AssemblyInstructionType::HALT,   "HALT"},

        // non-existing GeAssembly instructions
        {AssemblyInstructionType::LABEL_IF, "# IF"},
        {AssemblyInstructionType::LABEL_ELSE, "# ELSE"},
        {AssemblyInstructionType::LABEL_ENDIF, "# ENDIF"},
        {AssemblyInstructionType::LABEL_WHILE, "# WHILE"},
        {AssemblyInstructionType::LABEL_ENDWHILE, "# ENDWHILE"},
        {AssemblyInstructionType::LABEL_REPEAT, "# REPEAT"},
        {AssemblyInstructionType::LABEL_UNTIL, "# UNTIL"},
        {AssemblyInstructionType::LABEL_FOR_UP, "# FOR_UP"},
        {AssemblyInstructionType::LABEL_FOR_DOWN, "# FOR_DOWN"},
        {AssemblyInstructionType::LABEL_ENDFOR, "# ENDFOR"},
        {AssemblyInstructionType::LABEL_PROCEDURE, "# PROCEDURE"},
        {AssemblyInstructionType::LABEL_ENDPROCEDURE, "# ENDPROCEDURE"},
        {AssemblyInstructionType::LABEL_MAIN, "# MAIN"},
        {AssemblyInstructionType::LABEL_INSTRUCTION, "#"}
};

/**
 * @brief Represents a single assembly instruction.
 * 
 * This struct models an assembly instruction with a type and an address. 
 * The address can either be a numeric value (for real instructions) or a string 
 * (for labels in the generated code).
 */
struct AssemblyInstruction {
    AssemblyInstructionType type;
    std::variant<ll, std::string> address; 

    AssemblyInstruction(AssemblyInstructionType t, ll addr) : type(t), address(addr) {}
    AssemblyInstruction(AssemblyInstructionType t, const std::string& addr) : type(t), address(addr) {}

    bool hasAddress() const {
        return std::holds_alternative<ll>(address);
    }

    bool hasLabel() const {
        return std::holds_alternative<std::string>(address);
    }

    ll getAddress() const {
        return std::get<ll>(address);
    }

    std::string getLabel() const {
        return std::get<std::string>(address);
    }

    std::string toString() const {
        auto it = instructionNames.find(type);
        if (it == instructionNames.end()) {
            return "UNKNOWN";
        }
        
        std::string result = it->second;
        if (hasAddress()) {
            if (type == AssemblyInstructionType::HALT || type == AssemblyInstructionType::HALF) {
                return result;
            }else{
                result += " " + std::to_string(getAddress());
            }
        } else {
            result += " " + getLabel();
        }
        return result;
    }

};


// codeGenerator.cpp
/**
 * @brief Compiles the parsed program into GeAssembly code
 * 
 * @param program the parsed program
 * @return `std::vector<AssemblyInstruction>` instructions generated in the process 
 */
std::vector<AssemblyInstruction> compile(std::unique_ptr<Program>& program);


#endif // ASSEMBLING_HPP
