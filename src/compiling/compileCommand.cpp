#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "colors.hpp"

std::vector<AssemblyInstruction> compileCommand(SymbolsTable& symbolsTable, std::unique_ptr<Command>& cmd) {
    std::vector<AssemblyInstruction> code;

    switch (cmd->type) {
        case CommandType::Read: {
            std::vector<AssemblyInstruction> readCode = compileRead(symbolsTable, 
                    std::move(std::unique_ptr<ReadCommand>(
                        dynamic_cast<ReadCommand*>(cmd.release())
                ))
            );
            code.insert(code.end(), readCode.begin(), readCode.end());
            break;
        }
        case CommandType::Write: {
            std::vector<AssemblyInstruction> writeCode = compileWrite(
                    symbolsTable, 
                    std::move(std::unique_ptr<WriteCommand>(
                        dynamic_cast<WriteCommand*>(cmd.release())
                ))
            );
            code.insert(code.end(), writeCode.begin(), writeCode.end());
            break;
        }
        case CommandType::Assign: {
            std::vector<AssemblyInstruction> assignCode = compileAssign(
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

            std::vector<AssemblyInstruction> thenCode = compileAll(symbolsTable, ifCmd->thenCommands);
            ll jumpAddress = countRealInstructions(thenCode) + 1;
            std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> conditionCode = compileCondition(symbolsTable, ifCmd->condition, jumpAddress);

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

            std::vector<AssemblyInstruction> thenCode = compileAll(symbolsTable, ifElseCmd->thenCommands);
            std::vector<AssemblyInstruction> elseCode = compileAll(symbolsTable, ifElseCmd->elseCommands);
            ll jumpAddress = countRealInstructions(thenCode) + 1;
            ll jumpAddressElse = countRealInstructions(elseCode) + 1;
            std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> conditionCode = compileCondition(symbolsTable, ifElseCmd->condition, jumpAddress + 1);

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

            std::vector<AssemblyInstruction> whileCode = compileAll(symbolsTable, whileCmd->commands);
            ll jumpAddress = countRealInstructions(whileCode) + 1;
            std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> conditionCode = compileCondition(symbolsTable, whileCmd->condition, jumpAddress + 1);

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

            std::vector<AssemblyInstruction> repeatCode = compileAll(symbolsTable, repeatCmd->commands);
            // The loop will be repeated at least once
            code.insert(code.end(), repeatCode.begin(), repeatCode.end());

            ll loopSize = countRealInstructions(repeatCode);
            std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> conditionCode = compileCondition(symbolsTable, repeatCmd->condition, -1);

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
        case CommandType::ForTo:{
            ForToCommand* forToCmd = dynamic_cast<ForToCommand*>(cmd.release());
            LabelCounters::forCounter++;
            std::string labelStart = getStartLabel(std::to_string(LabelCounters::forCounter));
            std::string labelEnd = getEndLabel(std::to_string(LabelCounters::forCounter));

            // If the loop is degenerate do not compile it but still check if the iterator is valid
            if(forToCmd->fromValue->isNumber() && forToCmd->toValue->isNumber() && forToCmd->fromValue->asNumber() > forToCmd->toValue->asNumber()){
                code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_FOR_UP, labelStart + forToCmd->bounds()));
                code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ENDFOR, labelEnd));
                symbolsTable.validateIterator(forToCmd->iterator);
                break;
            }

            // Check if lower bound is valid
            if(forToCmd->fromValue->isIdentifier()){
                validateUseOfVariable(symbolsTable, forToCmd->fromValue->asIdentifier(), "FOR loop", true);
            }
            // Check if upper bound is valid
            if(forToCmd->toValue->isIdentifier()){
                validateUseOfVariable(symbolsTable, forToCmd->toValue->asIdentifier(), "FOR loop", true);
            }

            // Create the iterator variable
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_INSTRUCTION, "Initializing FOR_UP " + labelStart));
            symbolsTable.addIterator(forToCmd->iterator);
            ll iteratorAddress = symbolsTable.getMemoryAddress_variable(forToCmd->iterator);
            std::vector<AssemblyInstruction> initilizeIteratorCode = getValueToDestinationAddress(symbolsTable, *(forToCmd->fromValue), iteratorAddress);
            code.insert(code.end(), initilizeIteratorCode.begin(), initilizeIteratorCode.end());

            // Make copy of the upper bound in case it changes during the loop
            symbolsTable.addForLoopBound(forToCmd->iterator, forToCmd->iterator + "_LIMIT");
            ll finalBoundAddress = symbolsTable.getMemoryAddress_forLoopBound(forToCmd->iterator);
            std::vector<AssemblyInstruction> copyBoundCode = getValueToDestinationAddress(symbolsTable, *(forToCmd->toValue), finalBoundAddress);
            code.insert(code.end(), copyBoundCode.begin(), copyBoundCode.end());

            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_FOR_UP, labelStart + forToCmd->bounds()));
            std::vector<AssemblyInstruction> forToCode = compileAll(symbolsTable, forToCmd->commands);
            code.insert(code.end(), forToCode.begin(), forToCode.end());

            // Increment the iterator
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_INSTRUCTION, forToCmd->iterator + "++"));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, iteratorAddress));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, MEMORY_ONE));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, iteratorAddress));

            // Check the loop condition
            code.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, finalBoundAddress));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::JNEG, -countRealInstructions(forToCode) - 4));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::JZERO, -countRealInstructions(forToCode) - 5));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ENDFOR, labelEnd));

            // Remove the iterator variable
            symbolsTable.removeIterator(forToCmd->iterator);
            break;
        }
        case CommandType::ForDownto:{
            ForDowntoCommand* forDowntoCmd = dynamic_cast<ForDowntoCommand*>(cmd.release());
            LabelCounters::forCounter++;
            std::string labelStart = getStartLabel(std::to_string(LabelCounters::forCounter));
            std::string labelEnd = getEndLabel(std::to_string(LabelCounters::forCounter));

            // If the loop is degenerate do not compile it but still check if the iterator is valid
            if(forDowntoCmd->fromValue->isNumber() && forDowntoCmd->downtoValue->isNumber() && forDowntoCmd->fromValue->asNumber() < forDowntoCmd->downtoValue->asNumber()){
                code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_FOR_DOWN, labelStart + forDowntoCmd->bounds()));
                code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ENDFOR, labelEnd));
                symbolsTable.validateIterator(forDowntoCmd->iterator);
                break;
            }

            // Check if lower bound is valid
            if(forDowntoCmd->fromValue->isIdentifier()){
                validateUseOfVariable(symbolsTable, forDowntoCmd->fromValue->asIdentifier(), "FOR loop", true);
            }
            // Check if upper bound is valid
            if(forDowntoCmd->downtoValue->isIdentifier()){
                validateUseOfVariable(symbolsTable, forDowntoCmd->downtoValue->asIdentifier(), "FOR loop", true);
            }

            // Create the iterator variable
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_INSTRUCTION, "Initializing FOR_DOWN " + labelStart));
            symbolsTable.addIterator(forDowntoCmd->iterator);
            ll iteratorAddress = symbolsTable.getMemoryAddress_variable(forDowntoCmd->iterator);
            std::vector<AssemblyInstruction> initilizeIteratorCode = getValueToDestinationAddress(symbolsTable, *(forDowntoCmd->fromValue), iteratorAddress);
            code.insert(code.end(), initilizeIteratorCode.begin(), initilizeIteratorCode.end());

            // Make copy of the upper bound in case it changes during the loop
            symbolsTable.addForLoopBound(forDowntoCmd->iterator, forDowntoCmd->iterator + "_LIMIT");
            ll finalBoundAddress = symbolsTable.getMemoryAddress_forLoopBound(forDowntoCmd->iterator);
            std::vector<AssemblyInstruction> copyBoundCode = getValueToDestinationAddress(symbolsTable, *(forDowntoCmd->downtoValue), finalBoundAddress);
            code.insert(code.end(), copyBoundCode.begin(), copyBoundCode.end());

            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_FOR_DOWN, labelStart + forDowntoCmd->bounds()));
            std::vector<AssemblyInstruction> forToCode = compileAll(symbolsTable, forDowntoCmd->commands);
            code.insert(code.end(), forToCode.begin(), forToCode.end());

            // Decrement the iterator
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_INSTRUCTION, forDowntoCmd->iterator + "--"));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, iteratorAddress));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, MEMORY_ONE));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, iteratorAddress));

            // Check the loop condition
            code.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, finalBoundAddress));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::JPOS, -countRealInstructions(forToCode) - 4));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::JZERO, -countRealInstructions(forToCode) - 5));
            code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ENDFOR, labelEnd));

            // Remove the iterator variable
            symbolsTable.removeIterator(forDowntoCmd->iterator);
            break;
        }
        default:
            throw std::runtime_error("Non-implemented command type");
    }

    return code;
}


std::vector<AssemblyInstruction> compileAll(SymbolsTable& symbolsTable, std::vector<std::unique_ptr<Command>>& commands){
    std::vector<AssemblyInstruction> code;

    for(auto& cmd : commands){
        std::vector<AssemblyInstruction> commandCode = compileCommand(symbolsTable, cmd);
        code.insert(code.end(), commandCode.begin(), commandCode.end());
    }

    return code;
}