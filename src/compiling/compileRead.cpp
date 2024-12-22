#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utlity.hpp"

std::vector<AssemblyInstruction> compileRead(SymbolsTable& symbolsTable, const std::unique_ptr<ReadCommand>& cmd) {
    std::vector<AssemblyInstruction> result;
    Identifier* identifier = cmd->identifier.get();
    std::string id = identifier->id;

    validateUseOfVariable(symbolsTable, *(cmd->identifier), "READ", false);
    if(symbolsTable.isIterator(id)){
        throw std::logic_error("Cannot READ into an iterator " + id);
    }

    result.push_back(AssemblyInstruction(AssemblyInstructionType::LABEL_INSTRUCTION, "READ " + id));

    // READ a variable
    if(identifier->isVariable()){

        // If variable is local
        if(!symbolsTable.isParameter(id)){
            ll address = symbolsTable.getMemoryAddress_variable(id);

            result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, address));
            symbolsTable.markAsInitialized(id);
            return result;
        }

        // If variable is a parameter
        ll address = symbolsTable.getMemoryAddressPointer_parameter(id);

        result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, 0));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STOREI, address));
        return result;
    }

    // READ an array
    ArrayAccess& arrayAccess = identifier->arrayAccess.value();

    // If array is accessed by index
    if(arrayAccess.isByIndex()){

        ll index = arrayAccess.getIndex();

        // If the array is local
        if(!symbolsTable.isParameter(id)){
            // Get memory address of array at the index and store the input there
            ll address = symbolsTable.getMemoryAddress_at(id, index);
            result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, address));
            return result;
        }

        // If the array is a parameter
        ll address = symbolsTable.getMemoryAddressPointer_parameter(id);

        // If index is 0
        if(index == 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, 0));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::STOREI, address));
            return result;
        }

        result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, index));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::ADDI, address));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, MEMORY_ARRAY_VARIABLE_ASSIGN));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, 0));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STOREI, MEMORY_ARRAY_VARIABLE_ASSIGN));

    }

    // If array is accessed by variable
    std::string indexIdentifier = arrayAccess.getIndexVariable();

    // If both array and the variable are local
    if(!symbolsTable.isParameter(id) && !symbolsTable.isParameter(indexIdentifier)){
        
        // Get memory address of index and store the input there. Account for the offset of the array
        ll indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
        ll arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

        result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, MEMORY_ARRAY_VARIABLE_ASSIGN));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, 0));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STOREI, MEMORY_ARRAY_VARIABLE_ASSIGN));
    }

    // If array is local but the index is a parameter
    if(!symbolsTable.isParameter(id) && symbolsTable.isParameter(indexIdentifier)){

        ll indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
        ll arrayStartAddress = symbolsTable.getMemoryAddress_start(id) + symbolsTable.get_offset(id);

        result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::ADDI, indexAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, MEMORY_ARRAY_VARIABLE_ASSIGN));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, 0));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STOREI, MEMORY_ARRAY_VARIABLE_ASSIGN));
    }
    
    // If array is a parameter but the index is local
    if(symbolsTable.isParameter(id) && !symbolsTable.isParameter(indexIdentifier)){

        ll indexAddress = symbolsTable.getMemoryAddress_variable(indexIdentifier);
        ll arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOADI, arrayAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, MEMORY_ARRAY_VARIABLE_ASSIGN));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, 0));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STOREI, MEMORY_ARRAY_VARIABLE_ASSIGN));
    }

    // If both array and the index are parameters
    if(symbolsTable.isParameter(id) && symbolsTable.isParameter(indexIdentifier)){

        ll indexAddress = symbolsTable.getMemoryAddressPointer_parameter(indexIdentifier);
        ll arrayAddress = symbolsTable.getMemoryAddressPointer_parameter(id);

        // Load into the accumulator the value at the index of the array
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOADI, arrayAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::ADDI, indexAddress));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, MEMORY_ARRAY_VARIABLE_ASSIGN));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::GET, 0));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STOREI, MEMORY_ARRAY_VARIABLE_ASSIGN));

        return result;
    }
    return result;
}
