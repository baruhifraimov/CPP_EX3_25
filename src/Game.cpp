#include <iostream>
#include <vector>
#include "Role.hpp"
#include "../include/players/Player.hpp"
#include "../include/players/General.hpp"
#include "../include/players/Judge.hpp"

#include "../include/Game.hpp"

using namespace coup;

void Game::reset() {
    // Reset all game state to fresh values
    player_objects.clear();
    cur_round = 0;
    index = 0;
    treasury = INITIAL_TREASURY;
	target_player = nullptr;
}

void Game::start_game() {
    size_t n = this->player_objects.size();
    if (n < MIN_PLAYERS || n > MAX_PLAYERS) {
        throw std::runtime_error(
            "Game requires between " + std::to_string(MIN_PLAYERS) +
            " and " + std::to_string(MAX_PLAYERS) +
            " players; currently " + std::to_string(n)
        );
    }
    // reset turn counters
    this->cur_round = 0;
    this->index     = 0;
}

std::string Game::turn() {
    // Check for empty player list
    if (this->player_objects.empty()) {
        throw std::runtime_error("No players in the game");
    }
    
    // Calculate current player index
    size_t current_index = this->index % this->player_objects.size();
    
    return player_objects.at(current_index)->getName();
}

Player* Game::get_current_player() {
    // Check for empty player list
    if (this->player_objects.empty()) {
        throw std::runtime_error("No players in the game");
    }
    
    // Calculate current player index
    size_t current_index = this->index % this->player_objects.size();
    
    return player_objects.at(current_index);
}

std::vector<std::string> Game::players(){
	std::vector<std::string> names;
    for (Player* player : player_objects) {
        if (player && player->getActive()) {  // Only active players
            names.push_back(player->getName());
        }
    }
    return names;
}

std::string Game::winner(){
	if(this->player_objects.size()!=1){
		throw std::runtime_error( "Game is still runing" );
	}
	else{
		return this->player_objects[0]->getName();
	}
}

void Game::registerPlayer(Player* player) {
	if(player_objects.size() <= MAX_PLAYERS){
	// Only add if not already present
	for (Player* p : player_objects) {
		if (p == player) {
			return;  // Already registered
		}
	}

	
	player_objects.emplace_back(player);
}
else{
	throw std::out_of_range("Maximum number of players reached");
}
}

void Game::removePlayer(std::string name){
	for (size_t i = 0; i < this->player_objects.size(); i++)
	{	
		// deletes the payer(name) from the players list (0 if true)
		if(!this->player_objects.at(i)->getName().compare(name)){
			if(i <= this->index){
				this->index--;
			}
			player_objects.at(i)->setActive(false);
			this->player_objects.erase(begin(player_objects)+i);
			return;
		}
	}
	throw std::invalid_argument("Player not found");
	
}

// void Game::take_coins(int n){
// 	// check if we can take n coins from treasury
// 	if(this->treasury - n < 0){
// 		throw std::range_error("Not enough coins in treasury");
// 	}
// 	this->treasury-=n;
// }

void Game::add_coins(int n){
	// check if we can take n coins from treasury
	if(this->treasury + n < 0){
		throw std::range_error("Not enough coins in treasury");
	}
	this->treasury+=n;
}

void Game::next_turn(){
    // Increment the index before normalizing it
    this->cur_round+=1;
    this->index+=1;
    
    // Normalize the index - in case the array size has changed
    this->index = this->index % player_objects.size();
    
    if(player_objects.size() < MIN_PLAYERS){
        this->winner();
    }
    
    for (int i = 0; i < player_objects.size(); i++)
    {
        if(player_objects.at(i)->getName() == this->turn() && player_objects.at(i)->getActive()) {
            player_objects[i]->update_block_timers();
            if(player_objects.at(i)->IsOver10Coins()){
                std::cout << *player_objects.at(i)<<" got over 10 coins, must use coup, all other abilities are disabled" << std::endl;
            }
            // Check if Merchant, if yes -> do special ability
            if(player_objects.at(i)->getRole() == Role::MERCHANT && player_objects.at(i)->coins() >= 3){
                player_objects.at(i)->addCoins(1);
            }
        }
        else{
            if(!player_objects.at(i)->getActive()){
                throw std::runtime_error(player_objects.at(i)->getName()+" is not active.");
            }
        }
    }
}

bool Game::check_general_intervention(Player& attacker, Player& target){
	std::vector<Player*> generals = get_generals();

	if(generals.size()>0){
		for(Player* general : generals){
			// Skip if the general is the attacker or got below 5<coins
			if(general == &attacker || general->coins()<5){
				continue;
			}

			// Ask general if they want to intervene
			if(dynamic_cast<General*>(general)->undo(attacker, target)){
				return true;
			}
		}
	}
	return false;

}

bool Game::check_judge_intervention(Player& attacker){
	std::vector<Player*> judges = get_judges();

	if(judges.size()>0){
		for(Player* judge : judges){
			// Skip if the judge is the attacker
			if(judge == &attacker){
				continue;
			}

			// Ask general if they want to intervene
			if(dynamic_cast<Judge*>(judge)->undo((attacker)) && judge_intervention){
				return true;
			}
		}
	}
	return false;
}

std::vector<Player*> Game::get_generals(){
	std::vector<Player*> generals;
	for (size_t i = 0; i < player_objects.size(); i++)
	{
		if(player_objects.at(i)->getRole() == Role::GENERAL){
			generals.emplace_back(player_objects.at(i));
		}
	}
	
	return generals;
}

std::vector<Player*> Game::get_judges(){
	std::vector<Player*> judges;
	for (size_t i = 0; i < player_objects.size(); i++)
	{
		if(player_objects.at(i)->getRole() == Role::JUDGE){
			judges.emplace_back(player_objects.at(i));
		}
	}
	
	return judges;
}

std::vector<Player*> Game::get_players_objects(){
	return this->player_objects;
}

void Game::set_target_player(Player& o){
	this->target_player = &o;
}

Player& Game::get_target_player(){
	return *this->target_player;
}

bool Game::get_general_intervention() const {
    return general_intervention;
}

void Game::set_general_intervention(bool value) {
    general_intervention = value;
}

bool Game::get_judge_intervention() const {
    return judge_intervention;
}

void Game::set_judge_intervention(bool value) {
    judge_intervention = value;
}

Player* Game::getLastArrestedPlayer() const {
    return lastArrestedPlayer;
}

void Game::setLastArrestedPlayer(Player* player) {
    lastArrestedPlayer = player;
}