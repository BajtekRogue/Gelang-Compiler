#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utlity.hpp"


bool isThereForLoop(std::unique_ptr<Program>& program){
    std::string s = program.get()->toString();
    return s.find("FOR") != std::string::npos;
}

std::vector<AssemblyInstruction> initilizedConstants(std::vector<AssemblyInstruction> code, bool isOnePresent){
    std::vector<AssemblyInstruction> newCode;

    std::unordered_map<ll, ll> constantsOccurences;
    bool loop = false;
    ll loopCounter = -1;

    for(size_t i = 0; i < code.size(); i++){

        // Check if we are in a non-degenerated loop
        if(code[i].type == AssemblyInstructionType::LABEL_FOR_UP && code[i+1].type != AssemblyInstructionType::LABEL_ENDFOR){
            std::string label = code[i].getLabel();
            loop = true;
            loopCounter = extractLoopLabel(label);
        }
        else if(code[i].type == AssemblyInstructionType::LABEL_FOR_DOWN && code[i+1].type != AssemblyInstructionType::LABEL_ENDFOR){
            std::string label = code[i].getLabel();
            loop = true;
            loopCounter = extractLoopLabel(label);
        }
        else if(code[i].type == AssemblyInstructionType::LABEL_WHILE && code[i+1].type != AssemblyInstructionType::LABEL_ENDWHILE){
            std::string label = code[i].getLabel();
            loop = true;
            loopCounter = extractLoopLabel(label);
        }
        else if(code[i].type == AssemblyInstructionType::LABEL_REPEAT && code[i+1].type != AssemblyInstructionType::LABEL_UNTIL){
            std::string label = code[i].getLabel();
            loop = true;
            loopCounter = extractLoopLabel(label);
        }

        // Control when we exit the loop
        if(code[i].type == AssemblyInstructionType::LABEL_ENDFOR){
            std::string label = code[i].getLabel();
            if(loopCounter == extractLoopLabel(label)){
                loopCounter = -1;
                loop = false;
            }
        }
        else if(code[i].type == AssemblyInstructionType::LABEL_ENDWHILE){
            std::string label = code[i].getLabel();
            if(loopCounter == extractLoopLabel(label)){
                loopCounter = -1;
                loop = false;
            }
        }
        else if(code[i].type == AssemblyInstructionType::LABEL_UNTIL){
            std::string label = code[i].getLabel();
            if(loopCounter == extractLoopLabel(label)){
                loopCounter = -1;
                loop = false;
            }
        }


        if(code[i].type == AssemblyInstructionType::SET){
            ll constant = code[i].getAddress();
            
            if(constantsOccurences.find(constant) == constantsOccurences.end()){
                // If it is in a loop it is worth storing it before
                constantsOccurences[constant] = (loop ? 2 : 1);
            }else{
                constantsOccurences[constant]++;
            }
        }
    }

    // If 1 must be initialized, mark it
    if(isOnePresent){
        constantsOccurences[1] = 2;
    }

    // Load constants
    ll memoryAddress = MEMORY_CONSTANTS + 1;
    std::unordered_map<ll, ll> constantsMemoryAddresses;
    bool isPreprocessing = false;

    for(const auto& [constant, occurence] : constantsOccurences){
        if(occurence > 1){
            ll destination = (isOnePresent && constant == 1 ? MEMORY_ONE : memoryAddress);
            newCode.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_INSTRUCTION, "Store const = " + std::to_string(constant) + " at p[" + std::to_string(destination) + "]"));
            newCode.push_back(AssemblyInstruction(AssemblyInstructionType::SET, constant));
            newCode.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, destination));
            constantsMemoryAddresses[constant] = destination;
            memoryAddress++;
            isPreprocessing = true;
        }
    }

    if(isPreprocessing){
        newCode.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_INSTRUCTION, "Finished preprocessing\n"));
    }

    // Replace constants with memory addresses
    for(auto& instruction : code){

        // If it is not SET, do nothing
        if(instruction.type != AssemblyInstructionType::SET){
            newCode.push_back(instruction);
            continue;
        }

        // If the number in SET occurs more than once load it with memory address
        ll constant = instruction.getAddress();
        if(constantsOccurences[constant] > 1){
            newCode.push_back(AssemblyInstruction(AssemblyInstructionType::LOAD, constantsMemoryAddresses[constant]));
        }else{
            newCode.push_back(instruction);
        }
    }

    newCode.insert(newCode.begin(), AssemblyInstruction(AssemblyInstructionType::LABEL_INSTRUCTION, "Preprocessing..."));
    return newCode;
}