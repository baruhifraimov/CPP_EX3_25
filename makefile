# baruh.ifraimov@gmail.com
CXX = clang++
CXXFLAGS = -Wall -std=c++17 -ggdb
OBJ = ./obj/
SRC = ./src/
INC = ./inc/

all: mmain mtest

Main:
	echo "Compiling Main..."

test:
	echo "Running tests..."

mmain:
	echo "Compiling Main..."

mtest:
	echo "Running tests..."

.PHONY: Main test valgrind clean all

#make sure to run 'ulimit -n 1024' before running valgrind, thank you
#
#--leak-check=full: "each individual leak will be shown in detail"
#--show-leak-kinds=all: Show all of "definite, indirect, possible, 
#				reachable" leak kinds in the "full" report.
#--track-origins=yes: Favor useful output over speed. This tracks 
#			the origins of uninitialized values, which could be very useful 
#			for memory errors. Consider turning off if Valgrind is unacceptably slow.
#
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./Main

clean:
	rm -rf Main matxTest $(OBJ)