#ifndef LANGUAGE_STRUCTS_HPP
#define LANGUAGE_STRUCTS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <variant>
#include <optional>

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
    std::optional<std::pair<int, int>> arrayBounds;  // Only used for array declarations

    // Constructor for integer variable
    explicit Variable(const std::string& id) 
        : type(VariableType::Integer), identifier(id) {}

    // Constructor for array variable with bounds
    Variable(const std::string& id, int start, int end) 
        : type(VariableType::Array), 
          identifier(id), 
          arrayBounds(std::make_pair(start, end)) {}
    
    bool isArray() const { return type == VariableType::Array; }
    bool hasBounds() const { return arrayBounds.has_value(); }

    // Utility method to get bounds, throws if no bounds
    std::pair<int, int> getBounds() const {
        if (!hasBounds()) {
            throw std::runtime_error("No array bounds specified");
        }
        return *arrayBounds;
    }

    void print() const {
        std::cout << identifier;
        if (isArray()) {
            auto bounds = getBounds();
            std::cout << "[" << bounds.first << ":" << bounds.second << "]";
        }
        std::cout << " ";
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

    bool isArray() const { return type == ParameterType::Array; }
    bool isInteger() const { return type == ParameterType::Integer; }

    void print() const {
        std::cout <<  identifier;
        if (isArray()) {
            std::cout << " (Array)";
        } else {
            std::cout << " (Integer)";
        }
        std::cout << " ";
    }
};

// Represents an array access
struct ArrayAccess {
    std::string identifier;
    std::variant<std::string, long long> index;

    ArrayAccess(const std::string& id, const std::string& idx) 
        : identifier(id), index(idx) {}
    
    ArrayAccess(const std::string& id, long long idx) 
        : identifier(id), index(idx) {}

    void print() const {
        std::cout << identifier << "[";
        if (std::holds_alternative<long long>(index)) {
            std::cout << std::get<long long>(index);
        } else {
            std::cout << std::get<std::string>(index);
        }
        std::cout << "] ";
    }
};

struct Identifier{
    std::string id;
    std::optional<ArrayAccess> arrayAccess;

    Identifier(const std::string& id) 
        : id(id) {}

    Identifier(const std::string& id, const ArrayAccess& access)
        : id(id), arrayAccess(access) {}

    void print() const {
        if(arrayAccess.has_value()){
            arrayAccess.value().print();
        } else {
            std::cout << id;
        }
    }

};

// Represents a value - can be a number or an identifier
struct Value {
    std::variant<long long, Identifier> data;

    Value(long long val) : data(val) {}
    Value(const Identifier& id) : data(Identifier(id)) {}

    bool isNumber() const { return std::holds_alternative<long long>(data); }
    bool isIdentifier() const { return std::holds_alternative<Identifier>(data); }

    long long asNumber() const { return std::get<long long>(data); }
    Identifier asIdentifier() const { return std::get<Identifier>(data); }

