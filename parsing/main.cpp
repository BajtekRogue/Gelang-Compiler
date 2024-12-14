
#include <iostream>
#include "languageStructs.hpp"

extern void run_parser(std::FILE* data, std::unique_ptr<Program>& parsed_program);

int main(int argc, char *argv[]) {
    // Check if an output file path is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file_path>\n", argv[0]);
        return 1;
    }

    std::FILE* data = nullptr;
    std::unique_ptr<Program> parsed_program;

    // Open the file
    data = std::fopen(argv[1], "r");
    if (!data) {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    try {
        run_parser(data, parsed_program);
    } catch (const std::exception& e) {
        std::cerr << "Exception during parsing: " << e.what() << std::endl;
    }
    
    // Always close the file
    if (data) {
        std::fclose(data);
    }

    // Check if parsing was successful
    if (!parsed_program) {
        std::cerr << "Parsing did not produce a valid program" << std::endl;
        return 1;
    }

    return 0;
}