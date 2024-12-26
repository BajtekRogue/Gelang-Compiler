#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"
#include "compiling.hpp"
#include "utility.hpp"


std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> compileCondition(SymbolsTable& symbolsTable, const std::unique_ptr<Condition>& cond, int64_t jumpAddress){

    std::vector<AssemblyInstruction> code;

    // Grammar ensures that both values are initialized
    Value& left = *(cond->left);
    Value& right = *(cond->right);

    // If both are numbers evaluate the condition during compile time and return the result as second std::optional argument
    if(left.isNumber() && right.isNumber()){

        int64_t leftNum = left.asNumber();
        int64_t rightNum = right.asNumber();
        
        switch(cond->type){
            case ConditionType::Equal:
                return {code, leftNum == rightNum};
            case ConditionType::NotEqual:
                return {code, leftNum != rightNum};
            case ConditionType::GreaterThan:
                return {code, leftNum > rightNum};
            case ConditionType::LessThan:
                return {code, leftNum < rightNum};
            case ConditionType::GreaterEqual:
                return {code, leftNum >= rightNum};
            case ConditionType::LessEqual:
                return {code, leftNum <= rightNum};
            default:
                throw std::runtime_error("Unknown condition type");
        }
    }

    // Validate use of variables
    if(left.isIdentifier()){
        validateUseOfVariable(symbolsTable, left.asIdentifier(), "condition", true);
    }
    if(right.isIdentifier()){
        validateUseOfVariable(symbolsTable, right.asIdentifier(), "condition", true);
    }

    // If both values are the same, we can skip the comparison and return the result as second std::optional argument
    if(left == right){
        switch(cond->type){
            case ConditionType::Equal:
                return {code, true};
            case ConditionType::NotEqual:
                return {code, false};
            case ConditionType::GreaterThan:
                return {code, false};
            case ConditionType::LessThan:
                return {code, false};
            case ConditionType::GreaterEqual:
                return {code, true};
            case ConditionType::LessEqual:
                return {code, true};
            default:
                throw std::runtime_error("Unknown condition type");
        }
    }

    std::vector<AssemblyInstruction> loadRightCode;
    std::vector<AssemblyInstruction> loadLeftCode;

    // If one side of the condition is 0 we can check it more efficiently
    // If left is zero
    if(left.isNumber() && left.asNumber() == 0){

        loadRightCode = getValueToDestinationAddress(symbolsTable, right, 0);
        code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());

        switch(cond->type){
            case ConditionType::Equal:
                // 0 == y  iff  ~(y > 0 AND y < 0)
                code.push_back(AssemblyInstruction(Instruction::JPOS, jumpAddress + 1));
                code.push_back(AssemblyInstruction(Instruction::JNEG, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::NotEqual:
                // 0 != y  iff  ~(y == 0)
                code.push_back(AssemblyInstruction(Instruction::JZERO, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::GreaterThan:
                // 0 > y  iff  ~(y >= 0)
                code.push_back(AssemblyInstruction(Instruction::JPOS, jumpAddress + 1));
                code.push_back(AssemblyInstruction(Instruction::JZERO, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::LessThan:
                // 0 < y  iff  ~(y <= 0)
                code.push_back(AssemblyInstruction(Instruction::JNEG, jumpAddress + 1));
                code.push_back(AssemblyInstruction(Instruction::JZERO, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::GreaterEqual:
                // 0 >= y  iff  ~(y > 0)
                code.push_back(AssemblyInstruction(Instruction::JPOS, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::LessEqual:
                // 0 <= y  iff  ~(y < 0)
                code.push_back(AssemblyInstruction(Instruction::JNEG, jumpAddress));
                return {code, std::nullopt};
                break;
            default:
                throw std::runtime_error("Unknown condition type");
        }
    }

    // If right is zero
    if(right.isNumber() && right.asNumber() == 0){

        loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
        code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());

        switch(cond->type){
            case ConditionType::Equal:
                // x == 0  iff  ~(x > 0 AND x < 0)
                code.push_back(AssemblyInstruction(Instruction::JPOS, jumpAddress + 1));
                code.push_back(AssemblyInstruction(Instruction::JNEG, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::NotEqual:
                // x != 0  iff  ~(x == 0)
                code.push_back(AssemblyInstruction(Instruction::JZERO, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::GreaterThan:
                // x > 0  iff  ~(x <= 0)
                code.push_back(AssemblyInstruction(Instruction::JNEG, jumpAddress + 1));
                code.push_back(AssemblyInstruction(Instruction::JZERO, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::LessThan:
                // x < 0  iff  ~(x >= 0)
                code.push_back(AssemblyInstruction(Instruction::JPOS, jumpAddress + 1));
                code.push_back(AssemblyInstruction(Instruction::JZERO, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::GreaterEqual:
                // x >= 0  iff  ~(x < 0)
                code.push_back(AssemblyInstruction(Instruction::JNEG, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::LessEqual:
                // x <= 0  iff  ~(x > 0)
                code.push_back(AssemblyInstruction(Instruction::JPOS, jumpAddress));
                return {code, std::nullopt};
                break;
            default:
                throw std::runtime_error("Unknown condition type");
        }
    }

    // Check if we can optimize the subtraction
    bool leftMinusRight = false;
    bool rightMinusLeft = false;

    // Check if we can efficiently subtract the the right
    if(right.isIdentifier()){

        Identifier identifier = right.asIdentifier();

        // If right is a variable
        if(symbolsTable.isLocalVariable(identifier.id)){

            int64_t varAddress = symbolsTable.getMemoryAddress_variable(identifier.id);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUB, varAddress));
            leftMinusRight = true;
        }

        // If right is a parameter
        else if(symbolsTable.isParameter(identifier.id) && symbolsTable.getParameterType(identifier.id) == ParameterType::Integer){

            int64_t paramAddress = symbolsTable.getMemoryAddressPointer_parameter(identifier.id);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUBI, paramAddress));
            leftMinusRight = true;
        }

        // If right is a local array accesed by index
        else if(symbolsTable.isArrayDeclared(identifier.id) && identifier.getArrayAccess().isByIndex()){
            int64_t index = identifier.getArrayAccess().getIndex();
            int64_t arrayAddress = symbolsTable.getMemoryAddress_start(identifier.id) + symbolsTable.get_offset(identifier.id);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUB, arrayAddress + index));
            leftMinusRight = true;
        }

        // If right is an iterator
        else if(symbolsTable.isIterator(identifier.id)){
            int64_t iteratorAddress = symbolsTable.getMemoryAddress_variable(identifier.id);
            loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
            code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUB, iteratorAddress));
            leftMinusRight = true;
        }
    }

    // Check if we can efficiently subtract the the left if we haven't already subtracted the right
    if(left.isIdentifier() && !leftMinusRight){

        Identifier identifier = left.asIdentifier();

        // If left is a variable
        if(symbolsTable.isLocalVariable(identifier.id)){

            int64_t varAddress = symbolsTable.getMemoryAddress_variable(identifier.id);
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 0);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUB, varAddress));
            rightMinusLeft = true;
        }

        // If left is a parameter
        else if(symbolsTable.isParameter(identifier.id) && symbolsTable.getParameterType(identifier.id) == ParameterType::Integer){

            int64_t paramAddress = symbolsTable.getMemoryAddressPointer_parameter(identifier.id);
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 0);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUBI, paramAddress));
            rightMinusLeft = true;
        }

        // If left is a local array accesed by index
        else if(symbolsTable.isArrayDeclared(identifier.id) && identifier.getArrayAccess().isByIndex()){
            int64_t index = identifier.getArrayAccess().getIndex();
            int64_t arrayAddress = symbolsTable.getMemoryAddress_start(identifier.id) + symbolsTable.get_offset(identifier.id);
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 0);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUB, arrayAddress + index));
            rightMinusLeft = true;
        }

        // If left is an iterator
        else if(symbolsTable.isIterator(identifier.id)){
            int64_t iteratorAddress = symbolsTable.getMemoryAddress_variable(identifier.id);
            loadRightCode = getValueToDestinationAddress(symbolsTable, right, 0);
            code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
            code.push_back(AssemblyInstruction(Instruction::SUB, iteratorAddress));
            rightMinusLeft = true;
        }
    }

    // Otherwise nothing can be done simply, load the values and perform the comparison
    if(!leftMinusRight && !rightMinusLeft){
        loadRightCode = getValueToDestinationAddress(symbolsTable, right, 1);
        loadLeftCode = getValueToDestinationAddress(symbolsTable, left, 0);
        code.insert(code.end(), loadRightCode.begin(), loadRightCode.end());
        code.insert(code.end(), loadLeftCode.begin(), loadLeftCode.end());
        code.push_back(AssemblyInstruction(Instruction::SUB, 1));
    }

    // If left - right
    if(leftMinusRight){
        switch(cond->type){
            case ConditionType::Equal:
                // x == y  iff  ~(x - y > 0 AND x - y < 0)
                code.push_back(AssemblyInstruction(Instruction::JPOS, jumpAddress + 1));
                code.push_back(AssemblyInstruction(Instruction::JNEG, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::NotEqual:
                // x != y  iff  ~(x - y = 0)
                code.push_back(AssemblyInstruction(Instruction::JZERO, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::GreaterThan:
                // x > y  iff  ~(x - y <= 0)
                code.push_back(AssemblyInstruction(Instruction::JNEG, jumpAddress + 1));
                code.push_back(AssemblyInstruction(Instruction::JZERO, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::LessThan:
                // x < y  iff  ~(x - y >= 0)
                code.push_back(AssemblyInstruction(Instruction::JPOS, jumpAddress + 1));
                code.push_back(AssemblyInstruction(Instruction::JZERO, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::GreaterEqual:
                // x >= y  iff  ~(x - y < 0)
                code.push_back(AssemblyInstruction(Instruction::JNEG, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::LessEqual:
                // x <= y  iff  ~(x - y > 0)
                code.push_back(AssemblyInstruction(Instruction::JPOS, jumpAddress));
                return {code, std::nullopt};
                break;
            default:
                throw std::runtime_error("Unknown condition type");
        }
    }

    // If right - left
    if(rightMinusLeft){
        switch(cond->type){
            case ConditionType::Equal:
                // x == y  iff  ~(y - x > 0 AND y - x < 0)
                code.push_back(AssemblyInstruction(Instruction::JPOS, jumpAddress + 1));
                code.push_back(AssemblyInstruction(Instruction::JNEG, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::NotEqual:
                // x != y  iff  ~(y - x = 0)
                code.push_back(AssemblyInstruction(Instruction::JZERO, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::GreaterThan:
                // x > y  iff  ~(y - x >= 0)
                code.push_back(AssemblyInstruction(Instruction::JPOS, jumpAddress + 1));
                code.push_back(AssemblyInstruction(Instruction::JZERO, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::LessThan:
                // x < y  iff  ~(y - x <= 0)
                code.push_back(AssemblyInstruction(Instruction::JNEG, jumpAddress + 1));
                code.push_back(AssemblyInstruction(Instruction::JZERO, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::GreaterEqual:
                // x >= y  iff  ~(y - x > 0)
                code.push_back(AssemblyInstruction(Instruction::JPOS, jumpAddress));
                return {code, std::nullopt};
                break;
            case ConditionType::LessEqual:
                // x <= y  iff  ~(y - x < 0)
                code.push_back(AssemblyInstruction(Instruction::JNEG, jumpAddress));
                return {code, std::nullopt};
                break;
            default:
                throw std::runtime_error("Unknown condition type");
        }
    }
    return {code, std::nullopt};
}