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

        switch (cmd->type) {
            case CommandType::Read: {

                std::vector<AssemblyInstruction> readCode = compile_READ(symbolsTable, 
                        std::move(std::unique_ptr<ReadCommand>(
                            dynamic_cast<ReadCommand*>(cmd.release())
                    ))
                );
                code.insert(code.end(), readCode.begin(), readCode.end());
                break;
            }
            case CommandType::Write: {
                   
                std::vector<AssemblyInstruction> writeCode = compile_WRITE(
                        symbolsTable, 
                        std::move(std::unique_ptr<WriteCommand>(
                            dynamic_cast<WriteCommand*>(cmd.release())
                    ))
                );
                code.insert(code.end(), writeCode.begin(), writeCode.end());
                break;
            }
            case CommandType::Assign: {
                std::vector<AssemblyInstruction> assignCode = compile_ASSIGN(
                        symbolsTable, 
                        std::move(std::unique_ptr<AssignCommand>(
                            dynamic_cast<AssignCommand*>(cmd.release())
                    ))
                );
                code.insert(code.end(), assignCode.begin(), assignCode.end());
                break;
            }
            default:
                throw std::runtime_error("Non-implemented command type");
        }
    }

    // Finish the program with HALT instruction
    code.push_back(AssemblyInstruction(AssemblyInstructionType::HALT));
    return code;
}

