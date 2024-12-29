#ifndef LANGUAGE_STRUCTS_HPP
#define LANGUAGE_STRUCTS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <variant>
#include <optional>
#include <cinttypes>


enum class ExpressionType {
    Value,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo
};

enum class CommandType {
    Assign,
    IfElse,
    If,
    While,
    Repeat,
    ForTo,
    ForDownto,
    ProcedureCall,
    Read,
    Write
};

enum class ConditionType {
    Equal,
    NotEqual,
    GreaterThan,
    LessThan,
    GreaterEqual,
    LessEqual
};

enum class VariableType {
    Integer,
    Array
};

enum class ParameterType {
    Integer,
    Array
};



struct Variable {
    VariableType type;
    std::string identifier;
    std::optional<std::pair<int64_t, int64_t>> arrayBounds;  // Only used for array declarations

    // Constructor for integer variable
    explicit Variable(const std::string& id) 
        : type(VariableType::Integer), identifier(id) {}

    // Constructor for array variable with bounds
    Variable(const std::string& id, int64_t start, int64_t end) 
        : type(VariableType::Array), 
          identifier(id), 
          arrayBounds(std::make_pair(start, end)) {}
    
    bool isArray() const { 
        return type == VariableType::Array; 
    }

    bool hasBounds() const { 
        return arrayBounds.has_value(); 
    }

    // Utility method to get bounds, throws if no bounds
    std::pair<int64_t, int64_t> getBounds() const {
        if (!hasBounds()) {
            throw std::runtime_error("No array bounds specified");
        }
        return *arrayBounds;
    }

    std::string toString() const {
        std::string result = identifier;
        if (isArray()) {
            auto bounds = getBounds();
            result += "[" + std::to_string(bounds.first) + ":" + std::to_string(bounds.second) + "]";
        }
        return result;
    }

};

struct Parameter {
    ParameterType type;
    std::string identifier;

    // Constructor for integer parameter
    explicit Parameter(const std::string& id) 
        : type(ParameterType::Integer), identifier(id) {}

    // Constructor for array parameter (marked with 'T')
    Parameter(const std::string& id, bool) 
        : type(ParameterType::Array), identifier(id) {}

    bool isArray() const { 
        return type == ParameterType::Array; 
    }

    bool isInteger() const { 
        return type == ParameterType::Integer; 
    }

    std::string toString() const {
        std::string result = identifier;
        if (isArray()) {
            result += " (Array)";
        } else {
            result += " (Integer)";
        }
        return result;
    }
};

// Represents an array access
struct ArrayAccess {
    std::string identifier;
    std::variant<std::string, int64_t> index;

    ArrayAccess(const std::string& id, const std::string& idx) 
        : identifier(id), index(idx) {}
    
    ArrayAccess(const std::string& id, int64_t idx) 
        : identifier(id), index(idx) {}

    bool isByIndex() const { 
        return std::holds_alternative<int64_t>(index); 
    }

    bool isByVariable() const { 
        return std::holds_alternative<std::string>(index); 
    }

    int64_t getIndex() const { 
        if(!isByIndex()){
            throw std::runtime_error("Array " + identifier + " is accessed by variable " + std::get<std::string>(index));
        }
        return std::get<int64_t>(index); 
    }

    std::string getIndexVariable() const { 
        if(!isByVariable()){
            throw std::runtime_error("Array " + identifier + " is accessed by index " + std::to_string(std::get<int64_t>(index)));
        }
        return std::get<std::string>(index); 
    }

    std::string toString() const {
        std::string result = identifier + "[";
        if (std::holds_alternative<int64_t>(index)) {
            result += std::to_string(std::get<int64_t>(index));
        } else {
            result += std::get<std::string>(index);
        }
        result += "]";
        return result;
    }

    bool operator==(const ArrayAccess& other) const {
        return identifier == other.identifier && index == other.index;
    }

};

struct Identifier{
    std::string id;
    std::optional<ArrayAccess> arrayAccess;

    Identifier(const std::string& id) 
        : id(id) {}

    Identifier(const std::string& id, const ArrayAccess& access)
        : id(id), arrayAccess(access) {}

