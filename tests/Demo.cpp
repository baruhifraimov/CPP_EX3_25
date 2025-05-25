
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Game.hpp"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;
using namespace coup;

int main() {
    Game game_1{};

    Governor governor(game_1, "Moshe");
    Spy spy(game_1, "Yossi");
    Baron baron(game_1, "Meirav");
    General general(game_1, "Reut");
    Judge judge(game_1, "Gilad");

    vector<string> players = game_1.players();
    
    // Expected output:
    // Moshe
    // Yossi
    // Meirav
    // Reut
    // Gilad
    for(string name : players){
        cout << name << endl;
    }

    // Expected output: Moshe
    cout << game_1.turn() << endl;

    governor.gather();
    spy.gather();
    baron.gather();
    general.gather();
    judge.gather();

    // Expected exception - Not spy's turn
    try{
        spy.gather();
    } catch (const std::exception &e){
        std::cerr << e.what() << '\n';
    }

    governor.gather();

    // Expected exception - Not enough money from the bank
    try{
    spy.tax();
    } catch (const std::exception &e){
        std::cerr << e.what() << '\n';
    }


    // Expected exception - Judge bribe block activation on 'Moshe'
    try{
        judge.undo(governor);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    cout << governor.coins() << endl; // Expected: 2
    cout << spy.coins() << endl; // Expected: 1

    // Expected exception - Governor trying to block while not in his turn
    try{
        governor.block_tax(spy); // Governor undo tax
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    cout << spy.coins() << endl; // Expected: 1

    game_1.add_coins(100); // adding 100 coins to the game bank

    spy.tax();

    baron.gather();
    general.gather();
    judge.gather();
    governor.gather();
    spy.gather();
    baron.gather();
    general.gather();
    judge.gather();
    governor.gather();
    spy.gather();

    baron.invest(); // Baron traded its 3 coins and got 6 
    cout << game_1.turn() << endl; // Expected Reut
    general.gather();
    judge.gather();
    
    cout << baron.coins() << endl; // Expected: 6

    governor.tax();
    spy.gather();
    baron.gather();
    general.gather();
    judge.gather();

    governor.tax();
    spy.gather();
    cout << baron.coins() << endl; // Expected: 7
    baron.coup(governor); // Coup against governor
    general.gather();
    judge.gather();
    
    players = game_1.players();
    // Since no one blocked the Baron, the expected output is:
    // Yossi
    // Meirav
    // Reut
    // Gilad
    for (string name : players) {
        cout << name << endl;
    }

}
