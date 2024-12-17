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
            auto* ifCmd = dynamic_cast<IfCommand*>(cmd.release());
            std::vector<AssemblyInstruction> thenCode = compile_ALL(symbolsTable, ifCmd->thenCommands);
            ll jumpAddress = thenCode.size() + 1;
            std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> conditionCode = compile_CONDITION(symbolsTable, ifCmd->condition, jumpAddress);

            // Check if condition is known during compile time
            if(conditionCode.second.has_value()){
                if(conditionCode.second.value()){
                    code.insert(code.end(), thenCode.begin(), thenCode.end());
                }
                break;
            }

            // Check the condition and then jump to the end of the if block
            code.insert(code.end(), conditionCode.first.begin(), conditionCode.first.end());
            code.insert(code.end(), thenCode.begin(), thenCode.end());
            break;

        }
        case CommandType::IfElse:{
            auto* ifElseCmd = dynamic_cast<IfElseCommand*>(cmd.release());
            std::vector<AssemblyInstruction> thenCode = compile_ALL(symbolsTable, ifElseCmd->thenCommands);
            std::vector<AssemblyInstruction> elseCode = compile_ALL(symbolsTable, ifElseCmd->elseCommands);
            ll jumpAddress = thenCode.size() + 1;
            ll jumpAddressElse = elseCode.size() + 1;
            std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> conditionCode = compile_CONDITION(symbolsTable, ifElseCmd->condition, jumpAddress + 1);

            // Check if condition is known during compile time
            if(conditionCode.second.has_value()){
                if(conditionCode.second.value()){
                    code.insert(code.end(), thenCode.begin(), thenCode.end());
                }else{
                    code.insert(code.end(), elseCode.begin(), elseCode.end());
                }
                break;
            }

            // Check the condition and then jump to the end of the if block
            code.insert(code.end(), conditionCode.first.begin(), conditionCode.first.end());
            code.insert(code.end(), thenCode.begin(), thenCode.end());
            code.push_back(AssemblyInstruction(AssemblyInstructionType::JUMP, jumpAddressElse));
            code.insert(code.end(), elseCode.begin(), elseCode.end());
            break;
        }
        case CommandType::While:{
            auto* whileCmd = dynamic_cast<WhileCommand*>(cmd.release());
            std::vector<AssemblyInstruction> whileCode = compile_ALL(symbolsTable, whileCmd->commands);
            ll jumpAddress = whileCode.size() + 1;
            std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> conditionCode = compile_CONDITION(symbolsTable, whileCmd->condition, jumpAddress + 1);

            // Get these to check back the condition
            ll jumBackToCond = -whileCode.size() - conditionCode.first.size();

            // Check if condition is known during compile time
            if(conditionCode.second.has_value()){
                if(!conditionCode.second.value()){
                    break;
                }
                std::cout << color_Magenta << "Warning: Infinite WHILE loop detected!" << color_Reset << std::endl;
            }

            // Check the condition and then jump to the end of the while block
            code.insert(code.end(), conditionCode.first.begin(), conditionCode.first.end());
            code.insert(code.end(), whileCode.begin(), whileCode.end());
            code.push_back(AssemblyInstruction(AssemblyInstructionType::JUMP, jumBackToCond));
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