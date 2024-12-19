#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "colors.hpp"

std::vector<AssemblyInstruction> compile_COMMAND(SymbolsTable& symbolsTable, std::unique_ptr<Command>& cmd) {
    std::vector<AssemblyInstruction> code;

    switch (cmd->type) {
        case CommandType::Read: {
            std::vector<AssemblyInstruction> readCode = compile_READ(symbolsTable, 
                    std::move(std::unique_ptr<ReadCommand>(
                        dynamic_cast<ReadCommand*>(cmd.release())
                ))
            );
            code.insert(code.end(), readCode.begin(), readCode.end());
            break;
        }
        case CommandType::Write: {
            std::vector<AssemblyInstruction> writeCode = compile_WRITE(
                    symbolsTable, 
                    std::move(std::unique_ptr<WriteCommand>(
                        dynamic_cast<WriteCommand*>(cmd.release())
                ))
            );
            code.insert(code.end(), writeCode.begin(), writeCode.end());
            break;
        }
        case CommandType::Assign: {
            std::vector<AssemblyInstruction> assignCode = compile_ASSIGN(
                    symbolsTable, 
                    std::move(std::unique_ptr<AssignCommand>(
                        dynamic_cast<AssignCommand*>(cmd.release())
                ))
            );
            code.insert(code.end(), assignCode.begin(), assignCode.end());
            break;
        }
        case CommandType::If:{
            IfCommand* ifCmd = dynamic_cast<IfCommand*>(cmd.release());
            LabelCounters::ifCounter++;
            std::string labelStart = getStartLabel(std::to_string(LabelCounters::ifCounter));
            std::string labelEnd = getEndLabel(std::to_string(LabelCounters::ifCounter));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_IF, labelStart + ifCmd->condition->toString()));

            std::vector<AssemblyInstruction> thenCode = compile_ALL(symbolsTable, ifCmd->thenCommands);
            ll jumpAddress = countRealInstructions(thenCode) + 1;
            std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> conditionCode = compile_CONDITION(symbolsTable, ifCmd->condition, jumpAddress);

            // Check if condition is known during compile time
            if(conditionCode.second.has_value()){
                if(conditionCode.second.value()){
                    code.insert(code.end(), thenCode.begin(), thenCode.end());
                    code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ENDIF, labelEnd));
                }
                break;
            }

            // Check the condition and then jump to the end of the if block
            code.insert(code.end(), conditionCode.first.begin(), conditionCode.first.end());
            code.insert(code.end(), thenCode.begin(), thenCode.end());

            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ENDIF, labelEnd));
            break;
        }
        case CommandType::IfElse:{
            IfElseCommand* ifElseCmd = dynamic_cast<IfElseCommand*>(cmd.release());
            LabelCounters::ifCounter++;
            std::string labelStart = getStartLabel(std::to_string(LabelCounters::ifCounter));
            std::string labelEnd = getEndLabel(std::to_string(LabelCounters::ifCounter));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_IF, labelStart + ifElseCmd->condition->toString()));

            std::vector<AssemblyInstruction> thenCode = compile_ALL(symbolsTable, ifElseCmd->thenCommands);
            std::vector<AssemblyInstruction> elseCode = compile_ALL(symbolsTable, ifElseCmd->elseCommands);
            ll jumpAddress = countRealInstructions(thenCode) + 1;
            ll jumpAddressElse = countRealInstructions(elseCode) + 1;
            std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> conditionCode = compile_CONDITION(symbolsTable, ifElseCmd->condition, jumpAddress + 1);

            // Check if condition is known during compile time
            if(conditionCode.second.has_value()){
                if(conditionCode.second.value()){
                    code.insert(code.end(), thenCode.begin(), thenCode.end());
                    code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ELSE, labelStart));
                    code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ENDIF, labelEnd));
                        
                }else{
                    code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ELSE, labelStart));
                    code.insert(code.end(), elseCode.begin(), elseCode.end());
                    code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ENDIF, labelEnd));
                }
                break;
            }

            // Check the condition and then jump to the end of the if block
            code.insert(code.end(), conditionCode.first.begin(), conditionCode.first.end());
            code.insert(code.end(), thenCode.begin(), thenCode.end());

            code.push_back(AssemblyInstruction(AssemblyInstructionType::JUMP, jumpAddressElse));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ELSE, labelStart));

            code.insert(code.end(), elseCode.begin(), elseCode.end());
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ENDIF, labelEnd));
            break;
        }
        case CommandType::While:{
            WhileCommand* whileCmd = dynamic_cast<WhileCommand*>(cmd.release());
            LabelCounters::whileCounter++;
            std::string labelStart = getStartLabel(std::to_string(LabelCounters::whileCounter));
            std::string labelEnd = getEndLabel(std::to_string(LabelCounters::whileCounter));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_WHILE, labelStart + whileCmd->condition->toString()));

            std::vector<AssemblyInstruction> whileCode = compile_ALL(symbolsTable, whileCmd->commands);
            ll jumpAddress = countRealInstructions(whileCode) + 1;
            std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> conditionCode = compile_CONDITION(symbolsTable, whileCmd->condition, jumpAddress + 1);

            // Get these to check back the condition
            ll jumBackToCond = -countRealInstructions(whileCode) - countRealInstructions(conditionCode.first);

            // Check if condition is known during compile time
            if(conditionCode.second.has_value()){
                if(!conditionCode.second.value()){
                    code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ENDWHILE, labelEnd));
                    break;
                }
                std::cout << color_Magenta << "Warning: Infinite WHILE loop detected!" << color_Reset << std::endl;
            }

            // Check the condition and then jump to the end of the while block
            code.insert(code.end(), conditionCode.first.begin(), conditionCode.first.end());
            code.insert(code.end(), whileCode.begin(), whileCode.end());

            code.push_back(AssemblyInstruction(AssemblyInstructionType::JUMP, jumBackToCond));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ENDWHILE, labelStart));
            break;
        }
        case CommandType::Repeat:{
            RepeatCommand* repeatCmd = dynamic_cast<RepeatCommand*>(cmd.release());
            LabelCounters::repeatCounter++;
            std::string labelStart = getStartLabel(std::to_string(LabelCounters::repeatCounter));
            std::string labelEnd = getEndLabel(std::to_string(LabelCounters::repeatCounter));
            std::string originalCondition = repeatCmd->condition->toString();
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_REPEAT, labelStart));

            std::vector<AssemblyInstruction> repeatCode = compile_ALL(symbolsTable, repeatCmd->commands);
            // The loop will be repeated at least once
            code.insert(code.end(), repeatCode.begin(), repeatCode.end());

            ll loopSize = countRealInstructions(repeatCode);
            std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> conditionCode = compile_CONDITION(symbolsTable, repeatCmd->condition, -1);

            // Check if condition is known during compile time
            bool finiteLoop = true;
            if(conditionCode.second.has_value()){
                if(conditionCode.second.value()){
                    code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_UNTIL, labelEnd + originalCondition));
                    break;
                }
                finiteLoop = false;
                std::cout << color_Magenta << "Warning: Infinite UNTIL loop detected!" << color_Reset << std::endl;
            }

            // Check the condition and then jump to the end of the while block
            ll conditonSize = countRealInstructions(conditionCode.first);
            code.insert(code.end(), conditionCode.first.begin(), conditionCode.first.end());

            // Fix the jump addresses
            if(finiteLoop){
                fixUntilJump(code, loopSize, conditonSize);
            }else{
                code.push_back(AssemblyInstruction(AssemblyInstructionType::JUMP, -loopSize));
            }
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_UNTIL, labelStart + originalCondition));
            break;
        }
        default:
            throw std::runtime_error("Non-implemented command type");
    }

    return code;
}


std::vector<AssemblyInstruction> compile_ALL(SymbolsTable& symbolsTable, std::vector<std::unique_ptr<Command>>& commands){
    std::vector<AssemblyInstruction> code;

    for(auto& cmd : commands){
        std::vector<AssemblyInstruction> commandCode = compile_COMMAND(symbolsTable, cmd);
        code.insert(code.end(), commandCode.begin(), commandCode.end());
    }

    return code;
}