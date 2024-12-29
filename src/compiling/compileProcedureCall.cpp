#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utility.hpp"


std::vector<AssemblyInstruction> compileProcedureCall(SymbolsTable& symbolsTable, const std::unique_ptr<ProcedureCallCommand>& cmd){

    std::vector<AssemblyInstruction> code;

    // Check for recursion
    if(symbolsTable.getOwnIdentifier() == cmd->identifier){
        ErrorHandler::recursionDetected(cmd->identifier, cmd->lineNumber);
    }

    // Check if procedure is declared
    if(!symbolsTable.isProcedureDeclared(cmd->identifier)){
        ErrorHandler::notDeclaredProcedure(cmd->identifier, cmd->lineNumber);
    }

    // Start checking parameters
    std::vector<ParameterType> procParamTypes = symbolsTable.getProcedureParameters(cmd->identifier);
    size_t expectedNumArgs = procParamTypes.size();
    size_t givenNumArgs = cmd->arguments.size();

    if(expectedNumArgs != givenNumArgs){
        ErrorHandler::invalidNumberOfArguments(cmd->identifier, expectedNumArgs, givenNumArgs, cmd->lineNumber);
    }

    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, cmd->toString()));

    std::vector<int64_t> argumentsAddresses = symbolsTable.getProcedureParametersMemoryAddresses(cmd->identifier);
    int64_t returnAddress = symbolsTable.getProcedureReturnAddress(cmd->identifier);

    // Load the arguments to the expected memory addresses
    for(size_t i = 0; i < givenNumArgs; i++){

        // Check if the argument is not an iterator
        if(symbolsTable.isIterator(cmd->arguments[i]->asIdentifier().id)){
            ErrorHandler::iteratorAsParameter(cmd->arguments[i]->asIdentifier().id, cmd->lineNumber);
        }

        Identifier paramId = cmd->arguments[i]->asIdentifier();

        // Check if the argument is used correctly
        if(symbolsTable.isVariableDeclared(paramId.id) && procParamTypes[i] == ParameterType::Array){
            ErrorHandler::invalidArgumentType(cmd->identifier, cmd->arguments[i]->asIdentifier().toString(), ParameterType::Array, ParameterType::Integer, i + 1, cmd->lineNumber);
        }
        else if(symbolsTable.isArrayDeclared(paramId.id) && procParamTypes[i] == ParameterType::Integer){
            ErrorHandler::invalidArgumentType(cmd->identifier, cmd->arguments[i]->asIdentifier().toString(), ParameterType::Integer, ParameterType::Array, i + 1, cmd->lineNumber);
        }
        validateUseOfVariable(symbolsTable, paramId, cmd->lineNumber, false, procParamTypes[i] == ParameterType::Array);

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