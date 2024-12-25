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
#include <cinttypes>



/**
 * @brief Types of assembly instructions.
 * 
 */
enum class Instruction{

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

static const std::unordered_map<Instruction, std::string> instructionNames = {
    // existing GeAssembly instructions
    {Instruction::GET,    "GET"},
    {Instruction::PUT,    "PUT"},
    {Instruction::LOAD,   "LOAD"},
    {Instruction::STORE,  "STORE"},
    {Instruction::LOADI,  "LOADI"},
    {Instruction::STOREI, "STOREI"},
    {Instruction::ADD,    "ADD"},
    {Instruction::SUB,    "SUB"},
    {Instruction::ADDI,   "ADDI"},
    {Instruction::SUBI,   "SUBI"},
    {Instruction::SET,    "SET"},
    {Instruction::HALF,   "HALF"},
    {Instruction::JUMP,   "JUMP"},
    {Instruction::JPOS,   "JPOS"},
    {Instruction::JZERO,  "JZERO"},
    {Instruction::JNEG,   "JNEG"},
    {Instruction::RTRN,   "RTRN"},
    {Instruction::HALT,   "HALT"},

    // non-existing GeAssembly instructions
    {Instruction::LABEL_IF, "# IF"},
    {Instruction::LABEL_ELSE, "# ELSE"},
    {Instruction::LABEL_ENDIF, "# ENDIF"},
    {Instruction::LABEL_WHILE, "# WHILE"},
    {Instruction::LABEL_ENDWHILE, "# ENDWHILE"},
    {Instruction::LABEL_REPEAT, "# REPEAT"},
    {Instruction::LABEL_UNTIL, "# UNTIL"},
    {Instruction::LABEL_FOR_UP, "# FOR_UP"},
    {Instruction::LABEL_FOR_DOWN, "# FOR_DOWN"},
    {Instruction::LABEL_ENDFOR, "# ENDFOR"},
    {Instruction::LABEL_PROCEDURE, "# PROCEDURE"},
    {Instruction::LABEL_ENDPROCEDURE, "# ENDPROCEDURE"},
    {Instruction::LABEL_MAIN, "# MAIN"},
    {Instruction::LABEL_INSTRUCTION, "#"}
};

/**
 * @brief Represents a single assembly instruction.
 * 
 * This struct models an assembly instruction with a type and an address. 
 * The address can either be a numeric value (for real instructions) or a string 
 * (for labels in the generated code).
 */
struct AssemblyInstruction {
    Instruction instruction;
    std::variant<int64_t, std::string> address; 

    AssemblyInstruction(Instruction t, int64_t addr) : instruction(t), address(addr) {}
    AssemblyInstruction(Instruction t, const std::string& addr) : instruction(t), address(addr) {}

    bool hasAddress() const {
        return std::holds_alternative<int64_t>(address);
    }

    bool hasLabel() const {
        return std::holds_alternative<std::string>(address);
    }

    int64_t getAddress() const {
        return std::get<int64_t>(address);
    }

    std::string getLabel() const {
        return std::get<std::string>(address);
    }

    std::string toString() const {
        auto it = instructionNames.find(instruction);
        if (it == instructionNames.end()) {
            return "UNKNOWN";
        }
        
        std::string result = it->second;
        if (hasAddress()) {
            if (instruction == Instruction::HALT || instruction == Instruction::HALF) {
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


#endif // ASSEMBLING_HPP
