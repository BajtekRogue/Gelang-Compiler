#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utlity.hpp"


std::vector<AssemblyInstruction> compileIfElse(SymbolsTable& symbolsTable, const std::unique_ptr<IfElseCommand>& cmd){

    std::vector<AssemblyInstruction> code;

    // Generate labels for the if block
    LabelCounters::ifCounter++;
    std::string labelStart = getStartLabel(std::to_string(LabelCounters::ifCounter));
    std::string labelEnd = getEndLabel(std::to_string(LabelCounters::ifCounter));
    code.push_back(AssemblyInstruction(Instruction::LABEL_IF, labelStart + cmd->condition->toString()));

    // Compile the if block
    std::vector<AssemblyInstruction> thenCode = compileAll(symbolsTable, cmd->thenCommands);
    std::vector<AssemblyInstruction> elseCode = compileAll(symbolsTable, cmd->elseCommands);
    int64_t jumpAddress = countRealInstructions(thenCode) + 2;
    int64_t jumpAddressElse = countRealInstructions(elseCode) + 1;
    auto [conditionCode, conditionValue] = compileCondition(symbolsTable, cmd->condition, jumpAddress);

    // Check if condition is known during compile time
    if(conditionValue.has_value()){
        if(conditionValue.value()){
            code.insert(code.end(), thenCode.begin(), thenCode.end());
            code.push_back(AssemblyInstruction(Instruction::LABEL_ELSE, labelStart));
            code.push_back(AssemblyInstruction(Instruction::LABEL_ENDIF, labelEnd));
            return code;
        }
        code.push_back(AssemblyInstruction(Instruction::LABEL_ELSE, labelStart));
        code.insert(code.end(), elseCode.begin(), elseCode.end());
        code.push_back(AssemblyInstruction(Instruction::LABEL_ENDIF, labelEnd));
        return code;
    }

    // Check the condition and then jump to the end of the if block
    code.insert(code.end(), conditionCode.begin(), conditionCode.end());
    code.insert(code.end(), thenCode.begin(), thenCode.end());
    code.push_back(AssemblyInstruction(Instruction::JUMP, jumpAddressElse));
    code.push_back(AssemblyInstruction(Instruction::LABEL_ELSE, labelStart));
    code.insert(code.end(), elseCode.begin(), elseCode.end());
    code.push_back(AssemblyInstruction(Instruction::LABEL_ENDIF, labelEnd));

    return code;
}