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

// Convenience type to handle different command types
// struct AssignCommand;
// struct IfCommand;
// struct IfElseCommand;
// struct WhileCommand;
// struct RepeatCommand;
// struct ForToCommand;
// struct ForDowntoCommand;
// struct ProcedureCallCommand;
// struct ReadCommand;
// struct WriteCommand;


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
};

// Represents an array access
struct ArrayAccess {
    std::string identifier;
    std::variant<std::string, int> index;

    ArrayAccess(const std::string& id, const std::string& idx) 
        : identifier(id), index(idx) {}
    
    ArrayAccess(const std::string& id, int idx) 
        : identifier(id), index(idx) {}
};

// Represents a value - can be a number or an identifier
struct Value {
    std::variant<int, std::string, ArrayAccess> data;

    Value(int val) : data(val) {}
    Value(const std::string& val) : data(val) {}
    Value(const ArrayAccess& access) : data(access) {}

    bool isNumber() const { return std::holds_alternative<int>(data); }
    bool isIdentifier() const { return std::holds_alternative<std::string>(data); }
    bool isArrayAccess() const { return std::holds_alternative<ArrayAccess>(data); }

    int asNumber() const { return std::get<int>(data); }
    std::string asIdentifier() const { return std::get<std::string>(data); }
    ArrayAccess asArrayAccess() const { return std::get<ArrayAccess>(data); }
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
};


// Represents an array declaration
struct ArrayDeclaration {
    std::string identifier;
    int startIndex;
    int endIndex;

    ArrayDeclaration(const std::string& id, int start, int end)
        : identifier(id), startIndex(start), endIndex(end) {}
};


// Base Command structure
struct Command {
    CommandType type;
    
    Command(CommandType t) : type(t) {}
    virtual ~Command() = default;
};

// Assignment Command
struct AssignCommand : public Command {
    std::unique_ptr<Value> identifier;
    std::unique_ptr<Expression> expression;
    
    AssignCommand() : Command(CommandType::Assign) {}
};

// Conditional Commands
struct IfCommand : public Command {
    std::unique_ptr<Condition> condition;
    std::vector<std::unique_ptr<Command>> thenCommands;

    IfCommand() : Command(CommandType::If) {}
};

struct IfElseCommand : public Command {
    std::unique_ptr<Condition> condition;
    std::vector<std::unique_ptr<Command>> thenCommands;
    std::vector<std::unique_ptr<Command>> elseCommands;

    IfElseCommand() : Command(CommandType::IfElse) {}
};

// Loop Commands
struct WhileCommand : public Command {
    std::unique_ptr<Condition> condition;
    std::vector<std::unique_ptr<Command>> commands;

    WhileCommand() : Command(CommandType::While) {}
};

struct RepeatCommand : public Command {
    std::vector<std::unique_ptr<Command>> commands;
    std::unique_ptr<Condition> condition;

    RepeatCommand() : Command(CommandType::Repeat) {}
};

struct ForToCommand : public Command {
    std::unique_ptr<Value> iterator;
    std::unique_ptr<Value> fromValue;
    std::unique_ptr<Value> toValue;
    std::vector<std::unique_ptr<Command>> commands;

    ForToCommand() : Command(CommandType::ForTo) {}
};

struct ForDowntoCommand : public Command {
    std::unique_ptr<Value> iterator;
    std::unique_ptr<Value> fromValue;
    std::unique_ptr<Value> downtoValue;
    std::vector<std::unique_ptr<Command>> commands;

    ForDowntoCommand() : Command(CommandType::ForDownto) {}
};

// Procedure-related Commands
struct ProcedureCallCommand : public Command {
    std::unique_ptr<Value> identifier;
    std::vector<std::unique_ptr<Value>> arguments;

    ProcedureCallCommand() : Command(CommandType::ProcedureCall) {}
};

// I/O Commands
struct ReadCommand : public Command {
    std::unique_ptr<Value> identifier;

    ReadCommand() : Command(CommandType::Read) {}
};

struct WriteCommand : public Command {
    std::unique_ptr<Value> value;

    WriteCommand() : Command(CommandType::Write) {}
};


// Represents a procedure declaration
struct Procedure {
    std::string identifier;
    std::vector<std::unique_ptr<Parameter>> parameters;
    std::vector<std::unique_ptr<Variable>> declarations;  
    std::vector<std::unique_ptr<Command>> commands;
};


struct Main{
    std::vector<std::unique_ptr<Variable>> declarations;
    std::vector<std::unique_ptr<Command>> commands;
};

// Represents the entire program
struct Program {
    std::vector<std::unique_ptr<Procedure>> procedures;
    std::vector<std::unique_ptr<Variable>> declarations;  
    std::vector<std::unique_ptr<Command>> mainCommands;
};
// void printProgram(const Program& program) {
//     auto printIndent = [](int indent) {
//         for (int i = 0; i < indent; ++i) {
//             std::cout << "  ";
//         }
//     };

