#include "errorHandler.hpp"

std::vector<std::string> ErrorHandler::programLines;


void ErrorHandler::notDeclaredVariable(const std::string& variableIdentifier, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Use of undeclared variable '" << variableIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::notDeclaredArray(const std::string& arrayIdentifier, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Use of undeclared array '" << arrayIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::redeclarationVariable(const std::string& variableIdentifier, int lineNumber){
    int firstDeclaration = 0;

    // If it was a local variable
    for(size_t i = 1; i < programLines.size(); i++){
        if(programLines[i-1].find("PROGRAM") != std::string::npos && programLines[i].find(variableIdentifier) != std::string::npos){
            firstDeclaration = i;
            break;
        }
    }

    // If it was an iterator
    for(size_t i = lineNumber; i >= 1; i--){

        size_t forPos = programLines[i].find("FOR");
        size_t fromPos = programLines[i].find("FROM");

        if (static_cast<int>(i) != lineNumber && forPos != std::string::npos && fromPos != std::string::npos) {
            std::string forSection = programLines[i].substr(forPos + 3,  fromPos - (forPos + 3)  );
            
            // Check if variable is in this section
            if (forSection.find(variableIdentifier) != std::string::npos) {
                firstDeclaration = i;
                break;
            }
        }
    }

    std::cerr << Colors::red << "Compilation Error: Second declaration of variable '" << variableIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    std::cerr << Colors::magenta << "First declaration at line " << firstDeclaration << ": " << programLines[firstDeclaration] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::redeclarationArray(const std::string& arrayIdentifier, int lineNumber){
    int firstDeclaration = 0;

    for(size_t i = lineNumber; i < programLines.size(); i++){
        if(programLines[i-1].find("PROGRAM") != std::string::npos && programLines[i].find(arrayIdentifier) != std::string::npos){
            firstDeclaration = i;
            break;
        }
    }

    std::cerr << Colors::red << "Compilation Error: Second declaration of array '" << arrayIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    std::cerr << Colors::magenta << "First declaration at line " << firstDeclaration << ": " << programLines[firstDeclaration] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::redeclarationParameter(const std::string& parameterIdentifier, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Second declaration of parameter '" << parameterIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::redeclarationProcedure(const std::string& procedureIdentifier, int lineNumber){
    int firstDeclaration = 0;

    for(size_t i = 0; i < programLines.size(); i++){
        if(programLines[i].find(procedureIdentifier) != std::string::npos){
            firstDeclaration = i;
            break;
        }
    }

    std::cerr << Colors::red << "Compilation Error: Second declaration of procedure '" << procedureIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    std::cerr << Colors::magenta << "First declaration at line " << firstDeclaration << ": " << programLines[firstDeclaration] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::overshadowingParameter(const std::string& parameterIdentifier, int lineNumber){
    int firstDeclaration = 0;

    for(size_t i = lineNumber; i >= 1; i--){
        if(programLines[i].find("PROCEDURE") != std::string::npos){
            firstDeclaration = i;
            break;
        }
    }

    // If line doesn't contain the variable, go down until you find it
    if(programLines[lineNumber].find(parameterIdentifier) == std::string::npos){
        for(size_t i = lineNumber; i < programLines.size(); i++){
            if(programLines[i].find(parameterIdentifier) != std::string::npos){
                lineNumber = i;
                break;
            }
        }
    }

    std::cerr << Colors::red << "Compilation Error: Overshadowing of parameter '" << parameterIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    std::cerr << Colors::magenta << "First declaration at line " << firstDeclaration << ": " << programLines[firstDeclaration] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::uninitializedVariable(const std::string& variableIdentifier, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Use of uninitialized variable '" << variableIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::arrayUsedAsVariable(const std::string& arrayIdentifier, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Use of array '" << arrayIdentifier << "' as variable " << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::variableUsedAsArray(const std::string& variableIdentifier, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Use of variable '" << variableIdentifier << "' as array " << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::outOfBoundsArray(const std::string& arrayIdentifier, int64_t index, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Index out of bounds for array '" << arrayIdentifier << "' at index " << index << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::invalidArrayBounds(const std::string& arrayIdentifier, int lineNumber){

    for(size_t i = 1; i < programLines.size(); i++){
        if(programLines[i-1].find("PROGRAM") != std::string::npos && programLines[i].find(arrayIdentifier) != std::string::npos){
            lineNumber = i;
            break;
        }
    }

    std::cerr << Colors::red << "Compilation Error: Invalid array bounds for array '" << arrayIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::readingIntoIterator(const std::string& iteratorIdentifier, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Reading into iterator '" << iteratorIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::assigningToIterator(const std::string& iteratorIdentifier, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Assigning to iterator '" << iteratorIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::iteratorAsParameter(const std::string& iteratorIdentifier, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Iterator '" << iteratorIdentifier << "' used as parameter in procedure call" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::notDeclaredProcedure(const std::string& procedureIdentifier, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Use of undeclared procedure '" << procedureIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::recursionDetected(const std::string& procedureIdentifier, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Recursion detected in procedure '" << procedureIdentifier << "'" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::invalidNumberOfArguments(const std::string& procedureIdentifier, int expected, int given, int lineNumber){
    std::cerr << Colors::red << "Compilation Error: Invalid number of arguments for procedure '" << procedureIdentifier << "'. Expected " << expected << " but got " << given << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::invalidArgumentType(const std::string& procedureIdentifier, const std::string& argument, ParameterType expected, ParameterType given, int index, int lineNumber) {

    auto parameterTypeToString = [](ParameterType type) {
        return (type == ParameterType::Integer) ? "Integer" : "Array";
    };

    std::cerr << Colors::red << "Compilation Error: Argument " << index << " ('" << argument << "') in procedure '" << procedureIdentifier << "' is of type " << parameterTypeToString(given) << ", expected " << parameterTypeToString(expected) << "." << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
    exit(1);
}

void ErrorHandler::infiniteWhileLoop(int lineNumber){
    std::cerr << Colors::magenta << "Warning: Infinite WHILE loop detected" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
}

void ErrorHandler::infiniteRepeatLoop(int lineNumber){
    std::cerr << Colors::magenta << "Warning: Infinite REPEAT loop detected" << Colors::reset << std::endl;
    std::cerr << Colors::yellow << "Line " << lineNumber << ": " << programLines[lineNumber] << Colors::reset << std::endl;
}