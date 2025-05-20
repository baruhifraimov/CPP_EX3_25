#pragama once

// can be up to 6 players, default is 2
// if player got 10 coins, he needs to use 'coup'
#include <iostream>
#include "game.hpp"

class Player{
	private:
		std::string player_name;
		std::string role;
		int num_coins;
		int blocked;
		Game current_game;
	public:
		// Constructor
		Player(Game& game,std::string name): player_name(name), role(""),num_coins(0), blocked(0),current_game(game){
			
		}
		// Copy constructor
		Player(Player &o): player_name(o.player_name), role(o.role), num_coins(o.num_coins),blocked(o.blocked), current_game(o.current_game){

		}
		// Destructor
		virtual ~Player(){
			delete current_game;
		}
		// Copy assignment operator
		Player& operator=(Player& o)
			{	// Check self assignment
				if (this == &o){
				return *this;
			}
			player_name = o.player_name;
			role = o.role;
			num_coins = o.num_coins;
			blocked = o.blocked;
			current_game = o.current_game;
			return *this;

		}

		/** @brief Player takes 1 coin from treasury
			* COST: 0
			* can be disabled by: sanction
		*/
		virtual void gather() = 0;

		/** @brief Player takes 2 coins from treasury
			* COST: 0
			* can be disabled by: certain roles/operations
		*/		
		virtual void tax() = 0;

		/** @brief Player pays to get an additional action
			* COST: 4
			* can be disabled by: NA
		*/		
		virtual void bribe() = 0;

		/** @brief Player selects a player (not himself) and takes from him 1 coin
		 	* NOTES: cannot be used twice on the same player in a row
			* COST: 0
			* can be disabled by: NA
		*/		
		virtual void arrest(Player& o) = 0;

		/** @brief Player selects a player (not himself) and disables his (gather,tax) actions until this player next turn
			* COST: 3
			* can be disabled by: NA
		*/		
		virtual void sanction(Player& o) = 0;

		/** @brief Player selects a player (not himself) and removes him from the game
			* COST: 7
			* can be disabled by: certain conditions
		*/		
		virtual void coup(Player& p) = 0;

		/**
		 * @brief 
		 * 
		 * @return int 
		 */
		virtual int coins() const{
			return this->num_coins;
		}

		std::ostream& operator<<(std::ostream& out, const Player& p){
			return out << p.player_name;
		}

		
};