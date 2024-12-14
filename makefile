FLAGS = -W -std=c++17 -O3
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
SRC_DIR = src
HEADER_DIR = $(SRC_DIR)/headers
PARSER_DIR = $(SRC_DIR)/parsing
COMPILING_DIR = $(SRC_DIR)/compiling
VIRTUAL_MACHINE_DIR = VM

# Create the build and obj directories if they don't exist
$(shell mkdir -p $(OBJ_DIR))

.PHONY = all clean cleanall

# Default target to compile the program
all: $(BUILD_DIR)/ge
	$(MAKE) -C $(VIRTUAL_MACHINE_DIR)

# Rule for the final executable
$(BUILD_DIR)/ge: $(OBJ_DIR)/lexer.o $(OBJ_DIR)/parser.o $(OBJ_DIR)/main.o $(OBJ_DIR)/codeGenerator.o
	$(CXX) $(FLAGS) $^ -o $@
	strip $@

# Rule to compile all .cpp files in the compiling directory
$(OBJ_DIR)/%.o: $(COMPILING_DIR)/%.cpp
	$(CXX) $(FLAGS) -I$(HEADER_DIR) -c $< -o $@

# Rule to generate lexer.cpp from lexer.l (using Flex)
$(OBJ_DIR)/lexer.o: $(OBJ_DIR)/lexer.cpp
	$(CXX) $(FLAGS) -I$(HEADER_DIR) -c $< -o $@

# Rule to generate lexer.cpp from lexer.l (using Flex)
$(OBJ_DIR)/lexer.cpp: $(PARSER_DIR)/lexer.l $(OBJ_DIR)/parser.hpp
	flex -o $@ $<

# Rule to compile the parser
$(OBJ_DIR)/parser.o: $(OBJ_DIR)/parser.cpp
	$(CXX) $(FLAGS) -I$(HEADER_DIR) -c $< -o $@

# Rule to generate parser.cpp and parser.hpp from parser.y (using Bison)
$(OBJ_DIR)/parser.cpp $(OBJ_DIR)/parser.hpp: $(PARSER_DIR)/parser.y $(HEADER_DIR)/languageStructs.hpp $(HEADER_DIR)/colors.hpp
	bison -Wall -d -o $(OBJ_DIR)/parser.cpp $<

# Rule to compile the main program
$(OBJ_DIR)/main.o: $(SRC_DIR)/compiling/main.cpp
	$(CXX) $(FLAGS) -I$(HEADER_DIR) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/parser.cpp $(OBJ_DIR)/parser.hpp $(OBJ_DIR)/lexer.cpp
	$(MAKE) -C $(VIRTUAL_MACHINE_DIR) clean   

cleanall: clean
	rm -f $(BUILD_DIR)/vm/* $(BUILD_DIR)/ge
	rm -rf $(BUILD_DIR)
