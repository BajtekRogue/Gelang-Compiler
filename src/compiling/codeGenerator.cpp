#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"

std::vector<AssemblyInstruction> compile(std::unique_ptr<Program>& program) {
    std::vector<AssemblyInstruction> code;
    ll mainVariablesMemoryAddress = MEMORY_START;
    SymbolsTable symbolsTable(mainVariablesMemoryAddress);

    for(const auto& decl : program->declarations) {
        symbolsTable.addVariable(decl);
    }

    for(auto& cmd : program->mainCommands) {
        std::vector<AssemblyInstruction> commandCode = compile_COMMAND(symbolsTable, cmd);
        code.insert(code.end(), commandCode.begin(), commandCode.end());
    }

    // Finish the program with HALT instruction
    code.push_back(AssemblyInstruction(AssemblyInstructionType::HALT));
    return code;
}

