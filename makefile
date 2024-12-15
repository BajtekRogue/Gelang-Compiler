FLAGS = -W -std=c++17 -O3
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
SRC_DIR = src
HEADER_DIR = $(SRC_DIR)/headers
PARSER_DIR = $(SRC_DIR)/parsing
COMPILING_DIR = $(SRC_DIR)/compiling
VIRTUAL_MACHINE_DIR = VM

# Find all .cpp files in the compiling directory
CPP_FILES := $(wildcard $(COMPILING_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(COMPILING_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CPP_FILES))

# Create the build and obj directories if they don't exist
$(shell mkdir -p $(OBJ_DIR))

.PHONY: all clean cleanall

# Default target
# all: $(BUILD_DIR)/ge
# 	$(MAKE) -C $(VIRTUAL_MACHINE_DIR)

# Rule for the final executable
$(BUILD_DIR)/ge: $(OBJ_FILES) $(OBJ_DIR)/lexer.o $(OBJ_DIR)/parser.o
	$(CXX) $(FLAGS) $^ -o $@
	strip $@

# Generic rule to compile all .cpp files in the compiling directory
$(OBJ_DIR)/%.o: $(COMPILING_DIR)/%.cpp
	$(CXX) $(FLAGS) -I$(HEADER_DIR) -c $< -o $@

# Rule for generating lexer.cpp from lexer.l
$(OBJ_DIR)/lexer.o: $(OBJ_DIR)/lexer.cpp
	$(CXX) $(FLAGS) -I$(HEADER_DIR) -c $< -o $@

$(OBJ_DIR)/lexer.cpp: $(PARSER_DIR)/lexer.l $(OBJ_DIR)/parser.hpp
	flex -o $@ $<

# Rule to compile the parser
$(OBJ_DIR)/parser.o: $(OBJ_DIR)/parser.cpp
	$(CXX) $(FLAGS) -I$(HEADER_DIR) -c $< -o $@

# Rule to generate parser.cpp and parser.hpp
$(OBJ_DIR)/parser.cpp $(OBJ_DIR)/parser.hpp: $(PARSER_DIR)/parser.y $(HEADER_DIR)/languageStructs.hpp $(HEADER_DIR)/colors.hpp
	bison -Wall -d -o $(OBJ_DIR)/parser.cpp $<

# $(MAKE) -C $(VIRTUAL_MACHINE_DIR) clean <- add this if you want to clean VM as well
clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/parser.cpp $(OBJ_DIR)/parser.hpp $(OBJ_DIR)/lexer.cpp

cleanall: clean
	rm -f $(BUILD_DIR)/vm/* $(BUILD_DIR)/ge
	rm -rf $(BUILD_DIR)
