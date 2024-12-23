#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utlity.hpp"


std::vector<AssemblyInstruction> compileExpression(SymbolsTable& symbolsTable, const std::unique_ptr<Expression>& expr){
    std::vector<AssemblyInstruction> result;

    // Grammar assures that left is always initialized
    Value& left = *(expr->left);

    // If there is no right expression, return the left one
    if(!expr->right){

        // Check if the variable is used correctly
        if(left.isIdentifier()){
            validateUseOfVariable(symbolsTable, left.asIdentifier(), "expression", true);
        }
        
        return getValueToDestinationAddress(symbolsTable, left, 0);
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
            
        // If 0 clear the accumulator because it is cheaper
        if(resultNum == 0){
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 0));
        }else{
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SET, resultNum));
        }
        return result;
    }

    // Check if the variables are declared and initialized before arithmetic operations
    if(left.isIdentifier()){
        validateUseOfVariable(symbolsTable, left.asIdentifier(), "expression", true);
    }
    if(right.isIdentifier()){
        validateUseOfVariable(symbolsTable, right.asIdentifier(), "expression", true);
    }

    // Check special cases of arithmetic identities
    // 0 + x = x
    if(expr->type == ExpressionType::Plus && left.isNumber() && left.asNumber() == 0){
        std::vector<AssemblyInstruction> loadRight_code = getValueToDestinationAddress(symbolsTable, right, 0);
        result.insert(result.end(), loadRight_code.begin(), loadRight_code.end());
        return result;
    }
    // x + 0 = x
    if(expr->type == ExpressionType::Plus &&  right.isNumber() && right.asNumber() == 0){
        std::vector<AssemblyInstruction> loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
        result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());
        return result;
    }
    // x - x = 0
    if(expr->type == ExpressionType::Minus && left == right){
        result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 0));
        return result;
    }
    // x - 0 = x
    if(expr->type == ExpressionType::Minus && right.isNumber() && right.asNumber() == 0){
        std::vector<AssemblyInstruction> loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
        result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());
        return result;
    }
    // 1 * x = x
    if(expr->type == ExpressionType::Multiply && left.isNumber() && left.asNumber() == 1){
        std::vector<AssemblyInstruction> loadRight_code = getValueToDestinationAddress(symbolsTable, right, 0);
        result.insert(result.end(), loadRight_code.begin(), loadRight_code.end());
        return result;
    }
    // x * 1 = 1
    if(expr->type == ExpressionType::Multiply && right.isNumber() && right.asNumber() == 1){
        std::vector<AssemblyInstruction> loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
        result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());
        return result;
    }
    // x / 1 = x
    if(expr->type == ExpressionType::Divide && right.isNumber() && right.asNumber() == 1){
        std::vector<AssemblyInstruction> loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
        result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());
        return result;
    }
    // x / 0 = 0
    if( expr->type == ExpressionType::Divide && right.isNumber() && right.asNumber() == 0){
        result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 0));
        return result;
    }
    // x % 1 = 0
    if(expr->type == ExpressionType::Modulo && right.isNumber() && right.asNumber() == 1){
        result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 0));
        return result;
    }
    // x % -1 = 0
    if(expr->type == ExpressionType::Modulo && right.isNumber() && right.asNumber() == -1){
        result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 0));
        return result;
    }
    // x % 0 = 0
    if(expr->type == ExpressionType::Modulo && right.isNumber() && right.asNumber() == 0){
        result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 0));
        return result;
    }

    // If at least one is a number load left into the accumulator and right to p[1] and preform the operation
    std::vector<AssemblyInstruction> loadRight_code = getValueToDestinationAddress(symbolsTable, right, 1);
    result.insert(result.end(), loadRight_code.begin(), loadRight_code.end());

    std::vector<AssemblyInstruction> loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
    result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());

    switch(expr->type){
        case ExpressionType::Plus:
            // Add the value of the right expression to the accumulator and store the result
            result.push_back(AssemblyInstruction(AssemblyInstructionType::ADD, 1));            
            return result;
            break;
        case ExpressionType::Minus:
            // Subtract the value of the right expression from the accumulator and store the result
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 1));
            return result;
            break;
        default:
            throw std::runtime_error("Expression not implemented");
    }
}