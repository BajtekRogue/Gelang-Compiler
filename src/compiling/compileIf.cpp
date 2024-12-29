#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utility.hpp"
#include "nameSpaces.hpp"

std::vector<AssemblyInstruction> compileIf(SymbolsTable& symbolsTable, const std::unique_ptr<IfCommand>& cmd){

    std::vector<AssemblyInstruction> code;

    // Generate labels for the if block
    LabelCounters::ifCounter++;
    std::string labelStart = getStartLabel(std::to_string(LabelCounters::ifCounter));
    std::string labelEnd = getEndLabel(std::to_string(LabelCounters::ifCounter));
    code.push_back(AssemblyInstruction(Instruction::LABEL_IF, labelStart + cmd->condition->toString()));

    // Compile the if block
    std::vector<AssemblyInstruction> thenCode = compileAll(symbolsTable, cmd->thenCommands);
    int64_t jumpAddress = countRealInstructions(thenCode) + 1;
    auto [conditionCode, conditionValue] = compileCondition(symbolsTable, cmd->condition, jumpAddress, cmd->lineNumber);

    // Check if condition is known during compile time
    if(conditionValue.has_value()){
        if(conditionValue.value()){
            code.insert(code.end(), thenCode.begin(), thenCode.end());
        }
        code.push_back(AssemblyInstruction(Instruction::LABEL_ENDIF, labelEnd));
        return code;
    }

    // Check the condition and then jump to the end of the if block
    code.insert(code.end(), conditionCode.begin(), conditionCode.end());
    code.insert(code.end(), thenCode.begin(), thenCode.end());
    code.push_back(AssemblyInstruction(Instruction::LABEL_ENDIF, labelEnd));
    
    return code;
}