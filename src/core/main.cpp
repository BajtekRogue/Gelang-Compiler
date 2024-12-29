#include "assembling.hpp"
#include "languageStructs.hpp"
#include "utility.hpp"
#include "compiling.hpp"
#include "errorHandler.hpp"

extern void runParser(std::FILE* data, std::unique_ptr<Program>& parsedProgram);

int main(int argc, char *argv[]) {

    if (argc < 3 || argc > 4) {
        std::cerr << Colors::red << "Usage: " << argv[0] << " <inputFilePath> <outputFilePath> [-g]" << Colors::reset << std::endl;
        return 1;
    }

    bool debugging = argc == 4 && std::string(argv[3]) == "-g";
    std::FILE* data = nullptr;
    std::unique_ptr<Program> parsedProgram;

    // Open the input file
    data = std::fopen(argv[1], "r");
    if (!data) {
        std::cerr << Colors::red << "Error: Could not open file " << argv[1] << Colors::reset << std::endl;
        return 1;
    }

    // Run the parser
    runParser(data, parsedProgram);

    // Rewind file to beginning
    if (std::rewind(data), ferror(data)) {
        std::cerr << Colors::red << "Error: Could not rewind file for error handling setup" << Colors::reset << std::endl;
        std::fclose(data);
        return 1;
    }

    // Store lines for potential errors
    ErrorHandler::initialize(data);

    // Always close the input file
    if (data) {
        std::fclose(data);
    }

    // Compile the program
    std::vector<AssemblyInstruction> code;
    try{
        code = compile(parsedProgram);
    }
    catch(const std::exception& e){
        std::cerr << Colors::red << "Error: " << e.what() << Colors::reset << std::endl;
        return 1;
    }

    std::cout << Colors::green << "Compilation successful\n" << Colors::reset;

    // Open the output file
    std::ofstream outputFile(argv[2]);
    if (!outputFile) {
        std::cerr << Colors::red << "Error: Could not open output file " << argv[2] << Colors::reset << std::endl;
        return 1;
    }

    // Write the assembly code to the output file
    for (const auto& instruction : code) {
        std::string s = instruction.toString();
        // Skip comments if not debugging
        if(!debugging && s[0] == '#') {
            continue;
        }
        outputFile << s << "\n";
    }

    return 0;
}
