#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"


std::vector<AssemblyInstruction> compileCommand(SymbolsTable& symbolsTable, std::unique_ptr<Command>& cmd) {

    switch (cmd->type) {
        case CommandType::Read: {
            return compileRead(symbolsTable, std::move(std::unique_ptr<ReadCommand>(dynamic_cast<ReadCommand*>(cmd.release()))));
            break;
        }
        case CommandType::Write: {
            return compileWrite(symbolsTable, std::move(std::unique_ptr<WriteCommand>(dynamic_cast<WriteCommand*>(cmd.release()))));
            break;
        }
        case CommandType::Assign: {
            return compileAssign(symbolsTable, std::move(std::unique_ptr<AssignCommand>(dynamic_cast<AssignCommand*>(cmd.release()))));
            break;
        }
        case CommandType::If:{
            return compileIf(symbolsTable, std::move(std::unique_ptr<IfCommand>(dynamic_cast<IfCommand*>(cmd.release()))));
            break;
        }
        case CommandType::IfElse:{
            return compileIfElse(symbolsTable, std::move(std::unique_ptr<IfElseCommand>(dynamic_cast<IfElseCommand*>(cmd.release()))));
            break;
        }
        case CommandType::While:{
            return compileWhile(symbolsTable, std::move(std::unique_ptr<WhileCommand>(dynamic_cast<WhileCommand*>(cmd.release()))));
            break;
        }
        case CommandType::Repeat:{
            return compileRepeat(symbolsTable, std::move(std::unique_ptr<RepeatCommand>(dynamic_cast<RepeatCommand*>(cmd.release()))));
            break;
        }
        case CommandType::ForTo:{
            return compileForTo(symbolsTable, std::move(std::unique_ptr<ForToCommand>(dynamic_cast<ForToCommand*>(cmd.release()))));
            break;
        }
        case CommandType::ForDownto:{
            return compileForDownto(symbolsTable, std::move(std::unique_ptr<ForDowntoCommand>(dynamic_cast<ForDowntoCommand*>(cmd.release()))));
            break;
        }
        case CommandType::ProcedureCall:{
            return compileProcedureCall(symbolsTable, std::move(std::unique_ptr<ProcedureCallCommand>(dynamic_cast<ProcedureCallCommand*>(cmd.release()))));
            break;
        }
        default:
            throw std::runtime_error("Non-implemented command type");
    }
}

std::vector<AssemblyInstruction> compileAll(SymbolsTable& symbolsTable, std::vector<std::unique_ptr<Command>>& commands){

    std::vector<AssemblyInstruction> code;

    for(auto& cmd : commands){
        
        std::vector<AssemblyInstruction> commandCode = compileCommand(symbolsTable, cmd);
        code.insert(code.end(), commandCode.begin(), commandCode.end());
    }

    return code;
}