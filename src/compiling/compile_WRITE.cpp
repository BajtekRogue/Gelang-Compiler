#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"

std::vector<AssemblyInstruction> compile_WRITE(SymbolsTable& symbolsTable, const std::unique_ptr<WriteCommand>& cmd){      
    std::vector<AssemblyInstruction> result;
    Value& val = *(cmd->value);
    
    // WRITE a number
    if(val.isNumber()){

        ll num = val.asNumber();
        result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, num));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::PUT, 0));
        return result;
    }

    // WRITE a identifier
    Identifier identifier = val.asIdentifier();
    std::string id = identifier.id;

    // Check if identifier is a variable
    if(identifier.isVariable()){
        
        // If identifier is not declared, throw an error
        if(!symbolsTable.isVariableDeclared(id)){
            throw std::runtime_error("Variable '" + id + "' not declared but is used in WRITE command");
        }
        
        // If variable is not initialized, throw an error
        if(!symbolsTable.isVariableInitialized(id)){
            throw std::runtime_error("Variable '" + id + "' not initialized but is used in WRITE command");
        }

        ll address = symbolsTable.getMemoryAddress_variable(id);
        result.push_back(AssemblyInstruction(AssemblyInstructionType::PUT, address));
        symbolsTable.markAsInitialized(id);  
        return result;
    }

    // WRITE an array
    ArrayAccess arrayAccess = identifier.getArrayAccess();
            
    // If array is not declared, throw an error
    if(!symbolsTable.isArrayDeclared(id)){
        throw std::runtime_error("Array '" + id + "' not declared but is used in WRITE command");
    }

    // If array is accessed by index
    if(arrayAccess.isByIndex()){

        ll index = arrayAccess.getIndex();

            // If index is out of bounds, throw an error
            if(!symbolsTable.isInsideArray(id, index)){
                throw std::runtime_error("Trying to WRITE from '" + id + "' at index " + std::to_string(index) + " which is out of bounds");
            }

            // If array is not initialized, throw an error
            if(!symbolsTable.isArrayInitialized_at(id, index)){
                throw std::runtime_error("Array '" + id + "' at index " + std::to_string(index) + " not initialized but is used in WRITE command");
            }
            
            // Get memory address of array at the index and ouput the value there
            ll address = symbolsTable.getMemoryAddress_at(id, index);
            result.push_back(AssemblyInstruction(AssemblyInstructionType::PUT, address));
            return result;
    }

    // If the array is accessed by a variable             
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // If index is not declared, throw an error
    if(!symbolsTable.isVariableDeclared(indexIdentifier)){
        throw std::runtime_error("Variable '" + indexIdentifier + "' not declared but is used as index in WRITE command");
    }

    // If index is declared but not initialized, throw an error
    if(!symbolsTable.isVariableInitialized(indexIdentifier)){
        throw std::runtime_error("Variable '" + indexIdentifier + "' not initialized but is used as index in WRITE command");
    }

    // Get memory address of index and output the value there. Account for the offset of the array
    ll indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
    ll arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

    result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddress));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddress));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::LOADI, 0));
    result.push_back(AssemblyInstruction(AssemblyInstructionType::PUT, 0));
    return result;
}