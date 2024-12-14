#include "languageStructs.hpp"
#include "utility.hpp"

std::vector<AssemblyInstruction> compile(std::unique_ptr<Program>& program) {
    std::vector<AssemblyInstruction> code = {
        AssemblyInstruction(AssemblyInstructionType::SET, 69),
        AssemblyInstruction(AssemblyInstructionType::PUT, 0),
        AssemblyInstruction(AssemblyInstructionType::HALT)
    };
    return code;
}