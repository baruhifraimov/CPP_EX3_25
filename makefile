# baruh.ifraimov@gmail.com
CXX = clang++
CXXFLAGS = -Wall -std=c++17 -ggdb -I$(INC) -I$(PLYR)
LDFLAGS = 
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
OBJ = ./obj/
SRC = ./src/
INC = ./include/
PLYR = ./include/players/
PLYR_SRC = ./src/players/
TST = ./tests/
GUI = ./src/GUI/

# Object files
MAIN_OBJS = $(OBJ)Demo.o $(OBJ)Game.o $(OBJ)Player.o $(OBJ)Governor.o $(OBJ)Spy.o $(OBJ)Baron.o $(OBJ)General.o $(OBJ)Judge.o $(OBJ)Merchant.o $(OBJ)PlayerFactory.o
SFML_OBJS = $(OBJ)main_sfml.o $(OBJ)Window.o $(OBJ)Game.o $(OBJ)Player.o $(OBJ)Governor.o $(OBJ)Spy.o $(OBJ)Baron.o $(OBJ)General.o $(OBJ)Judge.o $(OBJ)Merchant.o $(OBJ)PlayerFactory.o

# Create obj directory
$(shell mkdir -p $(OBJ))

# Default target
all: Main

# Main targets
Main: $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

GUI: $(SFML_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SFML_FLAGS) $^ -o GUI

SFML: $(SFML_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SFML_FLAGS) $^ -o $@

# Pattern rules for cleaner compilation
$(OBJ)%.o: $(SRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)%.o: $(PLYR_SRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)%.o: $(TST)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pattern rule for GUI sources
$(OBJ)%.o: $(GUI)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Special rule for SFML GUI
$(OBJ)main_sfml.o: $(GUI)main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug target to check object files
debug:
	@echo "Checking object files:"
	@ls -la $(OBJ)*.o 2>/dev/null || echo "No object files found"
	@echo "Required files for Main:"
	@echo $(MAIN_OBJS)

# Test and utility targets
test:
	@echo "Running tests..."
	./Main

run: Main
	./Main

run-gui: GUI
	./GUI

run-sfml: SFML
	./SFML

# Valgrind target
valgrind: Main
	@echo "Make sure to run 'ulimit -n 1024' before running valgrind"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./Main

# Debug target to check if Player.cpp exists and compiles
check-player:
	@echo "Checking Player.cpp:"
	@ls -la $(PLYR_SRC)Player.cpp 2>/dev/null || echo "Player.cpp not found in $(PLYR_SRC)"
	@echo "Checking Player.o compilation:"
	$(CXX) $(CXXFLAGS) -c $(PLYR_SRC)Player.cpp -o $(OBJ)Player_test.o && echo "Player.cpp compiles successfully" || echo "Player.cpp compilation failed"

# Clean target
clean:
	rm -rf Main GUI SFML $(OBJ)*.o coup_game

.PHONY: all Main GUI SFML test run run-gui run-sfml valgrind clean debug check-player
