#include <iostream>
#include <vector>
#include "../include/Game.hpp"

using namespace coup;

std::string Game::turn() {
    // Check for empty player list
    if (this->arr_players.empty()) {
        throw std::runtime_error("No players in the game");
    }
    
    // Calculate current player index
    size_t current_index = this->index % this->arr_players.size();
    
    return arr_players.at(current_index);
}

std::vector<std::string> Game::players(){
	return this->arr_players;
}

std::string Game::winner(){
	if(this->arr_players.size()!=1){
		throw std::runtime_error( "Game is still runing" );
	}
	else{
		return this->arr_players[0];
	}
}

void Game::addPlayer(std::string name){
	this->arr_players.push_back(name);
}

void Game::removePlayer(std::string name){
	for (size_t i = 0; i < this->arr_players.size(); i++)
	{	
		// deletes the payer(name) from the players list (0 if true)
		if(!this->arr_players.at(i).compare(name)){
			if(i <= this->index){
				this->index--;
			}
			this->arr_players.erase(begin(arr_players)+i);
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
}