#include "assembling.hpp"
#include "languageStructs.hpp"
#include "colors.hpp"

extern void runParser(std::FILE* data, std::unique_ptr<Program>& parsedProgram);

int main(int argc, char *argv[]) {

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "%sUsage: %s <input_file_path> <output_file_path> [-g]\n%s",
                color_White.c_str(), argv[0], color_Reset.c_str());
        return 1;
    }

    bool debugging = argc == 4 && std::string(argv[3]) == "-g";
    std::FILE* data = nullptr;
    std::unique_ptr<Program> parsedProgram;

    // Open the input file
    data = std::fopen(argv[1], "r");
    if (!data) {
        fprintf(stderr, "%sError: Could not open file %s %s\n", color_Red.c_str(), argv[1], color_Reset.c_str());
        return 1;
    }

    // Run the parser
    runParser(data, parsedProgram);

    // Always close the input file
    if (data) {
        std::fclose(data);
    }

    // std::cout << color_Magenta << "\nAST: " << color_Reset << "\n";
    // std::cout << parsedProgram->toString() << "\n";
    

    try {
        // Compile the program
        std::vector<AssemblyInstruction> code = compile(parsedProgram);
        std::cout << color_Green << "Compilation successful\n" << color_Reset;

        // Open the output file
        std::ofstream outputFile(argv[2]);
        if (!outputFile) {
            fprintf(stderr, "%sError: Could not open output file %s %s\n", color_Red.c_str(), argv[2], color_Reset.c_str());
            return 1;
        }

        // Write the assembly code to the output file
        for (const AssemblyInstruction& instruction : code) {
            std::string s = instruction.toString();
            if(!debugging && s[0] == '#') {
                continue;
            }
            outputFile << s << "\n";
        }

        return 0;

    } catch (const std::runtime_error& e) {
        // Handle specific runtime errors during compilation
        fprintf(stderr, "%sCompilation Error: %s%s\n", color_Red.c_str(), e.what(), color_Reset.c_str());
        return 1;

    } catch (const std::bad_alloc& e) {
        // Handle memory allocation errors
        fprintf(stderr, "%sMemory Allocation Error: Declared arrays are too large. Please decreace their sizes. %s\n", color_Red.c_str(), color_Reset.c_str());
        return 1;

    } catch (const std::exception& e) {
        // Catch any other standard exceptions
        fprintf(stderr, "%sUnexpected Error: %s%s\n", color_Red.c_str(), e.what(), color_Reset.c_str());
        return 1;

    } catch (...) {
        // Catch any unknown exceptions
        fprintf(stderr, "%sUnknown Critical Error Occurred%s\n", color_Red.c_str(), color_Reset.c_str());
        return 1;
    }
}
