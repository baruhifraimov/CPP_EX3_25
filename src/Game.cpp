#include <iostream>
#include <vector>
#include "../include/Game.hpp"

using namespace coup;

std::string Game::turn() {
    // Check for empty player list
    if (this->player_names.empty()) {
        throw std::runtime_error("No players in the game");
    }
    
    // Calculate current player index
    size_t current_index = this->index % this->player_names.size();
    
    return player_names.at(current_index);
}

std::vector<std::string> Game::players(){
	return this->player_names;
}

std::string Game::winner(){
	if(this->player_names.size()!=1){
		throw std::runtime_error( "Game is still runing" );
	}
	else{
		return this->player_names[0];
	}
}

void Game::registerPlayer(Player* player) {
	// Only add if not already present
	for (Player* p : player_objects) {
		if (p == player) {
			return;  // Already registered
		}
	}
	this->addPlayer(player->getName());
	player_objects.emplace_back(player);
}

void Game::addPlayer(std::string name){
	this->player_names.push_back(name);
}

void Game::removePlayer(std::string name){
	for (size_t i = 0; i < this->player_names.size(); i++)
	{	
		// deletes the payer(name) from the players list (0 if true)
		if(!this->player_names.at(i).compare(name)){
			if(i <= this->index){
				this->index--;
			}
			this->player_names.erase(begin(player_names)+i);
			player_objects.at(i)->setActive(false);
			this->player_objects.erase(begin(player_objects)+i);
			return;
		}
	}
	throw std::invalid_argument("Player not found");
	
}

void Game::take_coins(int n){
	// check if we can take n coins from treasury
	if(this->treasury - n < 0){
		throw std::range_error("Not enough coins in treasury");
	}
	this->treasury-=n;
}

void Game::add_coins(int n){
	this->treasury+=n;
}

void Game::next_turn(){
	this->cur_round+=1;
	this->index+=1;

	for (int i = 0; i < player_objects.size(); i++)
	{
		if(player_objects.at(i)->getName() == this->turn()) {
			player_objects[i]->update_block_timers();
		}
	}
	
	
}