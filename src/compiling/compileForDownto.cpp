#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utlity.hpp"


std::vector<AssemblyInstruction> compileForDownto(SymbolsTable& symbolsTable, const std::unique_ptr<ForDowntoCommand>& cmd){

    std::vector<AssemblyInstruction> code;

    // Generate labels for the FOR loop
    LabelCounters::forCounter++;
    std::string labelStart = getStartLabel(std::to_string(LabelCounters::forCounter));
    std::string labelEnd = getEndLabel(std::to_string(LabelCounters::forCounter));

    // If the loop is degenerate do not compile it but still check if the iterator is valid
    if(cmd->fromValue->isNumber() && cmd->downtoValue->isNumber() && cmd->fromValue->asNumber() < cmd->downtoValue->asNumber()){
        code.push_back(AssemblyInstruction(Instruction::LABEL_FOR_DOWN, labelStart + cmd->bounds()));
        code.push_back(AssemblyInstruction(Instruction::LABEL_ENDFOR, labelEnd));
        symbolsTable.validateIterator(cmd->iterator);
        return code;
    }

    // Check if lower bound is valid
    if(cmd->fromValue->isIdentifier()){
        validateUseOfVariable(symbolsTable, cmd->fromValue->asIdentifier(), "FOR loop", true);
    }
    // Check if upper bound is valid
    if(cmd->downtoValue->isIdentifier()){
        validateUseOfVariable(symbolsTable, cmd->downtoValue->asIdentifier(), "FOR loop", true);
    }

    // Create the iterator variable
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Initializing FOR_DOWN " + labelStart));
    symbolsTable.addIterator(cmd->iterator);
    int64_t iteratorAddress = symbolsTable.getMemoryAddress_variable(cmd->iterator);
    std::vector<AssemblyInstruction> initilizeIteratorCode = getValueToDestinationAddress(symbolsTable, *(cmd->fromValue), iteratorAddress);

    // Make copy of the upper bound in case it changes during the loop
    symbolsTable.addForLoopBound(cmd->iterator, cmd->iterator + "_LIMIT");
    int64_t finalBoundAddress = symbolsTable.getMemoryAddress_forLoopBound(cmd->iterator);
    std::vector<AssemblyInstruction> copyBoundCode = getValueToDestinationAddress(symbolsTable, *(cmd->downtoValue), finalBoundAddress);

    code.insert(code.end(), copyBoundCode.begin(), copyBoundCode.end());
    code.insert(code.end(), initilizeIteratorCode.begin(), initilizeIteratorCode.end());

    // Compile the loop but don't insert yet
    std::vector<AssemblyInstruction> forDowntoCode = compileAll(symbolsTable, cmd->commands);

    // First check the condition. Note that accumulator is already loaded with the iterator value and so will be at the end of the loop
    code.push_back(AssemblyInstruction(Instruction::LABEL_FOR_DOWN, labelStart + cmd->bounds()));
    code.push_back(AssemblyInstruction(Instruction::SUB, finalBoundAddress));
    code.push_back(AssemblyInstruction(Instruction::JNEG, countRealInstructions(forDowntoCode) + 5));
    code.insert(code.end(), forDowntoCode.begin(), forDowntoCode.end());

    // Decrement the iterator and jump back
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, cmd->iterator + "--"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, iteratorAddress));
    code.push_back(AssemblyInstruction(Instruction::SUB, MEMORY_ONE));
    code.push_back(AssemblyInstruction(Instruction::STORE, iteratorAddress));
    code.push_back(AssemblyInstruction(Instruction::JUMP, -countRealInstructions(forDowntoCode) - 5));

    // Remove the iterator variable
    symbolsTable.removeIterator(cmd->iterator);
    
    return code;
}