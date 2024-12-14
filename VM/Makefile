FLAGS = -W -pedantic -std=c++17 -O3

.PHONY = all clean cleanall

all: ../build/vm/vm ../build/vm/vm-cln

../build/vm/vm: lexer.o parser.o mw.o main.o
	$(CXX) $^ -o $@
	strip $@

../build/vm/vm-cln: lexer.o parser.o mw-cln.o main.o
	$(CXX) $^ -o $@ -l cln
	strip $@

%.o: %.cc
	$(CXX) $(FLAGS) -c $^

lexer.cc: lexer.l parser.hh
	flex -o $@ $<

parser.cc parser.hh: parser.y
	bison -Wall -d -o parser.cc $^

clean:
	rm -f *.o parser.cc parser.hh lexer.cc

cleanall: clean
	rm -f ../build/vm/vm ../build/vm/vm-cln
	rm -f vm vm-cln