    bool isVariable() const { 
        return !arrayAccess.has_value(); 
    }

    bool isArray() const { 
        return arrayAccess.has_value(); 
    }

    ArrayAccess getArrayAccess() const {
        if (!arrayAccess.has_value()) {
            throw std::runtime_error("Identifier is not an array");
        }
        return arrayAccess.value();
    }

    std::string toString() const {
        if (arrayAccess.has_value()) {
            return arrayAccess.value().toString();
        } else {
            return id;
        }
    }

    bool operator==(const Identifier& other) const {
        if (id != other.id) {
            return false;
        }else if (arrayAccess.has_value() != other.arrayAccess.has_value()) {
            return false;
        }else if (arrayAccess.has_value() && other.arrayAccess.has_value()) {
            return arrayAccess.value() == other.arrayAccess.value();
        }else{
            return true;
        }
    }
};

// Represents a value - can be a number or an identifier
struct Value {
    std::variant<int64_t, Identifier> data;

    Value(int64_t val) : data(val) {}
    Value(const Identifier& id) : data(Identifier(id)) {}

    bool isNumber() const { 
        return std::holds_alternative<int64_t>(data); 
    }

    bool isIdentifier() const { 
        return std::holds_alternative<Identifier>(data); 
    }

    int64_t asNumber() const { 
        if(!isNumber()){
            throw std::runtime_error("Value is not a number");
        }
        return std::get<int64_t>(data); 
    }

    Identifier asIdentifier() const { 
        if(!isIdentifier()){
            throw std::runtime_error("Value is not an identifier but a number " + std::to_string(std::get<int64_t>(data)));
        }
        return std::get<Identifier>(data); 
    }

    std::string toString() const {
        if (isNumber()) {
            return std::to_string(asNumber());
        } else {
            return asIdentifier().toString();
        }
    }

    bool operator==(const Value& other) const {
        return data == other.data;
    }
};


// Represents an expression
struct Expression {
    ExpressionType type;
    std::unique_ptr<Value> left;
    std::unique_ptr<Value> right;

    Expression() 
        : type(ExpressionType::Value), left(nullptr), right(nullptr) {}

    Expression(ExpressionType t, Value* l, Value* r)
        : type(t), left(l), right(r) {}

    Expression(Value* val) 
        : type(ExpressionType::Value), left(val), right(nullptr) {}

    std::string toString() const {
        std::string result;

        if(left){
            result += left->toString();
        }

        switch (type) {
            case ExpressionType::Value:
                return result;
                break;
            case ExpressionType::Plus:
                result += " + ";
                break;
            case ExpressionType::Minus:
                result += " - ";
                break;
            case ExpressionType::Multiply:
                result += " * ";
                break;
            case ExpressionType::Divide:
                result += " / ";
                break;
            case ExpressionType::Modulo:
                result += " % ";
                break;
        }

        if(right){
            result += right->toString();
        }
        return result;
    }
};

// Represents a condition
struct Condition {
    ConditionType type;
    std::unique_ptr<Value> left;
    std::unique_ptr<Value> right;

    Condition() 
        : type(ConditionType::Equal), left(nullptr), right(nullptr) {}

    Condition(ConditionType t, Value* l, Value* r)
        : type(t), left(l), right(r) {}

    std::string toString() const {
        std::string result;

        if(left){
            result += left->toString();
        }

        switch (type) {
            case ConditionType::Equal:
                result += " = ";
                break;
            case ConditionType::NotEqual:
                result += " != ";
                break;
            case ConditionType::GreaterThan:
                result += " > ";
                break;
            case ConditionType::LessThan:
                result += " < ";
                break;
            case ConditionType::GreaterEqual:
                result += " >= ";
                break;
            case ConditionType::LessEqual:
                result += " <= ";
                break;
        }

        if(right){
            result += right->toString();
        }
        return result;
    }

