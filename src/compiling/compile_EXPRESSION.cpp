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

        validateUseOfVariable(symbolsTable, identifier, "expression", true);

        // If the identifier is a variable
        if(identifier.isVariable()){

            // Don't need to load the value of the variable, just return its memory address
            address = symbolsTable.getMemoryAddress_variable(id);
            return {result, address};
        }

        // If its an array access by index
        ArrayAccess arrayAccess = identifier.arrayAccess.value();

        // If array is accessed by index
        if(arrayAccess.isByIndex()){

            ll index = arrayAccess.getIndex();

            // Get memory address of array at the index
            address = symbolsTable.getMemoryAddress_at(id, index);
            return {result, address};
        }

        //Array access by variable
        std::string indexIdentifier = arrayAccess.getIndexVariable();

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

    // If both are numbers preform the operation during compile time and return the result
    if(left.isNumber() && right.isNumber()){
        
        ll leftNum = left.asNumber();
        ll rightNum = right.asNumber();
        ll resultNum = 0;

        switch(expr->type){
            case ExpressionType::Plus:
                resultNum = leftNum + rightNum;
                break;
            case ExpressionType::Minus:
                resultNum = leftNum - rightNum;
                break;
            case ExpressionType::Multiply:
                resultNum = leftNum * rightNum;
                break;
            case ExpressionType::Divide:
                resultNum = (rightNum == 0 ? 0 : leftNum / rightNum);
                if(leftNum < 0 && rightNum > 0){
                    resultNum -= 1;
                }else if(leftNum > 0 && rightNum < 0){
                    resultNum -= 1;
                }
                break;
            case ExpressionType::Modulo:
                resultNum = (rightNum == 0 ? 0 : leftNum % rightNum);
                if(leftNum < 0 && rightNum > 0){
                    resultNum += rightNum;
                }else if(leftNum > 0 && rightNum < 0){
                    resultNum += rightNum;
                }
                break;
            default:
                throw std::runtime_error("Unknown expression type");
        }

        result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, resultNum));
        result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, MEMORY_CONST_ASSIGN));
        address = MEMORY_CONST_ASSIGN;
        return {result, address};
    }

    // Check if the variables are declared and initialized before arithmetic operations
    if(left.isIdentifier()){
        validateUseOfVariable(symbolsTable, left.asIdentifier(), "expression", true);
    }
    if(right.isIdentifier()){
        validateUseOfVariable(symbolsTable, right.asIdentifier(), "expression", true);
    }

    // If at least one is a number load left into the accumulator and right to memory[1] and preform the operation
    std::vector<AssemblyInstruction> loadRight_code = getValueToDestinationAddress(symbolsTable, right, 1);
    result.insert(result.end(), loadRight_code.begin(), loadRight_code.end());

    std::vector<AssemblyInstruction> loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
    result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());


    switch(expr->type){
        case ExpressionType::Plus:
            // Add the value of the right expression to the accumulator and store the result safely
            result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, 1));            
            result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, MEMORY_EXPRESSION_RESULT));
            return {result, MEMORY_EXPRESSION_RESULT};
            break;
        case ExpressionType::Minus:
            // Subtract the value of the right expression from the accumulator and store the result safely
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::STORE, MEMORY_EXPRESSION_RESULT));
            return {result, MEMORY_EXPRESSION_RESULT};
            break;
        default:
            std::cout << "Expression not implemented" << std::endl;
            exit(1);
    }
}