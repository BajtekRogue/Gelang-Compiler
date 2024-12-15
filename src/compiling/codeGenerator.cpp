#include "languageStructs.hpp"
#include "utility.hpp"
#include "symbolsTable.hpp"

std::vector<AssemblyInstruction> compile(std::unique_ptr<Program>& program) {
    std::vector<AssemblyInstruction> code;

    ll mainVariablesMemoryAddress = 1024;
    SymbolsTable symbolsTable(mainVariablesMemoryAddress);

    for(const auto& decl : program->declarations) {
        symbolsTable.addVariable(decl);
    }

    std::cout << "\nSymbols table for main:\n";
    symbolsTable.printSymbols();

    for(auto& cmd : program->mainCommands) {
        switch (cmd->type) {
            case CommandType::Read: {
                ReadCommand* readCmd = dynamic_cast<ReadCommand*>(cmd.get());
                std::unique_ptr<ReadCommand> readCmdPtr(readCmd);
                std::vector<AssemblyInstruction> readCode = compile_READ(symbolsTable, readCmdPtr);
                code.insert(code.end(), readCode.begin(), readCode.end());
                readCmdPtr.release();
                break;
            }
            case CommandType::Write: {
                WriteCommand* writeCmd = dynamic_cast<WriteCommand*>(cmd.get());
                std::unique_ptr<WriteCommand> writeCmdPtr(writeCmd);
                std::vector<AssemblyInstruction> writeCode = compile_WRITE(symbolsTable, writeCmdPtr);
                code.insert(code.end(), writeCode.begin(), writeCode.end());
                writeCmdPtr.release();
                break;
            }
            default:
                throw std::runtime_error("Unknown command type");
        }
    }

    //finished by adding HALT instruction
    code.push_back(AssemblyInstruction(AssemblyInstructionType::HALT));
    return code;
}


std::vector<AssemblyInstruction> compile_READ(SymbolsTable& symbolsTable, const std::unique_ptr<ReadCommand>& cmd) {
    std::vector<AssemblyInstruction> result;
    std::string identifier = cmd->identifier->id;

    // READ a variable
    if(!cmd->identifier->arrayAccess.has_value()){

        // If variable is not declared, throw an error
        if(!symbolsTable.isVariableDeclared(identifier)){
            throw std::runtime_error("Variable '" + identifier + "' not declared but is used in READ command");
        }

        // Get its memory address and add a GET instruction
        ll addres = symbolsTable.getMemoryAddress_variable(identifier);
        result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, addres));
        symbolsTable.markAsInitialized(identifier);

    }else{

        // READ an array
        ArrayAccess& arrayAccess = cmd->identifier->arrayAccess.value();

        // If array is not declared, throw an error
        if(!symbolsTable.isArrayDeclared(identifier)){
            throw std::runtime_error("Array '" + identifier + "' not declared but is used in READ command");
        }

        // If array is accessed by index
        if(std::holds_alternative<ll>(arrayAccess.index)){

            ll index = std::get<ll>(arrayAccess.index);

            // If index is out of bounds, throw an error
            if(!symbolsTable.isInsideArray(identifier, index)){
                throw std::runtime_error("Trying to READ into '" + identifier + "' at index " + std::to_string(index) + " which is out of bounds");
            }

            // Get memory address of array at the index and add a GET instruction
            ll addres = symbolsTable.getMemoryAddress_at(identifier, index);
            result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, addres));
            symbolsTable.markAsInitialized_at(identifier, index);

        }else{
            
            std::string indexIdentifier = std::get<std::string>(arrayAccess.index);

            // If index is not declared, throw an error
            if(!symbolsTable.isVariableDeclared(indexIdentifier)){
                throw std::runtime_error("Variable '" + indexIdentifier + "' not declared but is used as index in READ command");
            }

            // Get memory address of index and add a PUT instruction. Account for the offset of the array
            ll indexAddres = symbolsTable.getMemoryAddress_variable(indexIdentifier);
            ll arrayStartAddres = symbolsTable.getMemoryAddress_start(identifier) + symbolsTable.get_offset(identifier);

            result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddres));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddres));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, 0));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::STOREI, 1));
        }
    }

    return result;
}

std::vector<AssemblyInstruction> compile_WRITE(SymbolsTable& symbolsTable, const std::unique_ptr<WriteCommand>& cmd){      
    std::vector<AssemblyInstruction> result;
    Value& val = *(cmd->value);
    
    // WRITE a number
    if(val.isNumber()){

        result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, val.asNumber()));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::PUT, 0));

    }else{

        std::string identifier = val.asIdentifier().id;

        // WRITE a variable
        if(!val.asIdentifier().arrayAccess.has_value()){

            if(!symbolsTable.isVariableDeclared(identifier)){
                throw std::runtime_error("Variable '" + identifier + "' not declared but is used in WRITE command");
            }

            ll addres = symbolsTable.getMemoryAddress_variable(identifier);
            result.push_back(AssemblyInstruction(AssemblyInstructionType::PUT, addres));
            symbolsTable.markAsInitialized(identifier);  

        // WRITE an array
        }else{
            ArrayAccess arrayAccess = val.asIdentifier().arrayAccess.value();
            
            // If array is not declared, throw an error
            if(!symbolsTable.isArrayDeclared(identifier)){
                throw std::runtime_error("Array '" + identifier + "' not declared but is used in WRITE command");
            }

            // If array is accessed by index
            if(std::holds_alternative<ll>(arrayAccess.index)){

                ll index = std::get<ll>(arrayAccess.index);

                // If index is out of bounds, throw an error
                if(!symbolsTable.isInsideArray(identifier, index)){
                    throw std::runtime_error("Trying to WRITE from '" + identifier + "' at index " + std::to_string(index) + " which is out of bounds");
                }

                // Get memory address of array at the index and add a PUT instruction
                ll addres = symbolsTable.getMemoryAddress_at(identifier, index);
                result.push_back(AssemblyInstruction(AssemblyInstructionType::PUT, addres));

            }else{
                
                std::string indexIdentifier = std::get<std::string>(arrayAccess.index);

                // If index is not declared, throw an error
                if(!symbolsTable.isVariableDeclared(indexIdentifier)){
                    throw std::runtime_error("Variable '" + indexIdentifier + "' not declared but is used as index in WRITE command");
                }

                // Get memory address of index and add a PUT instruction. Account for the offset of the array
                ll indexAddres = symbolsTable.getMemoryAddress_variable(indexIdentifier);
                ll arrayStartAddres = symbolsTable.getMemoryAddress_start(identifier) + symbolsTable.get_offset(identifier);

                result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddres));
                result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddres));
                result.push_back(AssemblyInstruction(AssemblyInstructionType::LOADI, 0));
                result.push_back(AssemblyInstruction(AssemblyInstructionType::PUT, 0));
            }
        } 

    }

    return result;
}