#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"

std::pair<std::vector<AssemblyInstruction>, ll> compile_EXPRESSION(SymbolsTable& symbolsTable, const std::unique_ptr<Expression>& expr){
    std::vector<AssemblyInstruction> result;
    ll address = 0;

    // Grammar assures that left is always initialized
    Value& left = *(expr->left);

    // If there is no right expression, return the left one
    if(!expr->right){

        // If the expression is a number, just set some memory to it
        if(left.isNumber()){

            ll num = left.asNumber();
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, num));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, MEMORY_CONST_ASSIGN));
            address = MEMORY_CONST_ASSIGN;
            return {result, address};
        }

        // If the expression is an identifier
        Identifier identifier = left.asIdentifier();
        std::string id = identifier.id;

        // If the identifier is a variable
        if(identifier.isVariable()){

            // If variable is not declared, throw an error
            if(!symbolsTable.isVariableDeclared(id)){
                throw std::runtime_error("Variable '" + id + "' not declared but is used in expression");
            }

            // Don't need to load the value of the variable, just return its memory address
            address = symbolsTable.getMemoryAddress_variable(id);
            return {result, address};
        }

        // If its an array access by index
        ArrayAccess arrayAccess = identifier.arrayAccess.value();

        // If array is not declared, throw an error
        if(!symbolsTable.isArrayDeclared(id)){
            throw std::runtime_error("Array '" + id + "' not declared but is used in expression");
        }

        // If array is accessed by index
        if(arrayAccess.isByIndex()){

            ll index = arrayAccess.getIndex();

            // If index is out of bounds, throw an error
            if(!symbolsTable.isInsideArray(id, index)){
                throw std::runtime_error("Trying to access '" + id + "' at index " + std::to_string(index) + " which is out of bounds");
            }

            // Get memory address of array at the index
            address = symbolsTable.getMemoryAddress_at(id, index);
            return {result, address};
        }

        //Array access by variable
        std::string indexIdentifier = arrayAccess.getIndexVariable();

        // If index is not declared, throw an error
        if(!symbolsTable.isVariableDeclared(indexIdentifier)){
            throw std::runtime_error("Variable '" + indexIdentifier + "' not declared but is used as index in expression");
        }

        // If index is declared but not initialized, throw an error
        if(!symbolsTable.isVariableInitialized(indexIdentifier)){
            throw std::runtime_error("Variable '" + indexIdentifier + "' not initialized but is used as index in expression");
        }

        // Get memory address of index and load value of the array at it into memory. Account for the offset of the array
        ll indexAddres = symbolsTable.getMemoryAddress_variable(indexIdentifier);
        ll arrayStartAddres = symbolsTable.getMemoryAddress_start(identifier.id) + symbolsTable.get_offset(identifier.id);

        result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, arrayStartAddres));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, indexAddres));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::LOADI, MEMORY_CONST_ASSIGN));
        address = MEMORY_CONST_ASSIGN;
        // MEMORY_CONST_ASSIGN = array[index]
        return {result, address};
    }

    // If there is a right expression
    Value& right = *(expr->right);

    
}