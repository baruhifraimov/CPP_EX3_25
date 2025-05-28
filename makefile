# baruh.ifraimov@gmail.com

# Basic Setup
CXX = clang++
CXXFLAGS = -Wall -std=c++17 -ggdb -I$(INC) -I$(PLYR)
LDFLAGS = 
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Regular folders
OBJ = ./obj/
SRC = ./src/
INC = ./include/

# Player Setup
PLYR = ./include/players/
PLYR_SRC = ./src/players/

# Test folder
TST = ./tests/

# GUI folder<SFML>
GUI = ./src/GUI/

# Object files
MAIN_OBJS = $(OBJ)Demo.o $(OBJ)Game.o $(OBJ)Player.o $(OBJ)Governor.o $(OBJ)Spy.o $(OBJ)Baron.o $(OBJ)General.o $(OBJ)Judge.o $(OBJ)Merchant.o $(OBJ)PlayerFactory.o
SFML_OBJS = $(OBJ)main_sfml.o $(OBJ)Window.o $(OBJ)Game.o $(OBJ)Player.o $(OBJ)Governor.o $(OBJ)Spy.o $(OBJ)Baron.o $(OBJ)General.o $(OBJ)Judge.o $(OBJ)Merchant.o $(OBJ)PlayerFactory.o
TST_OBJS = $(OBJ)coup_test.o $(OBJ)Game.o $(OBJ)Player.o $(OBJ)Governor.o $(OBJ)Spy.o $(OBJ)Baron.o $(OBJ)General.o $(OBJ)Judge.o $(OBJ)Merchant.o $(OBJ)PlayerFactory.o
TST_HDRS = $(INC)doctest.h $(INC)Game.hpp $(PLYR)Player.hpp $(PLYR)Governor.hpp $(PLYR)Spy.hpp $(PLYR)Baron.hpp $(PLYR)General.hpp $(PLYR)Judge.hpp $(PLYR)Merchant.hpp $(INC)PlayerFactory.hpp $(INC)Operations.hpp $(INC)Role.hpp

# Create obj directory
$(shell mkdir -p $(OBJ))

# Default target
all: Main

# Main targets
Main: $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@



# Pattern rules for cleaner compilation
$(OBJ)%.o: $(SRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)%.o: $(PLYR_SRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#========================================================================#
#   		                 GUI 	  RULE                               #
#========================================================================#

GUI: $(SFML_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SFML_FLAGS) $^ -o GUI

SFML: $(SFML_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SFML_FLAGS) $^ -o $@

# Pattern rule for GUI sources
$(OBJ)%.o: $(GUI)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Special rule for SFML GUI
$(OBJ)main_sfml.o: $(GUI)main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#========================================================================#
#   		                 TEST 	 RULE                                #
#========================================================================#

# Test file rule
$(OBJ)coup_test.o: $(TST)coup_test.cpp $(TST_HDRS)
	mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)%.o: $(TST)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build and run tests
test: TestRunner
	@echo "Running tests..."

# Test executable
TestRunner: $(TST_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@






# Valgrind target
valgrind: Main
	@echo "Make sure to run 'ulimit -n 1024' before running valgrind"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./Main

# Clean target
clean:
	rm -rf Main GUI SFML $(OBJ)*.o coup_game

.PHONY: all Main GUI SFML test run run-gui run-sfml valgrind clean debug check-player
