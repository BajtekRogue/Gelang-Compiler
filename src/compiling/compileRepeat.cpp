#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utlity.hpp"
#include "colors.hpp"


std::vector<AssemblyInstruction> compileRepeat(SymbolsTable& symbolsTable, const std::unique_ptr<RepeatCommand>& cmd){

    std::vector<AssemblyInstruction> code;

    // Generate labels for the loop
    LabelCounters::repeatCounter++;
    std::string labelStart = getStartLabel(std::to_string(LabelCounters::repeatCounter));
    std::string labelEnd = getEndLabel(std::to_string(LabelCounters::repeatCounter));
    std::string originalCondition = cmd->condition->toString();
    code.push_back(AssemblyInstruction(Instruction::LABEL_REPEAT, labelStart));

    // Compile the loop
    std::vector<AssemblyInstruction> repeatCode = compileAll(symbolsTable, cmd->commands);
    int64_t loopSize = countRealInstructions(repeatCode);
    auto [conditionCode, conditionValue] = compileCondition(symbolsTable, cmd->condition, -1);

    // The loop will be repeated at least once
    code.insert(code.end(), repeatCode.begin(), repeatCode.end());

    // Check if condition is known during compile time
    bool finiteLoop = true;
    if(conditionValue.has_value()){
        if(conditionValue.value()){
            code.push_back(AssemblyInstruction(Instruction::LABEL_UNTIL, labelEnd + originalCondition));
            return code;
        }
        finiteLoop = false;
        std::cout << color_Magenta << "Warning: Infinite UNTIL loop detected!" << color_Reset << std::endl;
    }

    // Check the condition and then jump to the end of the while block
    int64_t conditonSize = countRealInstructions(conditionCode);
    code.insert(code.end(), conditionCode.begin(), conditionCode.end());

    // Fix the jump addresses
    if(finiteLoop){
        fixUntilJump(code, loopSize, conditonSize);
    }else{
        code.push_back(AssemblyInstruction(Instruction::JUMP, -loopSize));
    }
    code.push_back(AssemblyInstruction(Instruction::LABEL_UNTIL, labelStart + originalCondition));

    return code;
}