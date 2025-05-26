# baruh.ifraimov@gmail.com
CXX = clang++
CXXFLAGS = -Wall -std=c++17 -ggdb -I$(INC) -I$(PLYR)
OBJ = ./obj/
SRC = ./src/
INC = ./include/
PLYR = ./include/players/
PLYR_SRC = ./src/players/
TST = ./tests/

# Create obj directory
$(shell mkdir -p $(OBJ))

# Rule to compile the SFML GUI main
$(OBJ)main_sfml.o: src/GUI/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# New target for SFML GUI
Main_sfml:	$(OBJ)main_sfml.o \
			$(OBJ)Game.o \
			$(OBJ)Player.o \
			$(OBJ)Governor.o \
			$(OBJ)Spy.o \
			$(OBJ)Baron.o \
			$(OBJ)General.o \
			$(OBJ)Judge.o \
			$(OBJ)Merchant.o
			$(CXX) $(CXXFLAGS) $^ -o $@ \
			-lsfml-graphics -lsfml-window -lsfml-system


# all: mmain mtest

Main: $(OBJ)Demo.o $(OBJ)Game.o $(OBJ)Player.o $(OBJ)Governor.o $(OBJ)Spy.o $(OBJ)Baron.o $(OBJ)General.o $(OBJ)Judge.o $(OBJ)Merchant.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile main source files
$(OBJ)Demo.o: $(TST)Demo.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)Game.o: $(SRC)Game.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile Demo (test file)
$(OBJ)Demo.o: tests/Demo.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile player source files
$(OBJ)Player.o: $(PLYR_SRC)Player.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)Governor.o: $(PLYR_SRC)Governor.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)Spy.o: $(PLYR_SRC)Spy.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)Baron.o: $(PLYR_SRC)Baron.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)General.o: $(PLYR_SRC)General.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)Judge.o: $(PLYR_SRC)Judge.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)Merchant.o: $(PLYR_SRC)Merchant.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


test:
	echo "Running tests..."

mmain:
	$()

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
