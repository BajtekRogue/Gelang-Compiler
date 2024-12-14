FLAGS = -W -std=c++17 -O3

.PHONY = all clean cleanall

all: X

X: lexer.o parser.o main.o
	$(CXX) $^ -o $@
	strip $@

%.o: %.cpp
	$(CXX) $(FLAGS) -c $^

lexer.cpp: lexer.l parser.hpp
	flex -o $@ $<

parser.cpp parser.hpp: parser.y
	bison -Wall -d -o parser.cpp $^

clean:
	rm -f *.o parser.cpp parser.hpp lexer.cpp X

cleanall: clean
	rm -f X