    void negate(){
        switch (type) {
            case ConditionType::Equal:
                type = ConditionType::NotEqual;
                break;
            case ConditionType::NotEqual:
                type = ConditionType::Equal;
                break;
            case ConditionType::GreaterThan:
                type = ConditionType::LessEqual;
                break;
            case ConditionType::LessThan:
                type = ConditionType::GreaterEqual;
                break;
            case ConditionType::GreaterEqual:
                type = ConditionType::LessThan;
                break;
            case ConditionType::LessEqual:
                type = ConditionType::GreaterThan;
                break;
        }
    }
};

// Base Command structure
struct Command {
    CommandType type;
    int lineNumber;

    Command(CommandType t) : type(t) {}
    virtual ~Command() = default;
    virtual std::string toString() const = 0;
    
};

// Assignment Command
struct AssignCommand : public Command {
    std::unique_ptr<Identifier> identifier;
    std::unique_ptr<Expression> expression;
    
    AssignCommand() : Command(CommandType::Assign) {}

    std::string toString() const override{
        std::string result;
        if(identifier){
            result += identifier->toString();
        }
        result += " :=  ";
        if(expression){
            result += expression->toString();
        }
        return result;
    }
};

// Conditional Commands
struct IfCommand : public Command {
    std::unique_ptr<Condition> condition;
    std::vector<std::unique_ptr<Command>> thenCommands;

    IfCommand() : Command(CommandType::If) {}

    std::string toString() const override{
        std::string result = "IF ";
        if(condition){
            result += condition->toString();
        }
        result += " THEN\n";
        for (const auto& cmd : thenCommands) {
            result += cmd->toString();
            result += "\n";
        }
        result += "ENDIF ";
        return result;
    }
};

struct IfElseCommand : public Command {
    std::unique_ptr<Condition> condition;
    std::vector<std::unique_ptr<Command>> thenCommands;
    std::vector<std::unique_ptr<Command>> elseCommands;

    IfElseCommand() : Command(CommandType::IfElse) {}

    std::string toString() const override{
        std::string result = "IF ";
        if(condition){
            result += condition->toString();
        }
        result += " THEN\n";
        for (const auto& cmd : thenCommands) {
            result += cmd->toString();
            result += "\n";
        }
        result += "ELSE\n";
        for (const auto& cmd : elseCommands) {
            result += cmd->toString();
            result += "\n";
        }
        result += "ENDIF ";
        return result;
    }
};

// Loop Commands
struct WhileCommand : public Command {
    std::unique_ptr<Condition> condition;
    std::vector<std::unique_ptr<Command>> commands;

    WhileCommand() : Command(CommandType::While) {}

    std::string toString() const override{
        std::string result = "WHILE ";
        if(condition){
            result += condition->toString();
        }
        result += " DO\n";
        for (const auto& cmd : commands) {
            result += cmd->toString();
            result += "\n";
        }
        result += "ENDWHILE ";
        return result;
    }
};

struct RepeatCommand : public Command {
    std::vector<std::unique_ptr<Command>> commands;
    std::unique_ptr<Condition> condition;

    RepeatCommand() : Command(CommandType::Repeat) {}

    std::string toString() const override{
        std::string result = "REPEAT\n";
        for (const auto& cmd : commands) {
            result += cmd->toString();
            result += "\n";
        }
        result += "UNTIL ";
        if(condition){
            result += condition->toString();
        }
        return result;
    }
};

struct ForToCommand : public Command {
    std::string iterator;
    std::unique_ptr<Value> fromValue;
    std::unique_ptr<Value> toValue;
    std::vector<std::unique_ptr<Command>> commands;

    ForToCommand() : Command(CommandType::ForTo) {}

    std::string bounds() const {
        return fromValue->toString() + " TO " + toValue->toString();
    }

    std::string toString() const override{
        std::string result = "FOR ";
        if(!iterator.empty()){
            result += iterator + " ";
        }
        result += "FROM ";
        if(fromValue){
            result += fromValue->toString();
        }
        result += " TO ";
        if(toValue){
            result += toValue->toString();
        }
        result += " DO\n";
        for (const auto& cmd : commands) {
            result += cmd->toString();
            result += "\n";
        }
        result += "ENDFOR ";
        return result;
    }
};

struct ForDowntoCommand : public Command {
    std::string iterator;
    std::unique_ptr<Value> fromValue;
    std::unique_ptr<Value> downtoValue;
    std::vector<std::unique_ptr<Command>> commands;

