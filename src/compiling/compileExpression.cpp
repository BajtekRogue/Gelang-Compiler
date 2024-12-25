#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utlity.hpp"


std::vector<AssemblyInstruction> compileExpression(SymbolsTable& symbolsTable, const std::unique_ptr<Expression>& expr){

    std::vector<AssemblyInstruction> code;

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
        
        int64_t leftNum = left.asNumber();
        int64_t rightNum = right.asNumber();
        int64_t resultNum = 0;

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
            code.push_back(AssemblyInstruction(Instruction::SUB, 0));
        }
        else{
            code.push_back(AssemblyInstruction(Instruction::SET, resultNum));
        }
        return code;
    }

    // Check if the variables are declared and initialized before arithmetic operations
    if(left.isIdentifier()){
        validateUseOfVariable(symbolsTable, left.asIdentifier(), "expression", true);
    }
    if(right.isIdentifier()){
        validateUseOfVariable(symbolsTable, right.asIdentifier(), "expression", true);
    }

    std::vector<AssemblyInstruction> loadRightCode;
    std::vector<AssemblyInstruction> loadLeftCode;

    // Check special cases of arithmetic identities
    // 0 + x = x
    if(expr->type == ExpressionType::Plus && left.isNumber() && left.asNumber() == 0){
        loadRightCode = getValueToDestinationAddress(symbolsTable, right, 0);
        code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
        return code;
    }
    // x + 0 = x
    if(expr->type == ExpressionType::Plus &&  right.isNumber() && right.asNumber() == 0){
        loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
        code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
        return code;
    }
    // x - x = 0
    if(expr->type == ExpressionType::Minus && left == right){
        code.push_back(AssemblyInstruction(Instruction::SUB, 0));
        return code;
    }
    // x - 0 = x
    if(expr->type == ExpressionType::Minus && right.isNumber() && right.asNumber() == 0){
        loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
        code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
        return code;
    }
    // 1 * x = x
    if(expr->type == ExpressionType::Multiply && left.isNumber() && left.asNumber() == 1){
        loadRightCode = getValueToDestinationAddress(symbolsTable, right, 0);
        code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
        return code;
    }
    // x * 1 = 1
    if(expr->type == ExpressionType::Multiply && right.isNumber() && right.asNumber() == 1){
        loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
        code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
        return code;
    }
    // x / 1 = x
    if(expr->type == ExpressionType::Divide && right.isNumber() && right.asNumber() == 1){
        loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
        code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
        return code;
    }
    // x / 0 = 0
    if( expr->type == ExpressionType::Divide && right.isNumber() && right.asNumber() == 0){
        code.push_back(AssemblyInstruction(Instruction::SUB, 0));
        return code;
    }
    // x % 1 = 0
    if(expr->type == ExpressionType::Modulo && right.isNumber() && right.asNumber() == 1){
        code.push_back(AssemblyInstruction(Instruction::SUB, 0));
        return code;
    }
    // x % -1 = 0
    if(expr->type == ExpressionType::Modulo && right.isNumber() && right.asNumber() == -1){
        code.push_back(AssemblyInstruction(Instruction::SUB, 0));
        return code;
    }
    // x % 0 = 0
    if(expr->type == ExpressionType::Modulo && right.isNumber() && right.asNumber() == 0){
        code.push_back(AssemblyInstruction(Instruction::SUB, 0));
        return code;
    }
    // 2 * x = x + x
    if(expr->type == ExpressionType::Multiply && left.isNumber() && left.asNumber() == 2){
        loadRightCode = getValueToDestinationAddress(symbolsTable, right, 0);
        code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
        code.push_back(AssemblyInstruction(Instruction::ADD, 0));
        return code;
    }
    // x * 2 = x + x
    if(expr->type == ExpressionType::Multiply && right.isNumber() && right.asNumber() == 2){
        loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
        code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
        code.push_back(AssemblyInstruction(Instruction::ADD, 0));
        return code;
    }
    // x / 2 can be done be HALF
    if(expr->type == ExpressionType::Divide && right.isNumber() && right.asNumber() == 2){
        loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
        code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
        code.push_back(AssemblyInstruction(Instruction::HALF, 0));
        return code;
    }
    // x % 2 = x - 2 * (x / 2)
    if(expr->type == ExpressionType::Modulo && right.isNumber() && right.asNumber() == 2){
        loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
        code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
        code.push_back(AssemblyInstruction(Instruction::STORE, 1));
        code.push_back(AssemblyInstruction(Instruction::HALF, 0));
        code.push_back(AssemblyInstruction(Instruction::ADD, 0));
        code.push_back(AssemblyInstruction(Instruction::STORE, 2));
        code.push_back(AssemblyInstruction(Instruction::LOAD, 1));
        code.push_back(AssemblyInstruction(Instruction::SUB, 2));
        return code;
    }

    // // Check if addition can be done efficiently
    // if(expr->type == ExpressionType::Plus){
        // TO DO
    // }

    switch(expr->type){
        case ExpressionType::Plus:
            // Add the value of the right expression to the accumulator and store the result
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 1);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::ADD, 1));            
            return code;
            break;
        case ExpressionType::Minus:
            // Subtract the value of the right expression from the accumulator and store the result
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 1);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUB, 1));
            return code;
            break;
        case ExpressionType::Multiply:
            // Jump to the multiplication procedure
            Arithmetic::multiplication = true;
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 2);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 1);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Setting return address and jumping to *"));
            code.push_back(AssemblyInstruction(Instruction::SET, "*"));
            code.push_back(AssemblyInstruction(Instruction::STORE, MEMORY_RETURN_MULTIPLICATION));
            code.push_back(AssemblyInstruction(Instruction::JUMP, "*"));
            return code;
            break;
        case ExpressionType::Divide:
            // Jump to the division procedure
            Arithmetic::division = true;
            code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Setting return address and jumping to /"));
            code.push_back(AssemblyInstruction(Instruction::SET, "/"));
            code.push_back(AssemblyInstruction(Instruction::STORE, MEMORY_RETURN_DIVISION));
            code.push_back(AssemblyInstruction(Instruction::JUMP, "/"));
            return code;
            break;
        case ExpressionType::Modulo:
            // Jump to the modulo procedure
            Arithmetic::modulo = true;
            code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Setting return address and jumping to %"));
            code.push_back(AssemblyInstruction(Instruction::SET, "%"));
            code.push_back(AssemblyInstruction(Instruction::STORE, MEMORY_RETURN_MODULO));
            code.push_back(AssemblyInstruction(Instruction::JUMP, "%"));
            return code;
            break;
        default:
            throw std::runtime_error("Expression not implemented");
    }
    return code;
}