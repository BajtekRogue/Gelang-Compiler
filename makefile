FLAGS = -W -Wall -Wextra -pedantic -std=c++17 -g
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/bin
SRC_DIR = src
HEADER_DIR = $(SRC_DIR)/headers
PARSER_DIR = $(SRC_DIR)/parsing
COMPILING_DIR = $(SRC_DIR)/compiling
CORE_DIR = $(SRC_DIR)/core
VIRTUAL_MACHINE_DIR = VM

# Find all .cpp files in the compiling and core directories
COMPILING_CPP_FILES := $(wildcard $(COMPILING_DIR)/*.cpp)
CORE_CPP_FILES := $(wildcard $(CORE_DIR)/*.cpp)
ALL_CPP_FILES := $(COMPILING_CPP_FILES) $(CORE_CPP_FILES)

# Generate object file paths
OBJ_FILES := $(patsubst $(COMPILING_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(COMPILING_CPP_FILES))
OBJ_FILES += $(patsubst $(CORE_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CORE_CPP_FILES))

# Create the build and bin directories if they don't exist
$(shell mkdir -p $(OBJ_DIR))

.PHONY: all debug release clean cleanall

# Default target (debug build)
all: debug

# Debug target
debug: $(BUILD_DIR)/ge

# Release target
release: FLAGS += -O2
release: $(BUILD_DIR)/ge
	strip $@

# Rule for the final executable
$(BUILD_DIR)/ge: $(OBJ_FILES) $(OBJ_DIR)/lexer.o $(OBJ_DIR)/parser.o
	$(CXX) $(FLAGS) $^ -o $@

# Generic rule to compile all .cpp files in the compiling and core directories
$(OBJ_DIR)/%.o: $(COMPILING_DIR)/%.cpp
	$(CXX) $(FLAGS) -I$(HEADER_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: $(CORE_DIR)/%.cpp
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
$(OBJ_DIR)/parser.cpp $(OBJ_DIR)/parser.hpp: $(PARSER_DIR)/parser.y $(HEADER_DIR)/languageStructs.hpp $(HEADER_DIR)/utility.hpp
	bison -Wall -d -o $@ $<

# Clean rules
clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/parser.cpp $(OBJ_DIR)/parser.hpp $(OBJ_DIR)/lexer.cpp

cleanall: clean
	rm -f $(BUILD_DIR)/vm/* $(BUILD_DIR)/ge
	rm -rf $(BUILD_DIR)
