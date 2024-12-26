#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utility.hpp"


std::vector<AssemblyInstruction> cacheConstants(std::vector<AssemblyInstruction> code){

    std::vector<AssemblyInstruction> newCode;
    std::unordered_map<int64_t, int64_t> constantsOccurences;

    // Find all constants that are used more than once or in a loop
    bool loop = false;
    int64_t loopCounter = -1;

    for(size_t i = 0; i < code.size(); i++){

        // Check if we are in a non-degenerated loop
        if(code[i].instruction == Instruction::LABEL_FOR_UP && code[i+1].instruction != Instruction::LABEL_ENDFOR){
            std::string label = code[i].getLabel();
            loop = true;
            loopCounter = extractLoopLabel(label);
        }
        else if(code[i].instruction == Instruction::LABEL_FOR_DOWN && code[i+1].instruction != Instruction::LABEL_ENDFOR){
            std::string label = code[i].getLabel();
            loop = true;
            loopCounter = extractLoopLabel(label);
        }
        else if(code[i].instruction == Instruction::LABEL_WHILE && code[i+1].instruction != Instruction::LABEL_ENDWHILE){
            std::string label = code[i].getLabel();
            loop = true;
            loopCounter = extractLoopLabel(label);
        }
        else if(code[i].instruction == Instruction::LABEL_REPEAT && code[i+1].instruction != Instruction::LABEL_UNTIL){
            std::string label = code[i].getLabel();
            loop = true;
            loopCounter = extractLoopLabel(label);
        }

        // Control when we exit the loop
        if(code[i].instruction == Instruction::LABEL_ENDFOR){
            std::string label = code[i].getLabel();
            if(loopCounter == extractLoopLabel(label)){
                loopCounter = -1;
                loop = false;
            }
        }
        else if(code[i].instruction == Instruction::LABEL_ENDWHILE){
            std::string label = code[i].getLabel();
            if(loopCounter == extractLoopLabel(label)){
                loopCounter = -1;
                loop = false;
            }
        }
        else if(code[i].instruction == Instruction::LABEL_UNTIL){
            std::string label = code[i].getLabel();
            if(loopCounter == extractLoopLabel(label)){
                loopCounter = -1;
                loop = false;
            }
        }


        if(code[i].instruction == Instruction::SET && code[i].hasAddress()){
            int64_t constant = code[i].getAddress();
            

            if(constantsOccurences.find(constant) == constantsOccurences.end()){
                // If it is in a loop it is worth storing it before
                constantsOccurences[constant] = (loop ? 2 : 1);
            }
            else{
                constantsOccurences[constant]++;
            }
        }
    }

    // Find all occurences of constants in procedures that are called more than once
    std::unordered_map<std::string, int64_t> proceduresOccurences;

    for(const auto& instruction : code){
        // Check if the instruction label is a procedure call
        if (instruction.instruction == Instruction::LABEL_INSTRUCTION && instruction.hasLabel()) {
            std::string label = instruction.getLabel();
            std::string prefix = "CALL ";
            std::string suffix = " (";

            if (label.substr(0, prefix.size()) == prefix && label.find(suffix) != std::string::npos) {
                std::string procedureName = label.substr(prefix.size(), label.find(suffix) - prefix.size());
                if(proceduresOccurences.find(procedureName) == proceduresOccurences.end()){
                    proceduresOccurences[procedureName] = 1;
                }
                else{
                    proceduresOccurences[procedureName]++;
                }
            }
        }
    }

    // Add to occurences all constants that appear in procedures that are called more than once
    bool mulitCallProcedure = false;

    for(const auto& instruction : code){

        // Check if we are in a procedure that is called more than once
        if(instruction.instruction == Instruction::LABEL_PROCEDURE && instruction.hasLabel()){
            std::string procedureName = instruction.getLabel();

            if(proceduresOccurences[procedureName] > 1){
                mulitCallProcedure = true;
            }
        }

        // Control when we exit the procedure
        if(instruction.instruction == Instruction::LABEL_ENDPROCEDURE){
            mulitCallProcedure = false;
        }

        if(instruction.instruction == Instruction::SET && instruction.hasAddress() && mulitCallProcedure){
            int64_t constant = instruction.getAddress();

            // If the constant is in a procedure that is called more than once, mark it
            if(constantsOccurences.find(constant) == constantsOccurences.end()){
                constantsOccurences[constant] = 2;
            }
            else{
                constantsOccurences[constant]++;
            }
        }
    }

    // If 1 must be initialized, mark it
    if(Arithmetic::isOneNeeded){
        constantsOccurences[1] = 2;
    }

    // Load constants
    int64_t memoryAddress = Memory::constantsStart + 1;
    std::unordered_map<int64_t, int64_t> constantsMemoryAddresses;
    bool isPreprocessing = false;

    for(const auto& [constant, occurence] : constantsOccurences){
        if(occurence > 1){
            int64_t destination = (Arithmetic::isOneNeeded && constant == 1 ? Memory::one : memoryAddress);
            newCode.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Store const = " + std::to_string(constant) + " at p[" + std::to_string(destination) + "]"));
            newCode.push_back(AssemblyInstruction(Instruction::SET, constant));
            newCode.push_back(AssemblyInstruction(Instruction::STORE, destination));
            constantsMemoryAddresses[constant] = destination;
            memoryAddress++;
            isPreprocessing = true;
        }
    }

    // Add preprocessing label
    if(isPreprocessing){
        newCode.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Finished preprocessing\n"));
    }

    // Replace constants with memory addresses
    for(size_t j = 0; j < code.size(); j++){

        // If it is not SET, do nothing
        if(code[j].instruction != Instruction::SET){
            newCode.push_back(code[j]);
            continue;
        }

        // If it is a SET of fixing return to procedure DO NOT TOUCH IT
        if(code[j].instruction == Instruction::SET && j > 0 && code[j-1].hasLabel() && code[j-1].getLabel().find("Setting return address and jumping to ") != std::string::npos){
            newCode.push_back(code[j]);
            continue;
        }

        // If the number in SET occurs more than once load it with memory address
        int64_t constant = code[j].getAddress();
        if(constantsOccurences[constant] > 1){
            newCode.push_back(AssemblyInstruction(Instruction::LOAD, constantsMemoryAddresses[constant]));
        }
        else{
            newCode.push_back(code[j]);
        }
    }

    // Add preprocessing label
    if(isPreprocessing){
        newCode.insert(newCode.begin(), AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Preprocessing..."));
    }

    return newCode;
}