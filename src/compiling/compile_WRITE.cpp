#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"

std::vector<AssemblyInstruction> compile_WRITE(SymbolsTable& symbolsTable, const std::unique_ptr<WriteCommand>& cmd){      
    std::vector<AssemblyInstruction> result;
    Value& val = *(cmd->value);
    
    // WRITE a number
    if(val.isNumber()){

        ll num = val.asNumber();

        // If 0 clear the accumulator because it is cheaper
        if(num == 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 0));
        }else{
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, num));
        }

        result.push_back(AssemblyInstruction(AssemblyInstructionType::PUT, 0));
        return result;
    }

    // Check if the variable is used correctly
    validateUseOfVariable(symbolsTable, cmd->value->asIdentifier(), "WRITE", true);
    
    // WRITE a identifier
    Identifier identifier = val.asIdentifier();
    std::string id = identifier.id;

    // Check if identifier is a variable
    if(identifier.isVariable()){
        
        ll address = symbolsTable.getMemoryAddress_variable(id);
        result.push_back(AssemblyInstruction(AssemblyInstructionType::PUT, address));
        symbolsTable.markAsInitialized(id);  
        return result;
    }

    // WRITE an array
    ArrayAccess arrayAccess = identifier.getArrayAccess();
            
    // If array is accessed by index
    if(arrayAccess.isByIndex()){

        ll index = arrayAccess.getIndex();
      
        // Get memory address of array at the index and ouput the value there
        ll address = symbolsTable.getMemoryAddress_at(id, index);
        result.push_back(AssemblyInstruction(AssemblyInstructionType::PUT, address));
        return result;
    }

    // If the array is accessed by a variable             
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // Get memory address of index and output the value there. Account for the offset of the array
    ll indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
    ll arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

    result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddress));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddress));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::LOADI, 0));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::PUT, 0));
    return result;
}