# COUP Game (C++/SFML)

## DISCLAIMER
- The game has been tested on a Debian-based Linux distro.
- The GUI was developed with assistance from AI.

## Game Description

COUP is a strategy card game implemented in C++ with a graphical interface using SFML. The game simulates a battle of influence, deduction, and bluffing between players, each with a secret role and special abilities.

- **Minimum Players:** 2
- **Maximum Players:** 6
- **Roles:**
  - Baron
  - General
  - Governor
  - Judge
  - Merchant
  - Spy

Each role has unique abilities and may have special actions.

### Objective
The goal is to be the last player remaining by eliminating all other players through actions and strategic play.

## How to Play
** DEBUG ** you can enable DEBUG by going to the windows.cpp and turn the DEBUG flag to TRUE. it will enable SKIP button for skipping turns and reveal coins for all players in the Play screen.

1. **Start the Game:**
   - Run the executable. The GUI will prompt for the number of players (2-6).
   - Enter each player's name. Roles are assigned randomly.
2. **Gameplay:**
   - Players take turns performing actions.
   - If a player accumulates 10 or more coins, they must perform a Coup.
   - Players can use their coins and role abilities to perform actions, block others, or intervene.
3. **Actions:**
   - **Gather:** Gain 1 coin.
   - **Tax:** Gain 2 or 3 coins (depending on role).
   - **Bribe:** Spend 4 coins for an extra turn (can be blocked by Judges).
   - **Arrest:** Target another player, costing coins and possibly removing them from play.
   - **Sanction:** Block another player's actions for a round (costs 3 coins).
   - **Coup:** Spend 7 coins to eliminate another player (mandatory at 10+ coins).
   - **Special Actions:**
     - **Baron:** Invest 3 coins to get 6 coins; gets compensation if sanctioned.
     - **Spy:** View coins or block arrests.
     - **Governor:** Block tax actions.
     - **General/Judge:** Intervene in Coup/Bribe attempts.
     - **Merchant:** Gets bonus coin if starting turn with 3+ coins; pays extra coin to the treasury if arrested.

4. **Winning:**
   - The last remaining active player is declared the winner.

## Building and Running

 **Build:**
   - Run `make Main` to build and run the game. 

**Requierments**
- SFML Library (Simple and Fast Multimedia Library)
`sudo apt-get install libsfml-dev`


## Controls (GUI)
- Use mouse to click buttons and select actions.
- Enter player names and number of players as prompted.
- Follow on-screen prompts for targeted actions.

## Tests (./tests)
The tests (in coup_test.cpp and Demo.cpp) check the main game logic, including:

Player registration, turn order, and winner detection.
All player actions (gather, tax, bribe, arrest, sanction, coup) and their effects.
Special role abilities and interventions (Baron, Spy, Governor, General, Judge, Merchant).
Edge cases like illegal moves, blocked actions, and game reset.
Memory management (no leaks).
Tests use the doctest framework and cover both normal and edge-case scenarios to ensure the game works as expected.

## Valgrind explanation  

When running Valgrind:

**Tests (no leaks):** The unit tests only exercise your C++ game logic without any graphics libraries. no leaks found.

**GUI (shows leaks):** The GUI uses SFML for graphics rendering. Valgrind reports leaks from the graphics driver (`virtio_gpu_dri.so`) and SFML libraries, not from your code. These are GPU/OpenGL resources that the graphics system allocates but doesn't release until process termination - this is normal for graphics applications and not actual leaks in game logic.

## make Commands
`make Main` - Builds and runs the GUI version of the game

`make test` - Compiles and runs all unit tests using the TestRunner executable

`make valgrind` - Runs memory leak checks on both GUI and test executables using Valgrind

`make clean` - Removes all compiled object files and executables to clean up the workspace

## Project Structure

```
CPP_EX3_25/
├── makefile
├── docs/
│   └── README.md
├── include/
│   ├── doctest.h
│   ├── Game.hpp
│   ├── Operations.hpp
│   ├── PlayerFactory.hpp
│   ├── Role.hpp
│   └── GUI/
│       └── Window.hpp
│   └── players/
│       ├── Baron.hpp
│       ├── General.hpp
│       ├── Governor.hpp
│       ├── Judge.hpp
│       ├── Merchant.hpp
│       ├── Player.hpp
│       └── Spy.hpp
├── obj/
├── src/
│   ├── Game.cpp
│   ├── PlayerFactory.cpp
│   ├── GUI/
│   │   ├── main.cpp
│   │   ├── Window.cpp
│   │   └── fonts/
│   │       └── PressStart2P-Regular.ttf
│   └── players/
│       ├── Baron.cpp
│       ├── General.cpp
│       ├── Governor.cpp
│       ├── Judge.cpp
│       ├── Merchant.cpp
│       ├── Player.cpp
│       └── Spy.cpp
├── tests/
│   ├── coup_test.cpp
│   └── Demo.cpp
```

---

Baruh Ifraimov
