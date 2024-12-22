#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utlity.hpp"

std::vector<AssemblyInstruction> compile(std::unique_ptr<Program>& program) {
    std::vector<AssemblyInstruction> code;

    bool isOnePresent = isThereForLoop(program);

    // Check if there are no two procedures of the same name
    std::unordered_set<std::string> procedureNames;
    for(const auto& procedure : program->procedures) {
        if(procedureNames.find(procedure->identifier) != procedureNames.end()) {
            throw std::logic_error("Procedure `" + procedure->identifier + "` was already declared");
        }
        procedureNames.insert(procedure->identifier);
    }

    std::vector<SymbolsTable> proceduresTables(program->procedures.size());

    // Compile the procedures
    for(size_t i = 0; i < program->procedures.size(); i++) {

        ll nextFreeAddress = (i == 0 ? 100 : proceduresTables[i-1].getLastMemoryAddress() + 1);

        proceduresTables[i] = SymbolsTable(program->procedures[i]->identifier, findNext1000(nextFreeAddress));
        nextFreeAddress = proceduresTables[i].getLastMemoryAddress() + 1;

        for(const auto& param: program->procedures[i]->parameters) {
            proceduresTables[i].addParameter(param->identifier, param->type);
        }

        for(const auto& decl : program->procedures[i]->declarations) {
            proceduresTables[i].addVariable(decl);
        }

        // Add above declared procedures to be available
        for(size_t j = 0; j < i; j++) {
            proceduresTables[i].addProcedure(program->procedures[j]->identifier, program->procedures[j]->getParameterTypes(), proceduresTables[j].getOwnParametersMemoryAddresses(), proceduresTables[j].getReturnAddress());
        }

        code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_PROCEDURE, program->procedures[i]->identifier));

        for(auto& cmd : program->procedures[i]->commands) {
            std::vector<AssemblyInstruction> commandCode = compileCommand(proceduresTables[i], cmd);
            code.insert(code.end(), commandCode.begin(), commandCode.end());
        }

        proceduresTables[i].setReturnAddress();
        code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_INSTRUCTION, "RETURN " + program->procedures[i]->identifier));
        code.push_back(AssemblyInstruction(AssemblyInstructionType::RTRN, proceduresTables[i].getReturnAddress()));
        code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_ENDPROCEDURE, program->procedures[i]->identifier + "\n"));
    }

    // Jump to the main procedure
    std::vector<AssemblyInstruction> jumpToMainCode;
    jumpToMainCode.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_INSTRUCTION, "Jump to main procedure"));
    jumpToMainCode.push_back(AssemblyInstruction(AssemblyInstructionType::JUMP, countRealInstructions(code) + 1));
    jumpToMainCode.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_INSTRUCTION, "\n"));
    code.insert(code.begin(), jumpToMainCode.begin(), jumpToMainCode.end());

    ll mainVariablesMemoryAddress = (proceduresTables.empty() ? MEMORY_START : findNext1000(proceduresTables.back().getLastMemoryAddress() + 1));
    SymbolsTable mainTable("MAIN", mainVariablesMemoryAddress);

    code.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_MAIN, ""));

    for(const auto& decl : program->declarations) {
        mainTable.addVariable(decl);
    }

    for(size_t i = 0; i < proceduresTables.size(); i++) {
        mainTable.addProcedure(program->procedures[i]->identifier, program->procedures[i]->getParameterTypes(), proceduresTables[i].getOwnParametersMemoryAddresses(), proceduresTables[i].getReturnAddress());
    }

    for(auto& cmd : program->mainCommands) {
        std::vector<AssemblyInstruction> commandCode = compileCommand(mainTable, cmd);
        code.insert(code.end(), commandCode.begin(), commandCode.end());
    }

    // Fix procedure calls
    proceduresTables.push_back(mainTable);
    fixProcedureCallsJumps(code, proceduresTables);

    // Otpimalization
    code = initilizedConstants(code, isOnePresent);

    // Finish the program with HALT instruction
    code.push_back(AssemblyInstruction(AssemblyInstructionType::HALT, 0));
    return code;
}

