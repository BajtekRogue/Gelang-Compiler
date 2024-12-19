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

    // If both values are the same, we can skip the comparison and return the result as second std::optional argument
    if(left == right){
        switch(cond->type){
            case ConditionType::Equal:
                return {result, true};
            case ConditionType::NotEqual:
                return {result, false};
            case ConditionType::GreaterThan:
                return {result, false};
            case ConditionType::LessThan:
                return {result, false};
            case ConditionType::GreaterEqual:
                return {result, true};
            case ConditionType::LessEqual:
                return {result, true};
            default:
                throw std::runtime_error("Unknown condition type");
        }
    }

    // Load values to registers to be ready for comparison
    std::vector<AssemblyInstruction> loadRight_code = getValueToDestinationAddress(symbolsTable, right, 1);
    result.insert(result.end(), loadRight_code.begin(), loadRight_code.end());
    std::vector<AssemblyInstruction> loadLeft_code = getValueToDestinationAddress(symbolsTable, left, 0);
    result.insert(result.end(), loadLeft_code.begin(), loadLeft_code.end());

    // accu = x - y
    result.push_back(AssemblyInstruction(AssemblyInstructionType::SUB, 1));

    switch(cond->type){
        case ConditionType::Equal:
            // x = y  iff  ~(x - y > 0 AND x - y < 0)
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JPOS, jumpAddress + 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JNEG, jumpAddress));
            return {result, std::nullopt};
            break;
        case ConditionType::NotEqual:
            // x != y  iff  ~(x - y = 0)
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JZERO, jumpAddress));
            return {result, std::nullopt};
            break;
        case ConditionType::GreaterThan:
            // x > y  iff  ~(x - y <= 0)
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JNEG, jumpAddress + 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JZERO, jumpAddress));
            return {result, std::nullopt};
            break;
        case ConditionType::LessThan:
            // x < y  iff  ~(x - y >= 0)
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JPOS, jumpAddress + 1));
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JZERO, jumpAddress));
            return {result, std::nullopt};
            break;
        case ConditionType::GreaterEqual:
            // x >= y  iff  ~(x - y < 0)
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JNEG, jumpAddress));
            return {result, std::nullopt};
            break;
        case ConditionType::LessEqual:
            // x <= y  iff  ~(x - y > 0)
            result.push_back(AssemblyInstruction(AssemblyInstructionType::JPOS, jumpAddress));
            return {result, std::nullopt};
            break;
        default:
            throw std::runtime_error("Unknown condition type");
    }

}