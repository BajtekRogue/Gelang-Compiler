#include "assembling.hpp"
#include "symbolsTable.hpp"
#include "languageStructs.hpp"

std::pair<std::vector<AssemblyInstruction>, std::optional<bool>> compile_CONDITION(SymbolsTable& symbolsTable, const std::unique_ptr<Condition>& cond, ll jumpAddress){
    std::vector<AssemblyInstruction> result;

    // Grammar ensures that both values are initialized
    Value& left = *(cond->left);
    Value& right = *(cond->right);

    // If both are numbers evaluate the condition during compile time and return the result as second std::optional argument
    if(left.isNumber() && right.isNumber()){

        ll leftNum = left.asNumber();
        ll rightNum = right.asNumber();

        switch(cond->type){
            case ConditionType::Equal:
                return {result, leftNum == rightNum};
            case ConditionType::NotEqual:
                return {result, leftNum != rightNum};
            case ConditionType::GreaterThan:
                return {result, leftNum > rightNum};
            case ConditionType::LessThan:
                return {result, leftNum < rightNum};
            case ConditionType::GreaterEqual:
                return {result, leftNum >= rightNum};
            case ConditionType::LessEqual:
                return {result, leftNum <= rightNum};
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

    std::vector<AssemblyInstruction> loadRight_code;
    std::vector<AssemblyInstruction> loadLeft_code;

    switch(cond->type){
        case ConditionType::Equal:
            loadRight_code = getValueToDestinationAddress(symbolsTable, right, 1);
            result.insert(result.end(), loadRight_code.begin(), loadRight_code.end());

            loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
            result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());

            // x = y  iff  ~(x - y > 0 AND x - y < 0)
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JPOS, jumpAddress + 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JNEG, jumpAddress));

            return {result, std::nullopt};
            break;
        case ConditionType::NotEqual:
            loadRight_code = getValueToDestinationAddress(symbolsTable, right, 1);
            result.insert(result.end(), loadRight_code.begin(), loadRight_code.end());

            loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
            result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());

            // x != y  iff  ~ (x - y = 0)
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JZERO, jumpAddress));

            return {result, std::nullopt};
            break;
        case ConditionType::GreaterThan:
            loadRight_code = getValueToDestinationAddress(symbolsTable, right, 1);
            result.insert(result.end(), loadRight_code.begin(), loadRight_code.end());

            loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
            result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());

            // x > y  iff  ~(x - y <= 0)
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JNEG, jumpAddress + 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JZERO, jumpAddress));

            return {result, std::nullopt};
            break;
        case ConditionType::LessThan:
            loadRight_code = getValueToDestinationAddress(symbolsTable, right, 1);
            result.insert(result.end(), loadRight_code.begin(), loadRight_code.end());

            loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
            result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());

            // x < y  iff  ~(x - y >= 0)
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JPOS, jumpAddress + 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JZERO, jumpAddress));

            return {result, std::nullopt};
            break;
        case ConditionType::GreaterEqual:
            loadRight_code = getValueToDestinationAddress(symbolsTable, right, 1);
            result.insert(result.end(), loadRight_code.begin(), loadRight_code.end());

            loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
            result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());

            // x >= y  iff  ~(x - y < 0)
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JNEG, jumpAddress));

            return {result, std::nullopt};
            break;
        case ConditionType::LessEqual:
            loadRight_code = getValueToDestinationAddress(symbolsTable, right, 1);
            result.insert(result.end(), loadRight_code.begin(), loadRight_code.end());

            loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
            result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());

            // x <= y  iff  ~(x - y > 0)
            result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JPOS, jumpAddress));

            return {result, std::nullopt};
            break;
        default:
            throw std::runtime_error("Unknown condition type");
    }

}