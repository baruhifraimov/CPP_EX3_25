#include "Game.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include <iostream>
#include <vector>
#include "PlayerFactory.hpp"

using namespace std;
using namespace coup;

int main() {
    Game game;
    Player* gov;
    Player* spy;
    Player* baron;
    Player* general;
    Player* judge;
    Player* merchant;

    // Create and register players using PlayerFactory
    gov = &PlayerFactory::create_player(game, "Gov", Role::GOVERNOR);
    spy = &PlayerFactory::create_player(game, "Spy", Role::SPY);
    baron = &PlayerFactory::create_player(game, "Baron", Role::BARON);
    general = &PlayerFactory::create_player(game, "General", Role::GENERAL);
    judge = &PlayerFactory::create_player(game, "Judge", Role::JUDGE);
    merchant = &PlayerFactory::create_player(game, "Merchant", Role::MERCHANT);

    game.start_game();

    // Print initial players and turn
    // gov
    // spy
    // baron
    // general
    // judge
    // merchant
    for (Player* p: game.get_players_objects()){
        cout<<*p <<" coins: " << p->coins()<<endl;
        }
    // Each player starts with 0 coins

    // Print who starts the game
    cout<< "\n (0) GAME TURN:: "<<game.turn() << endl; // gov

    // Basic round: everyone gathers
    gov->gather();
    spy->gather();
    baron->gather();
    general->gather();
    judge->gather();
    merchant->gather();

    // Show coins after first round, should be all 1
    for (Player* p: game.get_players_objects()){
        cout<<*p <<" coins: " << p->coins()<<endl;
        }

    // Demonstrate tax
    gov->tax(); // 4
    spy->tax(); // 3
    baron->tax(); // 3
    general->tax(); // 3
    judge->tax(); // 3
    merchant->tax(); // 3

    // Show coins after second round
    for (Player* p: game.get_players_objects()){
        cout<<*p <<" coins: " << p->coins()<<endl;
        }

    // Governor gets 3 coins from tax, others get 2
    gov->tax(); // 7
    spy->tax(); // 5
    // Baron uses special invest ability: pays 3, gets 6 (net +3)
    dynamic_cast<Baron*>(baron)->invest(); // 6
    general->tax(); // 6
    judge->tax(); // 5
    // Merchant bonus +1  
    merchant->tax(); // 6

    // Show coins after third round
    for (Player* p: game.get_players_objects()){
    cout<<*p <<" coins: " << p->coins()<<endl;
    }
    // Check that Baron and Merchant special abilities are reflected

    // General blocks coup on spy (special ability)
    dynamic_cast<General*>(general)->undo(*gov, *spy, true);
    gov->pay_coup_cost(*spy); // pays the coup coins
    gov->coup(*spy); // does coup

    // check if spy is still alive
    cout <<boolalpha <<spy->getActive(); // true

    // Judge blocks bribe (special ability)
    dynamic_cast<Judge*>(judge)->undo(*spy, true);
    spy->bribe();

    // Sanction and arrest
    baron->sanction(*general);
    // Baron sanctions General (blocks gather/tax for General)

    // should be blocked, general can't gather due to sanction
    try{
    general->gather();
    }catch(exception& e){
        cout << "Error: " << e.what() << endl; 
    }

    cout<< "\n (1) GAME TURN:: "<<game.turn() << endl;

    // arrest once
    general->arrest(*merchant); 
    // General arrests Merchant (Merchant pays double penalty)
    cout<< "\n (2) GAME TURN:: "<<game.turn() << endl;

    // arrest twice, error
    try { 
        judge->arrest(*merchant); 
    }
    catch(const exception& e) 
         { 
            cout << "Error: " << e.what() << endl; 
        }
    // Can't arrest same player twice

    // adding coins enough to coup
    judge->addCoins(7);
    dynamic_cast<General*>(general)->undo(*judge,*merchant,false);
    // General can block coup on merchant
    judge->pay_coup_cost(*merchant);
    judge->coup(*merchant);

    cout<< "\n (3) GAME TURN:: "<<game.turn() << endl;

    // Show coins after judge's coup
    for (Player* p: game.get_players_objects()){
        cout<<*p <<" coins: " << p->coins()<<endl;
        }

    cout<< "\n (4) GAME TURN:: "<<game.turn() << endl;

    // Governor eliminates Baron
    gov->addCoins(10); 
    gov->pay_coup_cost(*baron);
    gov->coup(*baron);

    for (Player* p: game.get_players_objects()){
    cout<<*p <<" coins: " << p->coins()<<endl;
    }

    cout<< "\n (5) GAME TURN:: "<<game.turn() << endl;
    
    dynamic_cast<Spy*>(spy)->view_coins(*general); // should see 0

    // Spy doesnt have enough coins
    try{
    spy->pay_coup_cost(*judge);
    spy->coup(*judge);
    }catch(exception& e){
        cout << e.what() << endl; 
    }

    for (Player* p: game.get_players_objects()){
        cout<<*p <<" coins: " << p->coins()<<endl;
        }

    cout<< "\n (6) GAME TURN:: "<<game.turn() << endl;
    
    // Cant view coins of non active players
    try{
    dynamic_cast<Spy*>(spy)->view_coins(*baron);
    }catch(exception& e){
        cout << "Error: " << e.what() << endl; 
    }

    dynamic_cast<Spy*>(spy)->disable_arrest(*judge); // spy special abillity 
    // Spy disables arrest on Judge

    for (Player* p: game.get_players_objects()){
    cout<<*p <<" coins: " << p->coins()<<endl;
    }
    
    // cant decalre winner when there is still more that one player
    try {
        cout  << game.winner() << "Is the Winner"<< endl;
    } catch(exception& e) {
        cout << "Error: " << e.what() << endl; 
    }

    cout<< "\n (7) GAME TURN:: "<<game.turn() << endl;

    spy->gather();
    general->gather();

    // Judge tries to arrest Spy, but Spy blocked arrest
    try{
    judge->arrest(*spy); // arrest have been blocked by spy
    }catch(exception& e){
        cout << "Error: " << e.what() << endl; 
    }
    // Judge eliminates Governor
    judge->addCoins(10);
    judge->pay_coup_cost(*gov);
    judge->coup(*gov);

    spy->tax();

    cout<< "\n (8) GAME TURN:: "<<game.turn() << endl;

    for (Player* p: game.get_players_objects()){
        cout<<*p <<" coins: " << p->coins()<<endl;
        }
    
    // Judge eliminates General
    general->gather();
    judge->pay_coup_cost(*general);
    judge->coup(*general);

    cout<< "\n (9) GAME TURN:: "<<game.turn() << endl;

    for (Player* p: game.get_players_objects()){
        cout<<*p <<" coins: " << p->coins()<<endl;
        }
    
    spy->tax();
    judge->tax();
    spy->tax();
    judge->tax();


    // showcasing bribe()
    dynamic_cast<Judge*>(judge)->undo(*spy, false);
    spy->bribe();

    cout<< "\n GAME TURN:: "<<game.turn() << endl;
    spy->tax();
    cout<< "\n GAME TURN:: "<<game.turn() << endl;
    spy->sanction(*judge); // activating judge special abbility, instead of -3 coins, -4 coins to spy
    cout<< "\n Spy coins:: "<<spy->coins() << endl; // 3
    cout<< "\n GAME TURN:: "<<game.turn() << endl;
    
    try{
    judge->gather();
    }catch(exception &e){
        cout << "Error: " << e.what() << endl; 
    }
    // skipping judge turn
    game.next_turn();

    // Spy eliminates Judge
    spy->addCoins(10);
    spy->pay_coup_cost(*judge);
    spy->coup(*judge);

    cout<< "\n (10) GAME TURN:: "<<game.turn() << endl;

    for (Player* p: game.get_players_objects()){
        cout<<*p <<" coins: " << p->coins()<<endl;
        }
    
    cout  << game.winner() << " is the Winner"<< endl;
    // Only one player left, declare winner

    return 0;
}
