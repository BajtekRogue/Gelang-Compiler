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


#endif // ASSEMBLING_HPP
