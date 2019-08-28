CPPFLAGS= -Wall -Wextra -pedantic -g

all:	Function_test
	# Insert command to compile the code and generate executable

Function_test:	Function_test.o
				g++ $(CPPFLAGS) Function_test.o -o Function_test

Function_test.o:	Function_test.cpp
					g++ -std=c++11 $(CPPFLAGS) -c Function_test.cpp

run:	Function_test
		./Function_test

checkmem:	Function_test
			valgrind --leak-check=full ./Function_test

clean:
			rm -f *.o Function_test
