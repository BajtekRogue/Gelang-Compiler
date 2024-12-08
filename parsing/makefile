# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra

# Libraries
LIBS = -lfl -ljson-c

# File Names
LEX_FILE = lexer.l
YACC_FILE = parser.y

# Generated Files
LEX_OUTPUT = lex.yy.c
YACC_OUTPUT = parser.tab.c
YACC_HEADER = parser.tab.h

# Executable
EXEC = compiler

# JSON Output
JSON_OUTPUT = program.json

# Default Target
.PHONY: all clean run

all: $(EXEC)

# Build Executable
$(EXEC): $(YACC_OUTPUT) $(LEX_OUTPUT)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Generate Parser Files
$(YACC_OUTPUT) $(YACC_HEADER): $(YACC_FILE)
	bison -d $(YACC_FILE)

# Generate Lexer File
$(LEX_OUTPUT): $(LEX_FILE)
	flex $(LEX_FILE)

# Run and Generate JSON
run: $(EXEC)
	./$(EXEC) input.imp > $(JSON_OUTPUT)
	@echo "JSON output saved to $(JSON_OUTPUT)"

# Clean Up
clean:
	rm -f $(EXEC) $(LEX_OUTPUT) $(YACC_OUTPUT) $(YACC_HEADER) $(JSON_OUTPUT)