    ForDowntoCommand() : Command(CommandType::ForDownto) {}

    std::string bounds() const {
        return fromValue->toString() + " DOWNTO " + downtoValue->toString();
    }

    std::string toString() const override{
        std::string result = "FOR ";
        if(!iterator.empty()){
            result += iterator + " ";
        }
        result += "FROM ";
        if(fromValue){
            result += fromValue->toString();
        }
        result += " DOWNTO ";
        if(downtoValue){
            result += downtoValue->toString();
        }
        result += " DO\n";
        for (const auto& cmd : commands) {
            result += cmd->toString();
            result += "\n";
        }
        result += "ENDFOR ";
        return result;
    }
};

// Procedure-related Commands
struct ProcedureCallCommand : public Command {
    std::string identifier;
    std::vector<std::unique_ptr<Value>> arguments;

    ProcedureCallCommand() : Command(CommandType::ProcedureCall) {}

    std::string toString() const override{
        std::string result = "CALL " + identifier + " (";
        for (const auto& arg : arguments) {
            result += arg->toString();
            result += ", ";
        }
        if (!arguments.empty()) {
            result.pop_back();
            result.pop_back();
        }
        result += ")";
        return result;
    }
};

// I/O Commands
struct ReadCommand : public Command {
    std::unique_ptr<Identifier> identifier;

    ReadCommand() : Command(CommandType::Read) {}

    std::string toString() const override{
        std::string result = "READ ";
        if(identifier){
            result += identifier->toString();
        }
        return result;
    }
};

struct WriteCommand : public Command {
    std::unique_ptr<Value> value;

    WriteCommand() : Command(CommandType::Write) {}

    std::string toString() const override{
        std::string result = "WRITE ";
        if(value){
            result += value->toString();
        }
        return result;
    }
};


// Represents a procedure declaration
struct Procedure {
    std::string identifier;
    std::vector<std::unique_ptr<Parameter>> parameters;
    std::vector<std::unique_ptr<Variable>> declarations;  
    std::vector<std::unique_ptr<Command>> commands;
    int lineNumber;

    std::vector<ParameterType> getParameterTypes() const {
        std::vector<ParameterType> result;
        for (const auto& param : parameters) {
            result.push_back(param->type);
        }
        return result;
    }

    std::string toString() const {
        std::string result = "PROCEDURE: " + identifier + "\n";
        result += "Parameters:\n";
        for (const auto& param : parameters) {
            result += param->toString();
            result += "\n";
        }
        result += "Declarations:\n";
        for (const auto& decl : declarations) {
            result += decl->toString();
            result += "\n";
        }
        result += "Commands:\n";
        for (const auto& cmd : commands) {
            result += cmd->toString();
            result += "\n";
        }
        return result;
    }
};


struct Main{
    std::vector<std::unique_ptr<Variable>> declarations;
    std::vector<std::unique_ptr<Command>> commands;
    int lineNumber;

    std::string toString() const{
        std::string result = "MAIN: \n";
        result += "Declarations:\n";
        for (const auto& decl : declarations) {
            result += decl->toString();
            result += "\n";
        }
        result += "Commands:\n";
        for (const auto& cmd : commands) {
            result += cmd->toString();
            result += "\n";
        }
        return result;
    }
};

// Represents the entire program
struct Program {
    std::vector<std::unique_ptr<Procedure>> procedures;
    std::vector<std::unique_ptr<Variable>> declarations;  
    std::vector<std::unique_ptr<Command>> mainCommands;
    int lineNumber;

    std::string toString() const{
        std::string result = "PROGRAM:\nProcedures:\n";
        for (const auto& proc : procedures) {
            result += proc->toString();
            result += "\n";
        }
        result += "MAIN Declarations:\n";
        for (const auto& decl : declarations) {
            result += decl->toString();
            result += "\n";
        }
        result += "MAIN Commands:\n";
        for (const auto& cmd : mainCommands) {
            result += cmd->toString();
            result += "\n";
        }
        return result;
    }
};

#endif // LANGUAGE_STRUCTS_H