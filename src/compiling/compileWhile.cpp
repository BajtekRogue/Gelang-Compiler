#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utility.hpp"


std::vector<AssemblyInstruction> compileWhile(SymbolsTable& symbolsTable, const std::unique_ptr<WhileCommand>& cmd){

    std::vector<AssemblyInstruction> code;

    // Generate labels for the while loop
    LabelCounters::whileCounter++;
    std::string labelStart = getStartLabel(std::to_string(LabelCounters::whileCounter));
    std::string labelEnd = getEndLabel(std::to_string(LabelCounters::whileCounter));
    code.push_back(AssemblyInstruction(Instruction::LABEL_WHILE, labelStart + cmd->condition->toString()));

    // Compile the while loop
    std::vector<AssemblyInstruction> whileCode = compileAll(symbolsTable, cmd->commands);
    int64_t jumpAddress = countRealInstructions(whileCode) + 2;
    auto [conditionCode, conditionValue] = compileCondition(symbolsTable, cmd->condition, jumpAddress);

    // Get these to check back the condition
    int64_t jumBackToCond = -countRealInstructions(whileCode) - countRealInstructions(conditionCode);

    // Check if condition is known during compile time
    if(conditionValue.has_value()){
        if(!conditionValue.value()){
            code.push_back(AssemblyInstruction(Instruction::LABEL_ENDWHILE, labelEnd));
            return code;
        }
        std::cout << Colors::magenta << "Warning: Infinite WHILE loop detected!" << Colors::reset << std::endl;
    }

    // Check the condition and then jump to the end of the while block
    code.insert(code.end(), conditionCode.begin(), conditionCode.end());
    code.insert(code.end(), whileCode.begin(), whileCode.end());
    code.push_back(AssemblyInstruction(Instruction::JUMP, jumBackToCond));
    code.push_back(AssemblyInstruction(Instruction::LABEL_ENDWHILE, labelStart));

    return code;
}