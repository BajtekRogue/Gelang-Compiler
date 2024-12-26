#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utility.hpp"


std::vector<AssemblyInstruction> compileWrite(SymbolsTable& symbolsTable, const std::unique_ptr<WriteCommand>& cmd){      
    std::vector<AssemblyInstruction> result;
    Value& val = *(cmd->value);
    
    result.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "WRITE " + val.toString()));
    
    // WRITE a number
    if(val.isNumber()){

        int64_t num = val.asNumber();

        // If 0 clear the accumulator because it is cheaper
        if(num == 0){
            result.push_back(AssemblyInstruction(Instruction::SUB, 0));
        }
        else{
            result.push_back(AssemblyInstruction(Instruction::SET, num));
        }

        result.push_back(AssemblyInstruction(Instruction::PUT, 0));
        return result;
    }

    // Check if the variable is used correctly
    validateUseOfVariable(symbolsTable, cmd->value->asIdentifier(), "WRITE", true);
    
    // WRITE a identifier
    Identifier identifier = val.asIdentifier();
    std::string id = identifier.id;

    // Check if identifier is a variable
    if(identifier.isVariable()){
        
        // If variable is local
        if(!symbolsTable.isParameter(id)){
            int64_t address = symbolsTable.getMemoryAddress_variable(id);
            result.push_back(AssemblyInstruction(Instruction::PUT, address));
            symbolsTable.markAsInitialized(id);  
            return result;
        }

        // If variable is a parameter
        int64_t parameterAddress = symbolsTable.getMemoryAddressPointer_parameter(id);
        result.push_back(AssemblyInstruction(Instruction::LOADI, parameterAddress));
        result.push_back(AssemblyInstruction(Instruction::PUT, 0));

        return result;
    }

    // WRITE an array
    ArrayAccess arrayAccess = identifier.getArrayAccess();
            
    // If array is accessed by index
    if(arrayAccess.isByIndex()){

        int64_t index = arrayAccess.getIndex();

        // If the array is local
        if(!symbolsTable.isParameter(id)){
            int64_t address = symbolsTable.getMemoryAddress_at(id, index);
            result.push_back(AssemblyInstruction(Instruction::PUT, address));

            return result;
        }

        // If the array is a parameter
        int64_t address = symbolsTable.getMemoryAddressPointer_parameter(id);

        // If index is 0
        if(index == 0){
            result.push_back(AssemblyInstruction(Instruction::LOADI, address));
            result.push_back(AssemblyInstruction(Instruction::PUT, 0));
            return result;
        }

        result.push_back(AssemblyInstruction(Instruction::SET, index));
        result.push_back(AssemblyInstruction(Instruction::ADD, address));
        result.push_back(AssemblyInstruction(Instruction::LOADI, 0));
        result.push_back(AssemblyInstruction(Instruction::PUT, 0));

        return result;
    }

    // If the array is accessed by a variable             
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // If both array and the variable are local
    if(!symbolsTable.isParameter(id) && !symbolsTable.isParameter(indexIdentifier)){

        int64_t indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
        int64_t arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

        result.push_back(AssemblyInstruction(Instruction::SET, arrayStartAddress));
        result.push_back(AssemblyInstruction(Instruction::ADD, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::LOADI, 0));
        result.push_back(AssemblyInstruction(Instruction::PUT, 0));

        return result;
    }

    // If array is local but the index is a parameter
    if(!symbolsTable.isParameter(id) && symbolsTable.isParameter(indexIdentifier)){
        
        int64_t indexAddress = symbolsTable.getMemoryAddressPointer_parameter(indexIdentifier);
        int64_t arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

        result.push_back(AssemblyInstruction(Instruction::SET, arrayStartAddress));
        result.push_back(AssemblyInstruction(Instruction::ADDI, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::LOADI, 0));
        result.push_back(AssemblyInstruction(Instruction::PUT, 0));

        return result;
    }

    // If array is a parameter but the index is local
    if(symbolsTable.isParameter(id) && !symbolsTable.isParameter(indexIdentifier)){
        
        int64_t indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
        int64_t arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        result.push_back(AssemblyInstruction(Instruction::LOAD, arrayAddress));
        result.push_back(AssemblyInstruction(Instruction::ADD, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::LOADI, 0));
        result.push_back(AssemblyInstruction(Instruction::PUT, 0));

        return result;
    }

    // If both array and the index are parameters
    if(symbolsTable.isParameter(id) && symbolsTable.isParameter(indexIdentifier)){
        
        int64_t indexAddress = symbolsTable.getMemoryAddressPointer_parameter(indexIdentifier);
        int64_t arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        result.push_back(AssemblyInstruction(Instruction::LOAD, arrayAddress));
        result.push_back(AssemblyInstruction(Instruction::ADDI, indexAddress));
        result.push_back(AssemblyInstruction(Instruction::LOADI, 0));
        result.push_back(AssemblyInstruction(Instruction::PUT, 0));

        return result;
    }
    return result;
}