#include "assembling.hpp"
#include "utility.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <cstdlib>


std::vector<AssemblyInstruction> assembly(std::string filename){

    std::vector<AssemblyInstruction> code;

    std::ifstream file(filename);
    std::string line;
    int lineNumber = 0;

    if (!file.is_open()) {
        std::cerr << Colors::red << "Error: Could not open input file " << filename << Colors::reset << std::endl;
        exit(1);
    }

    while (std::getline(file, line)) {

        // Trim leading whitespaces
        line.erase(0, line.find_first_not_of(" \t"));

        // Skip lines starting with '#'
        if (!line.empty() && line[0] == '#') {
            continue;
        }

        lineNumber++;

        // Tokenize the line
        std::istringstream lineStream(line);
        std::string instruction;
        std::string address;

        lineStream >> instruction >> address;

        if (!instruction.empty()) {
            try{
                code.push_back(AssemblyInstruction(fromString(instruction), (address != "" ? std::stoll(address) : 0)));
            }
            catch (std::invalid_argument& e) {
                std::cout<< instruction << " " << address << std::endl;
                std::cerr << Colors::red << "Error reading the assembly: Address is not a number " << address << Colors::reset << std::endl;
                std::cerr << Colors::yellow << "Line: " << lineNumber << Colors::reset << std::endl;
                exit(1);
            }
            catch (std::domain_error& e) {
                std::cerr << Colors::red << "Error reading the assembly: Instruction " << instruction << " doesn't exist " << Colors::reset << std::endl;
                std::cerr << Colors::yellow << "Line: " << lineNumber << Colors::reset << std::endl;
                exit(1);
            }
        }
    }

    return code;
}


int main(int argc, char* argv[]){

    if(argc != 2) {
        std::cerr << Colors::red << "Usage: " << argv[0] << "<inputFilePath>" << Colors::reset << std::endl;
        return 1;
    }

    std::vector<AssemblyInstruction> code = assembly(argv[1]);

    std::cout << Colors::magenta << "Assembling was successful! Number of instructions: " << Colors::red << code.size() << Colors::reset << std::endl;
    std::cout << Colors::magenta << "Running the program..." << Colors::reset << std::endl;

    std::map<int64_t, int64_t> memory;
    int32_t instructionPointer = 0;
    int64_t totalCost = 0;
    int64_t ioCost = 0;

    // Scan the instructions until we encouter HALT
    while(code[instructionPointer].instruction != Instruction::HALT) {

        Instruction instruction = code[instructionPointer].instruction;
        int64_t address = code[instructionPointer].getAddress();

        // Check if we are refrencing negative memory address
        if (instruction != Instruction::SET && instruction != Instruction::JUMP && instruction != Instruction::JPOS && instruction != Instruction::JZERO && instruction != Instruction::JNEG && address < 0) {
            std::cerr << Colors::red << "Error: Negative memory address" << Colors::reset << std::endl;
            return 1;
        }


        switch(instruction) {
            case Instruction::GET:
                std::cout << "? ";
                std::cin >> memory[address];
                ioCost += 100;
                totalCost += 100;
                instructionPointer++;
                break;
            case Instruction::PUT:
                std::cout << "> " << memory[address] << std::endl;
                ioCost += 100;
                totalCost += 100;
                instructionPointer++;
                break;
            case Instruction::LOAD:
                memory[0] = memory[address];
                totalCost += 10;
                instructionPointer++;
                break;
            case Instruction::STORE:
                memory[address] = memory[0];
                totalCost += 10;
                instructionPointer++;
                break;
            case Instruction::LOADI:
                memory[0] = memory[memory[address]];
                totalCost += 20;
                instructionPointer++;
                break;
            case Instruction::STOREI:
                memory[memory[address]] = memory[0];
                totalCost += 20;
                instructionPointer++;
                break;
            case Instruction::ADD:
                memory[0] += memory[address];
                totalCost += 10;
                instructionPointer++;
                break;
            case Instruction::SUB:
                memory[0] -= memory[address];
                totalCost += 10;
                instructionPointer++;
                break;
            case Instruction::ADDI:
                memory[0] += memory[memory[address]];
                totalCost += 20;
                instructionPointer++;
                break;
            case Instruction::SUBI:
                memory[0] -= memory[memory[address]];
                totalCost += 20;
                instructionPointer++;
                break;
            case Instruction::SET:
                memory[0] = address;
                totalCost += 50;
                instructionPointer++;
                break;
            case Instruction::HALF:
                memory[0] >>= 1;
                totalCost += 5;
                instructionPointer++;
                break;
            case Instruction::JUMP:
                instructionPointer += address;
                totalCost += 1;
                break;
            case Instruction::JPOS:
                instructionPointer += memory[0] > 0 ? address : 1;
                totalCost += 1;
                break;
            case Instruction::JZERO:
                instructionPointer += memory[0] == 0 ? address : 1;
                totalCost += 1;
                break;
            case Instruction::JNEG:
                instructionPointer += memory[0] < 0 ? address : 1;
                totalCost += 1;
                break;
            case Instruction::RTRN:
                instructionPointer = memory[address];
                break;
        }

        // Check if we are out of bounds
        if (instructionPointer < 0 || instructionPointer >= code.size()) {
            std::cerr << Colors::red << "Error: Out of bounds" << Colors::reset << std::endl;
            return 1;
        }
    }

    std::cout.imbue(std::locale(""));
    std::cout << Colors::blue << "Program finished execution. Total cost is: " << Colors::cyan << totalCost << Colors::blue << "; with I/O: " << ioCost << Colors::reset << std::endl;
    return 0;
}