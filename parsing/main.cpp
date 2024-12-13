
#include <iostream>
#include "languageStructs.hpp"

extern void run_parser(std::FILE* data, std::unique_ptr<Program>& parsed_program);

int main(int argc, char *argv[]) {
    // Check if an output file path is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <output_file_path>\n", argv[0]);
        return 1;
    }

    std::FILE* data;
    std::unique_ptr<Program> parsed_program;

    // Open the file
    data = std::fopen(argv[1], "r");
    if (!data) {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    run_parser(data, parsed_program);
    
    // Close the file
    std::fclose(data);

    return 0;
}