#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utlity.hpp"


std::vector<AssemblyInstruction> compile(std::unique_ptr<Program>& program) {

    std::vector<AssemblyInstruction> code;

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

        // Create a symbols table, align starting addresses to multiple of 1000 for clarity
        int64_t nextFreeAddress = (i == 0 ? 100 : proceduresTables[i-1].getLastMemoryAddress() + 1);
        proceduresTables[i] = SymbolsTable(program->procedures[i]->identifier, findNext1000(nextFreeAddress));
        nextFreeAddress = proceduresTables[i].getLastMemoryAddress() + 1;

        // Add parameters and variables to the table
        for(const auto& param: program->procedures[i]->parameters) {
            proceduresTables[i].addParameter(param->identifier, param->type);
        }

        for(const auto& decl : program->procedures[i]->declarations) {
            proceduresTables[i].addVariable(decl);
        }

        // Add above declared procedures to be available in the current one
        for(size_t j = 0; j < i; j++) {
            proceduresTables[i].addProcedure(program->procedures[j]->identifier, program->procedures[j]->getParameterTypes(), proceduresTables[j].getOwnParametersMemoryAddresses(), proceduresTables[j].getReturnAddress());
        }

        code.push_back(AssemblyInstruction(Instruction::LABEL_PROCEDURE, program->procedures[i]->identifier));

        // Compile the procedure commands
        for(auto& cmd : program->procedures[i]->commands) {
            std::vector<AssemblyInstruction> commandCode = compileCommand(proceduresTables[i], cmd);
            code.insert(code.end(), commandCode.begin(), commandCode.end());
        }

        // Add return instruction
        proceduresTables[i].setReturnAddress();
        code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "RETURN " + program->procedures[i]->identifier));
        code.push_back(AssemblyInstruction(Instruction::RTRN, proceduresTables[i].getReturnAddress()));
        code.push_back(AssemblyInstruction(Instruction::LABEL_ENDPROCEDURE, program->procedures[i]->identifier + "\n"));
    }

    // Compile the main procedure
    int64_t mainVariablesMemoryAddress = (proceduresTables.empty() ? MEMORY_START : findNext1000(proceduresTables.back().getLastMemoryAddress() + 1));
    SymbolsTable mainTable("MAIN", mainVariablesMemoryAddress);
    std::vector<AssemblyInstruction> mainCode;
    mainCode.push_back(AssemblyInstruction(Instruction::LABEL_MAIN, ""));

    for(const auto& decl : program->declarations) {
        mainTable.addVariable(decl);
    }

    for(size_t i = 0; i < proceduresTables.size(); i++) {
        mainTable.addProcedure(program->procedures[i]->identifier, program->procedures[i]->getParameterTypes(), proceduresTables[i].getOwnParametersMemoryAddresses(), proceduresTables[i].getReturnAddress());
    }

    for(auto& cmd : program->mainCommands) {
        std::vector<AssemblyInstruction> commandCode = compileCommand(mainTable, cmd);
        mainCode.insert(mainCode.end(), commandCode.begin(), commandCode.end());
    }

    // Add arithmetic procedures if needed
    if(Arithmetic::multiplication){
        std::vector<AssemblyInstruction> multiplicationCode = generateMultiplication();
        code.insert(code.begin(), multiplicationCode.begin(), multiplicationCode.end());
    }

    if(Arithmetic::division){
        std::vector<AssemblyInstruction> divisionCode = generateDivision();
        code.insert(code.begin(), divisionCode.begin(), divisionCode.end());
    }

    if(Arithmetic::modulo){
        std::vector<AssemblyInstruction> moduloCode = generateModulo();
        code.insert(code.begin(), moduloCode.begin(), moduloCode.end());
    }

    // Jump to the main procedure if needed
    int64_t jumpMain = countRealInstructions(code);
    if(jumpMain > 0){
        std::vector<AssemblyInstruction> jumpToMainCode;
        jumpToMainCode.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Jump to MAIN procedure"));
        jumpToMainCode.push_back(AssemblyInstruction(Instruction::JUMP, jumpMain + 1));
        jumpToMainCode.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "\n"));
        code.insert(code.begin(), jumpToMainCode.begin(), jumpToMainCode.end());  
    }
    code.insert(code.end(), mainCode.begin(), mainCode.end());

    // Fix procedure calls
    fixProcedureCallsJumps(code);

    // Otpimalization
    code = cacheConstants(code, LabelCounters::forCounter != 0);

    // Fix procedure calls again after inserting code at the beginning...
    fixProcedureCallsJumps(code);

    // Finish the program with HALT instruction
    code.push_back(AssemblyInstruction(Instruction::HALT, /*this 0 is meaningless */ 0));

    return code;
}