    void print() const {
        if (isNumber()) {
            std::cout << asNumber() << " ";
        } else {
            asIdentifier().print();
        }
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

    void print() const {
        switch (type) {
            case ExpressionType::Value:
                if(left) {
                    left->print();
                }
                break;
            case ExpressionType::Plus:
                if(left) {
                    left->print();
                }
                std::cout << " +  ";
                if(right){
                    right->print();
                }
                break;
            case ExpressionType::Minus:
                if(left) {
                    left->print();
                }
                std::cout << " -  ";
                if(right){
                    right->print();
                }
                break;
            case ExpressionType::Multiply:
                if(left) {
                    left->print();
                }
                std::cout << " *  ";
                if(right){
                    right->print();
                }
                break;
            case ExpressionType::Divide:
                if(left) {
                    left->print();
                }
                std::cout << " /  ";
                if(right){
                    right->print();
                }
                break;
            case ExpressionType::Modulo:
                if(left) {
                    left->print();
                }
                std::cout << " %  ";
                if(right){
                    right->print();
                }
                break;
        }
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

    void print() const {
        if(left) {
            left->print();
        }
        switch (type) {
            case ConditionType::Equal:
                std::cout << " ==  ";
                break;
            case ConditionType::NotEqual:
                std::cout << " !=  ";
                break;
            case ConditionType::GreaterThan:
                std::cout << " >  ";
                break;
            case ConditionType::LessThan:
                std::cout << " <  ";
                break;
            case ConditionType::GreaterEqual:
                std::cout << " >=  ";
                break;
            case ConditionType::LessEqual:
                std::cout << " <=  ";
                break;
        }
        if(right){
            right->print();
        }
    }
};

// Base Command structure
struct Command {
    CommandType type;
    
    Command(CommandType t) : type(t) {}
    virtual ~Command() = default;
    virtual void print() const = 0;
    
};

// Assignment Command
struct AssignCommand : public Command {
    std::unique_ptr<Identifier> identifier;
    std::unique_ptr<Expression> expression;
    
    AssignCommand() : Command(CommandType::Assign) {}

    void print() const override{
        if(identifier){
            identifier->print();
        }
        std::cout << " :=  ";
        if(expression){
            expression->print();
        }
        std::cout << " ";
    }
};

// Conditional Commands
struct IfCommand : public Command {
    std::unique_ptr<Condition> condition;
    std::vector<std::unique_ptr<Command>> thenCommands;

    IfCommand() : Command(CommandType::If) {}

    void print() const override{
        std::cout << "If ";
        if(condition){
            condition->print();
        }
        std::cout << " then\n";
        for (const auto& cmd : thenCommands) {
            cmd->print();
            std::cout << "\n";
        }
        std::cout << "EndIf ";
    }
};

struct IfElseCommand : public Command {
    std::unique_ptr<Condition> condition;
    std::vector<std::unique_ptr<Command>> thenCommands;
    std::vector<std::unique_ptr<Command>> elseCommands;

    IfElseCommand() : Command(CommandType::IfElse) {}

    void print() const override{
        std::cout << "If ";
        if(condition){
            condition->print();
        }
        std::cout << " then\n";
        for (const auto& cmd : thenCommands) {
            cmd->print();
            std::cout << "\n";
        }
        std::cout << "Else\n";
        for (const auto& cmd : elseCommands) {
            cmd->print();
            std::cout << "\n";
        }
        std::cout << "EndIf ";
    }
};

// Loop Commands
struct WhileCommand : public Command {
    std::unique_ptr<Condition> condition;
    std::vector<std::unique_ptr<Command>> commands;

    WhileCommand() : Command(CommandType::While) {}

    void print() const override{
        std::cout << "While ";
        if(condition){
            condition->print();
        }
        std::cout << " do\n";
        for (const auto& cmd : commands) {
            cmd->print();
            std::cout << "\n";
        }
        std::cout << "EndWhile ";
    }
};

struct RepeatCommand : public Command {
    std::vector<std::unique_ptr<Command>> commands;
    std::unique_ptr<Condition> condition;

    RepeatCommand() : Command(CommandType::Repeat) {}

    void print() const override{
        std::cout << "Repeat\n";
        for (const auto& cmd : commands) {
            cmd->print();
            std::cout << "\n";
        }
        std::cout << "Until ";
        if(condition){
            condition->print();
        }
        std::cout << " \n";
    }
};

struct ForToCommand : public Command {
    std::string iterator;
    std::unique_ptr<Value> fromValue;
    std::unique_ptr<Value> toValue;
    std::vector<std::unique_ptr<Command>> commands;

    ForToCommand() : Command(CommandType::ForTo) {}

    void print() const override{
        std::cout << "For ";
        if(!iterator.empty()){
            std::cout<< iterator << " ";
        }
        std::cout << "from ";
        if(fromValue){
            fromValue->print();
        }
        std::cout << "to ";
        if(toValue){
            toValue->print();
        }
        std::cout << "do\n";
        for (const auto& cmd : commands) {
            cmd->print();
            std::cout << "\n";
        }
        std::cout << "EndFor ";
    }
};

struct ForDowntoCommand : public Command {
    std::string iterator;
    std::unique_ptr<Value> fromValue;
    std::unique_ptr<Value> downtoValue;
    std::vector<std::unique_ptr<Command>> commands;

    ForDowntoCommand() : Command(CommandType::ForDownto) {}

    void print() const override{
        std::cout << "For ";
        if(!iterator.empty()){
            std::cout<< iterator << " ";
        }
        std::cout << "from ";
        if(fromValue){
            fromValue->print();
        }
        std::cout << "downto ";
        if(downtoValue){
            downtoValue->print();
        }
        std::cout << "do\n";
        for (const auto& cmd : commands) {
            cmd->print();
            std::cout << "\n";
        }
        std::cout << "EndFor ";
    }
};

// Procedure-related Commands
struct ProcedureCallCommand : public Command {
    std::string identifier;
    std::vector<std::unique_ptr<Value>> arguments;

    ProcedureCallCommand() : Command(CommandType::ProcedureCall) {}

    void print() const override{
        std::cout << "Call " + identifier + " ";
        std::cout << "(";
        for (const auto& arg : arguments) {
            arg->print();
            std::cout << ", ";
        }
        std::cout << ") ";
    }
};

// I/O Commands
struct ReadCommand : public Command {
    std::unique_ptr<Identifier> identifier;

    ReadCommand() : Command(CommandType::Read) {}

    void print() const override{
        std::cout << "Read ";
        if(identifier){
            identifier->print();
        }
        std::cout << " ";
    }
};

struct WriteCommand : public Command {
    std::unique_ptr<Value> value;

    WriteCommand() : Command(CommandType::Write) {}

    void print() const override{
        std::cout << "Write ";
        if(value){
            value->print();
        }
        std::cout << " ";
    }
};


// Represents a procedure declaration
struct Procedure {
    std::string identifier;
    std::vector<std::unique_ptr<Parameter>> parameters;
    std::vector<std::unique_ptr<Variable>> declarations;  
    std::vector<std::unique_ptr<Command>> commands;

    void print() const {
        std::cout << "Procedure: " << identifier << "\n";
        std::cout << "Parameters:\n";
        for (const auto& param : parameters) {
            param->print();
            std::cout << "\n";
        }
        std::cout << "Declarations:\n";
        for (const auto& decl : declarations) {
            decl->print();
            std::cout << "\n";
        }
        std::cout << "Commands:\n";
        for (const auto& cmd : commands) {
            cmd->print();
            std::cout << "\n";
        }
    }
};


struct Main{
    std::vector<std::unique_ptr<Variable>> declarations;
    std::vector<std::unique_ptr<Command>> commands;

    void print() const {
        std::cout << "Main:\n";
        std::cout << "Declarations:\n";
        for (const auto& decl : declarations) {
            decl->print();
            std::cout << "\n";
        }
        std::cout << "Commands:\n";
        for (const auto& cmd : commands) {
            cmd->print();
            std::cout << "\n";
        }
    }
};

// Represents the entire program
struct Program {
    std::vector<std::unique_ptr<Procedure>> procedures;
    std::vector<std::unique_ptr<Variable>> declarations;  
    std::vector<std::unique_ptr<Command>> mainCommands;

    void print() const {
        std::cout << "Program:\n";
        std::cout << "Procedures:\n";
        for (const auto& proc : procedures) {
            proc->print();
            std::cout << "\n";
        }
        std::cout << "Main Declarations:\n";
        for (const auto& decl : declarations) {
            decl->print();
            std::cout << "\n";
        }
        std::cout << "Main Commands:\n";
        for (const auto& cmd : mainCommands) {
            cmd->print();
            std::cout << "\n";
        }
    }
};

#endif // LANGUAGE_STRUCTS_H