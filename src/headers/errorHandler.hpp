#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include <iostream>
#include <string>
#include <cinttypes>
#include <vector>
#include "nameSpaces.hpp"
#include "languageStructs.hpp"

class ErrorHandler{
public:
    ErrorHandler() = delete;
    ErrorHandler(const ErrorHandler&) = delete;
    ErrorHandler& operator=(const ErrorHandler&) = delete;

    static void initialize(std::FILE* data) {
        if (programLines.empty()) {  // Only initialize once
            char buffer[256];
            programLines.push_back(""); 

            while (fgets(buffer, sizeof(buffer), data)) {
                std::string line(buffer);

                if (!line.empty() && line.back() == '\n') {
                    line.pop_back();
                }
                programLines.push_back(line);
            }
        }
    }

    
    static void notDeclaredVariable(const std::string& variableIdentifier, int lineNumber);
    static void notDeclaredArray(const std::string& arrayIdentifier, int lineNumber);
    static void redeclarationVariable(const std::string& variableIdentifier, int lineNumber);
    static void redeclarationArray(const std::string& arrayIdentifier, int lineNumber);
    static void redeclarationParameter(const std::string& parameterIdentifier, int lineNumber);
    static void redeclarationProcedure(const std::string& procedureIdentifier, int lineNumber);
    static void overshadowingParameter(const std::string& parameterIdentifier, int lineNumber);
    static void uninitializedVariable(const std::string& variableIdentifier, int lineNumber);
    static void arrayUsedAsVariable(const std::string& arrayIdentifier, int lineNumber);
    static void variableUsedAsArray(const std::string& variableIdentifier, int lineNumber);
    static void outOfBoundsArray(const std::string& arrayIdentifier, int64_t index, int lineNumber);
    static void invalidArrayBounds(const std::string& arrayIdentifier, int lineNumber);
    static void readingIntoIterator(const std::string& iteratorIdentifier, int lineNumber);
    static void assigningToIterator(const std::string& iteratorIdentifier, int lineNumber);
    static void iteratorAsParameter(const std::string& iteratorIdentifier, int lineNumber);
    static void notDeclaredProcedure(const std::string& procedureIdentifier, int lineNumber);
    static void recursionDetected(const std::string& procedureIdentifier, int lineNumber);
    static void invalidNumberOfArguments(const std::string& procedureIdentifier, int expected, int given, int lineNumber);
    static void invalidArgumentType(const std::string& procedureIdentifier, const std::string& argument, ParameterType expected, ParameterType given, int index, int lineNumber);
    static void infiniteWhileLoop(int lineNumber);
    static void infiniteRepeatLoop(int lineNumber);

private:
    static std::vector<std::string> programLines;
    
};


#endif // ERROR_HANDLER_HPP