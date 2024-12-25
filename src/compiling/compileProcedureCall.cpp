#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utlity.hpp"


std::vector<AssemblyInstruction> compileProcedureCall(SymbolsTable& symbolsTable, const std::unique_ptr<ProcedureCallCommand>& cmd){

    std::vector<AssemblyInstruction> code;

    // Check if procedure is declared
    if(!symbolsTable.isProcedureDeclared(cmd->identifier)){
        throw std::logic_error("Procedure `" + cmd->identifier + "` was not declared but is called in " + symbolsTable.getOwnIdentifier());
    }

    // Start checking parameters
    std::vector<ParameterType> procParamTypes = symbolsTable.getProcedureParameters(cmd->identifier);
    size_t expectedNumArgs = procParamTypes.size();
    size_t givenNumArgs = cmd->arguments.size();

    if(expectedNumArgs != givenNumArgs){
        throw std::logic_error("Invalid number of arguments for procedure call " + cmd->identifier);
    }

    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, cmd->toString()));

    std::vector<int64_t> argumentsAddresses = symbolsTable.getProcedureParametersMemoryAddresses(cmd->identifier);
    int64_t returnAddress = symbolsTable.getProcedureReturnAddress(cmd->identifier);

    // Load the arguments to the expected memory addresses
    for(size_t i = 0; i < givenNumArgs; i++){

        // Check if the argument is valid
        if(cmd->arguments[i]->isNumber()){
            throw std::logic_error("A number `" + std::to_string(cmd->arguments[i]->asNumber()) + "` cannot be passed as an argument to a procedure");
        }
        if(symbolsTable.isIterator(cmd->arguments[i]->asIdentifier().id)){
            throw std::logic_error("Iterator `" + cmd->arguments[i]->asIdentifier().toString() + "` cannot be passed as an argument to a procedure");
        }

        Identifier paramId = cmd->arguments[i]->asIdentifier();

        // Check if the variable is used correctly
        validateUseOfVariable(symbolsTable, paramId, "Procedure call", false, procParamTypes[i] == ParameterType::Array);

        if(symbolsTable.isVariableDeclared(paramId.id) && procParamTypes[i] == ParameterType::Array){
            throw std::logic_error("Variable `" + cmd->arguments[i]->asIdentifier().toString() + "` cannot be passed as "  + std::to_string(i) + "-th argument to a procedure as it expects an array");
        }
        else if(symbolsTable.isArrayDeclared(paramId.id) && procParamTypes[i] == ParameterType::Integer){
            throw std::logic_error("Array `" + cmd->arguments[i]->asIdentifier().toString() + "` cannot be passed as "  + std::to_string(i) + "-th argument to a procedure as it expects a variable");
        }

        // Mark variables as initilized
        if(symbolsTable.isVariableDeclared(paramId.id)){
            symbolsTable.markAsInitialized(paramId.id);
        }
        
        // Load the pointers where the procedure expects them
        std::vector<AssemblyInstruction> loadArgCode = getAddressToDestinationAddress(symbolsTable, *(cmd->arguments[i]), argumentsAddresses[i]);
        code.insert(code.end(), loadArgCode.begin(), loadArgCode.end());
    }

    // Set the return address and jump to the procedure. These addresses will be fixed later
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Setting return address and jumping to " + cmd->identifier));
    code.push_back(AssemblyInstruction(Instruction::SET, cmd->identifier));
    code.push_back(AssemblyInstruction(Instruction::STORE, returnAddress));
    code.push_back(AssemblyInstruction(Instruction::JUMP, cmd->identifier));
    
    return code;
}