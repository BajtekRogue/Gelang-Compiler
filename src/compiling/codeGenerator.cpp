#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"

std::vector<AssemblyInstruction> compile(std::unique_ptr<Program>& program) {
    std::vector<AssemblyInstruction> code;
    ll mainVariablesMemoryAddress = MEMORY_START;
    SymbolsTable symbolsTable(mainVariablesMemoryAddress);

    bool isOnePresent = isThereForLoop(program);

    for(const auto& decl : program->declarations) {
        symbolsTable.addVariable(decl);
    }

    for(auto& cmd : program->mainCommands) {
        std::vector<AssemblyInstruction> commandCode = compileCommand(symbolsTable, cmd);
        code.insert(code.end(), commandCode.begin(), commandCode.end());
    }

    // Otpimalization
    code = initilizedConstants(code, isOnePresent);

    // Finish the program with HALT instruction
    code.push_back(AssemblyInstruction(AssemblyInstructionType::HALT, 0));
    return code;
}

