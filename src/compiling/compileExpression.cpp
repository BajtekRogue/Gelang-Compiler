#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utility.hpp"


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
    // 0 / x = 0
    if(expr->type == ExpressionType::Divide && left.isNumber() && left.asNumber() == 0){
        code.push_back(AssemblyInstruction(Instruction::SUB, 0));
        return code;
    }
    // 0 % x = 0
    if(expr->type == ExpressionType::Modulo && left.isNumber() && left.asNumber() == 0){
        code.push_back(AssemblyInstruction(Instruction::SUB, 0));
        return code;
    }

    // Check if addition can be done efficiently
    if(expr->type == ExpressionType::Plus && left.isIdentifier()){

        Identifier identifier = left.asIdentifier();

        // If left is a variable
        if(symbolsTable.isLocalVariable(identifier.id)){

            int64_t varAddress = symbolsTable.getMemoryAddress_variable(identifier.id);
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 0);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.push_back(AssemblyInstruction(Instruction::ADD, varAddress));

            return code;
        }

        // If left is a parameter
        if(symbolsTable.isParameter(identifier.id) && symbolsTable.getParameterType(identifier.id) == ParameterType::Integer){

            int64_t paramAddress = symbolsTable.getMemoryAddressPointer_parameter(identifier.id);
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 0);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.push_back(AssemblyInstruction(Instruction::ADDI, paramAddress));

            return code;
        }

        // If left is a local array accesed by index
        if(symbolsTable.isArrayDeclared(identifier.id) && identifier.getArrayAccess().isByIndex()){
            int64_t index = identifier.getArrayAccess().getIndex();
            int64_t arrayAddress = symbolsTable.getMemoryAddress_start(identifier.id) + symbolsTable.get_offset(identifier.id);
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 0);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.push_back(AssemblyInstruction(Instruction::ADD, arrayAddress + index));

            return code;
        }

        // If left is an iterator
        if(symbolsTable.isIterator(identifier.id)){
            int64_t iteratorAddress = symbolsTable.getMemoryAddress_variable(identifier.id);
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 0);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.push_back(AssemblyInstruction(Instruction::ADD, iteratorAddress));

            return code;
        }
    }

    if(expr->type == ExpressionType::Plus && right.isIdentifier()){

        Identifier identifier = right.asIdentifier();

        // If right is a variable
        if(symbolsTable.isLocalVariable(identifier.id)){

            int64_t varAddress = symbolsTable.getMemoryAddress_variable(identifier.id);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::ADD, varAddress));

            return code;
        }

        // If right is a parameter
        if(symbolsTable.isParameter(identifier.id) && symbolsTable.getParameterType(identifier.id) == ParameterType::Integer){

            int64_t paramAddress = symbolsTable.getMemoryAddressPointer_parameter(identifier.id);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::ADDI, paramAddress));

            return code;
        }

        // If left is a local array accesed by index
        if(symbolsTable.isArrayDeclared(identifier.id) && identifier.getArrayAccess().isByIndex()){
            int64_t index = identifier.getArrayAccess().getIndex();
            int64_t arrayAddress = symbolsTable.getMemoryAddress_start(identifier.id) + symbolsTable.get_offset(identifier.id);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::ADD, arrayAddress + index));

            return code;
        }

        // If right is an iterator
        if(symbolsTable.isIterator(identifier.id)){
            int64_t iteratorAddress = symbolsTable.getMemoryAddress_variable(identifier.id);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::ADD, iteratorAddress));

            return code;
        }
    }

    // Check if subtraction can be done efficiently
    if(expr->type == ExpressionType::Minus && right.isIdentifier()){

        Identifier identifier = right.asIdentifier();

        // If right is a variable
        if(symbolsTable.isLocalVariable(identifier.id)){

            int64_t varAddress = symbolsTable.getMemoryAddress_variable(identifier.id);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUB, varAddress));

            return code;
        }

        // If right is a parameter
        if(symbolsTable.isParameter(identifier.id) && symbolsTable.getParameterType(identifier.id) == ParameterType::Integer){

            int64_t paramAddress = symbolsTable.getMemoryAddressPointer_parameter(identifier.id);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUBI, paramAddress));

            return code;
        }

        // If right is a local array accesed by index
        if(symbolsTable.isArrayDeclared(identifier.id) && identifier.getArrayAccess().isByIndex()){
            int64_t index = identifier.getArrayAccess().getIndex();
            int64_t arrayAddress = symbolsTable.getMemoryAddress_start(identifier.id) + symbolsTable.get_offset(identifier.id);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUB, arrayAddress + index));

            return code;
        }

        // If right is an iterator
        if(symbolsTable.isIterator(identifier.id)){
            int64_t iteratorAddress = symbolsTable.getMemoryAddress_variable(identifier.id);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUB, iteratorAddress));

            return code;
        }
    }

    // Otherwise nothing can be done simply, load the values and perform the operation
    switch(expr->type){
        case ExpressionType::Plus:
            // Add the values
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 1);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::ADD, 1));            
            return code;
            break;
        case ExpressionType::Minus:
            // Subtract the values
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 1);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUB, 1));
            return code;
            break;
        case ExpressionType::Multiply:
            // Mark it and load the arguments
            Arithmetic::multiplication = true;
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, Memory::multiplicationRight);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, Memory::multiplicationLeft);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());

            // Jump to the multiplication procedure
            code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Setting return address and jumping to " + Arithmetic::multiplicationProcedureName));
            code.push_back(AssemblyInstruction(Instruction::SET, Arithmetic::multiplicationProcedureName));
            code.push_back(AssemblyInstruction(Instruction::STORE, Memory::returnMultiplication));
            code.push_back(AssemblyInstruction(Instruction::JUMP, Arithmetic::multiplicationProcedureName));
            return code;
            break;
        case ExpressionType::Divide:
            // Mark it and load the arguments
            Arithmetic::division = true;
            Arithmetic::isOneNeeded = true;
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, Memory::divisionRight);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, Memory::divisionLeft);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());

            // Jump to the division procedure
            code.push_back(AssemblyInstruction(Instruction::SUB, 0));
            code.push_back(AssemblyInstruction(Instruction::STORE, Memory::divisionFlag));
            code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Setting return address and jumping to " + Arithmetic::divisionProcedureName));
            code.push_back(AssemblyInstruction(Instruction::SET, Arithmetic::divisionProcedureName));
            code.push_back(AssemblyInstruction(Instruction::STORE, Memory::returnDivision));
            code.push_back(AssemblyInstruction(Instruction::JUMP, Arithmetic::divisionProcedureName));
            return code;
            break;
        case ExpressionType::Modulo:
            // Mark it and load the arguments
            Arithmetic::division = true;
            Arithmetic::isOneNeeded = true;
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, Memory::divisionRight);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, Memory::divisionLeft);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());

            // Jump to the division procedure
            code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::one));
            code.push_back(AssemblyInstruction(Instruction::STORE, Memory::divisionFlag));
            code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Setting return address and jumping to " + Arithmetic::divisionProcedureName));
            code.push_back(AssemblyInstruction(Instruction::SET, Arithmetic::divisionProcedureName));
            code.push_back(AssemblyInstruction(Instruction::STORE, Memory::returnDivision));
            code.push_back(AssemblyInstruction(Instruction::JUMP, Arithmetic::divisionProcedureName));
            return code;
            break;
        default:
            throw std::runtime_error("Expression not implemented");
    }
    return code;
}