//     auto printValue = [&](const Value& value, int indent) {
//         printIndent(indent);
//         if (value.isNumber()) {
//             std::cout << value.asNumber();
//         } else if (value.isIdentifier()) {
//             std::cout << value.asIdentifier();
//         } else if (value.isArrayAccess()) {
//             const auto& access = value.asArrayAccess();
//             std::cout << access.identifier << "[";
//             if (std::holds_alternative<int>(access.index)) {
//                 std::cout << std::get<int>(access.index);
//             } else {
//                 std::cout << std::get<std::string>(access.index);
//             }
//             std::cout << "]";
//         }
//         std::cout << std::endl;
//     };

//     auto printExpression = [&](const Expression& expr, int indent) {
//         printIndent(indent);
//         std::cout << "Expression: ";
//         switch (expr.type) {
//             case ExpressionType::Value:
//                 std::cout << "Value" << std::endl;
//                 printValue(*expr.left, indent + 1);
//                 break;
//             case ExpressionType::Plus:
//                 std::cout << "Plus" << std::endl;
//                 break;
//             case ExpressionType::Minus:
//                 std::cout << "Minus" << std::endl;
//                 break;
//             case ExpressionType::Multiply:
//                 std::cout << "Multiply" << std::endl;
//                 break;
//             case ExpressionType::Divide:
//                 std::cout << "Divide" << std::endl;
//                 break;
//             case ExpressionType::Modulo:
//                 std::cout << "Modulo" << std::endl;
//                 break;
//         }
//         if (expr.left) {
//             printValue(*expr.left, indent + 1);
//         }
//         if (expr.right) {
//             printValue(*expr.right, indent + 1);
//         }
//     };

//     auto printCondition = [&](const Condition& cond, int indent) {
//         printIndent(indent);
//         std::cout << "Condition: ";
//         switch (cond.type) {
//             case ConditionType::Equal:
//                 std::cout << "Equal" << std::endl;
//                 break;
//             case ConditionType::NotEqual:
//                 std::cout << "NotEqual" << std::endl;
//                 break;
//             case ConditionType::GreaterThan:
//                 std::cout << "GreaterThan" << std::endl;
//                 break;
//             case ConditionType::LessThan:
//                 std::cout << "LessThan" << std::endl;
//                 break;
//             case ConditionType::GreaterEqual:
//                 std::cout << "GreaterEqual" << std::endl;
//                 break;
//             case ConditionType::LessEqual:
//                 std::cout << "LessEqual" << std::endl;
//                 break;
//         }
//         printValue(*cond.left, indent + 1);
//         printValue(*cond.right, indent + 1);
//     };

//     auto printCommand = [&](const Command& cmd, int indent) {
//         std::visit([&](const auto& actualCmd) {
//             using T = std::decay_t<decltype(actualCmd)>;
//             printIndent(indent);
            
//             if constexpr (std::is_same_v<T, AssignCommand>) {
//                 std::cout << "AssignCommand" << std::endl;
//             } else if constexpr (std::is_same_v<T, IfCommand>) {
//                 std::cout << "IfCommand" << std::endl;
//             } else if constexpr (std::is_same_v<T, IfElseCommand>) {
//                 std::cout << "IfElseCommand" << std::endl;
//             } else if constexpr (std::is_same_v<T, WhileCommand>) {
//                 std::cout << "WhileCommand" << std::endl;
//             } else if constexpr (std::is_same_v<T, RepeatCommand>) {
//                 std::cout << "RepeatCommand" << std::endl;
//             } else if constexpr (std::is_same_v<T, ForToCommand>) {
//                 std::cout << "ForToCommand" << std::endl;
//             } else if constexpr (std::is_same_v<T, ForDowntoCommand>) {
//                 std::cout << "ForDowntoCommand" << std::endl;
//             } else if constexpr (std::is_same_v<T, ProcedureCallCommand>) {
//                 std::cout << "ProcedureCallCommand" << std::endl;
//             } else if constexpr (std::is_same_v<T, ReadCommand>) {
//                 std::cout << "ReadCommand" << std::endl;
//             } else if constexpr (std::is_same_v<T, WriteCommand>) {
//                 std::cout << "WriteCommand" << std::endl;
//             }
//         }, cmd);
//     };


//     auto printProcedure = [&](const Procedure& proc, int indent) {
//         printIndent(indent);
//         std::cout << "Procedure: " << proc.identifier << std::endl;
//         for (const auto& param : proc.parameters) {
//             printIndent(indent + 1);
//             std::cout << "Parameter: " << param.identifier << std::endl;
//         }
//         for (const auto& decl : proc.declarations) {
//             printIndent(indent + 1);
//             std::cout << "Variable: " << decl.identifier << std::endl;
//         }
//         for (const auto& cmd : proc.commands) {
//             printCommand(*cmd.get(), indent + 1);
//         }
//     };

//     std::cout << "Program:" << std::endl;
//     for (const auto& proc : program.procedures) {
//         printProcedure(proc, 1);
//     }
//     for (const auto& decl : program.declarations) {
//         printIndent(1);
//         std::cout << "Variable: " << decl.identifier << std::endl;
//     }
//     for (const auto& cmd : program.mainCommands) {
//         printCommand(*cmd, 1);
//     }
// }

#endif // LANGUAGE_STRUCTS_H