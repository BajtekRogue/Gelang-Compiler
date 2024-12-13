# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

# Bison and Flex
YACC = bison
YFLAGS = -d
LEX = flex

# Source and output files
SRCS = main.cpp
PARSER = parser
LEXER = lexer

# Target executable
TARGET = X

all: $(TARGET)

# Generate parser and lexer
$(PARSER).tab.cpp $(PARSER).tab.hpp: $(PARSER).y languageStructs.hpp
	$(YACC) $(YFLAGS) -o $(PARSER).tab.cpp $(PARSER).y

$(LEXER).cpp: $(LEXER).l $(PARSER).tab.hpp
	$(LEX) -o $(LEXER).cpp $(LEXER).l

# Compilation rules
$(PARSER).tab.o: $(PARSER).tab.cpp $(PARSER).tab.hpp
	$(CXX) $(CXXFLAGS) -c $(PARSER).tab.cpp

$(LEXER).o: $(LEXER).cpp
	$(CXX) $(CXXFLAGS) -c $(LEXER).cpp

main.o: main.cpp languageStructs.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Linking
$(TARGET): main.o $(PARSER).tab.o $(LEXER).o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o $(PARSER).tab.o $(LEXER).o -lfl

# Clean up
clean:
	rm -f *.o $(PARSER).tab.cpp $(PARSER).tab.hpp $(LEXER).cpp $(TARGET)

.PHONY: all clean