#pragma once

// can be up to 6 players, default is 2
// if player got 10 coins, he needs to use 'coup'
#include <iostream>
#include "../Game.hpp"

namespace coup{

// Defines if the operation is blocked (1) or unblocked (0)
// Stores each enum value using 1 byte — exactly 8 bits.
enum class Operation : u_int8_t {
    NONE     = 0x00,  // 00000000
    GATHER   = 0x01,  // 00000001
    TAX      = 0x02,  // 00000010
    BRIBE    = 0x04,  // 00000100
    ARREST   = 0x08,  // 00001000
    SANCTION = 0x10,  // 00010000
    COUP     = 0x20,  // 00100000

	
};

// inline to declare the operator& same across all files
// we convert to unsigned int to work with bit operations and reconvert to the enum class
inline Operation operator&(Operation a, Operation b){
	return static_cast<Operation>(static_cast<u_int8_t>(a) & static_cast<u_int8_t>(b));
}

inline Operation operator|(Operation a, Operation b){
	return static_cast<Operation>(static_cast<u_int8_t>(a) | static_cast<u_int8_t>(b));
}

inline Operation& operator|=(Operation& a, Operation b){
	a = a | b;
	return a;
}

class Player{
	protected:
		std::string player_name;
		int num_coins;
		Game &current_game;
		Operation blocked_operations;
		bool is_active;

		/**
		 * @brief Checks if the operation is available to use
		 * 
		 * @param op The operator that we want to use
		 * @return true Is blocked
		 * @return false Is unblocked
		 */
		bool is_operation_blocked(Operation op){
			return ((blocked_operations & op) == op);
		}

	public:
		// Constructor
		Player(Game& game,std::string name): 
											player_name(name), 
											num_coins(0), 
											current_game(game),
											blocked_operations(Operation::NONE),
											is_active(true) {

			// Register player with the current game instance
			game.addPlayer(player_name);
			
		}

		// Copy constructor
		Player(Player &o): 	player_name(o.player_name), 
							num_coins(o.num_coins), 
							current_game(o.current_game){

		}

		// Destructor
		virtual ~Player(){}

		// Copy assignment operator
		Player& operator=(Player& o)
			{	// Check self assignment
				if (this == &o){
				return *this;
			}
			player_name = o.player_name;
			num_coins = o.num_coins;
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
		 * @brief Retrives the coins of the current player
		 * 
		 * @return int num of coins
		 */
		virtual int coins() const{
			return this->num_coins;
		}

		/**
		 * @brief Undo last Player action
		 * NOTE: Only Judge and Gover
		 * COST: 0
		 * @param o The other player we want to undo the action
		 */
		virtual void undo(Player& o) = 0;

		/**
		 * @brief Operator << prints the player name to STDOUT
		 * 
		 * @param out 
		 * @param p 
		 * @return std::ostream& Out with buffered player_name
		 */
		friend std::ostream& operator<<(std::ostream& out, const Player& p){
			return out << p.player_name;
		}

		virtual bool validate_active(){
			if(!this->is_active){
				throw std::runtime_error("Player " + this->player_name + " is not active anymore");
			}
			else{
				return true;
			}
		}

		
};
}