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
	EXTRA_TURN = 0x40 // 01000000

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

inline Operation& operator&=(Operation& a, Operation b){
	a = a & b;
	return a;
}

inline Operation operator~(Operation a){
	return static_cast<Operation>(~static_cast<u_int8_t>(a));
}



class Player{
	protected:
		std::string player_name;
		int num_coins;
		Game &current_game;
		Operation blocked_operations;
		bool is_active;

		// Field to track block timers for each operation
		u_int8_t block_timers[8];  // One timer for each bit in Operation

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

		/**
		 * @brief Unblocking operations by unmasking them
		 * 
		 * @param op The operation that you want to unblock
		 */
		void unblock_operation(Operation op) {
			blocked_operations = blocked_operations & ~op;
		}

		/**
		 * @brief Unblocking operations by unmasking them
		 * 
		 * @param op The operation that you want to unblock now with integers
		 */
		void unblock_operation(u_int8_t op) {
			blocked_operations = blocked_operations & ~static_cast<Operation>(op);
		}

		/**
		 * @brief Sets a timer for a blocked operation to understand when to unblock it in the next round
		 * Initalizes timer with 1 for each new block
		 * @param op The operation that we activated the block
		 */
		void block_operation_with_timer(Operation op) {
			blocked_operations |= op;
			
			// Initialize timer for each bit that's set
			for (int i = 0; i < 8; i++) {
				// Check if this bit is set in op
				if ((static_cast<u_int8_t>(op) & (1 << i)) != 0 & i != 7) {
					// Check if ARREST arrtibute in operation is ON, means someone activated arrest this round on this exact player
					// which results an illegal move
					if(i == 3 && block_timers[i] == 1){
						throw std::runtime_error("Player cannot be arrested twice in a lap, illegal move");
					}
					block_timers[i] = 1;  // Start timer at 1
				}
			}
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
			game.registerPlayer(this);

			// Initialize all timers to 0
			for (int i = 0; i < 8; i++) {
				block_timers[i] = 0;
			}
			
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

		/**
		 * @brief Updates the timers on the blocked operations
		 * If the operation block timer is 2 then it time to unblock
		 * Means you already passed a round with the blocked operation and now you are in a new round, means the operation is unlocked
		 * 
		 */
		void update_block_timers() {
			for (int i = 0; i < 8; i++) {
				// If this bit is blocked and has a timer
				if ((static_cast<u_int8_t>(blocked_operations) & (1 << i)) != 0 && block_timers[i] > 0) {
					block_timers[i]++;  // Increment timer
					
					// If timer reaches above 2, unblock this operation
					if (block_timers[i] > 2) {
						blocked_operations = static_cast<Operation>(
							static_cast<u_int8_t>(blocked_operations) & ~(1 << i));
						block_timers[i] = 0;  // Reset timer
					}
				}
			}
		}

		/** @brief Player takes 1 coin from treasury
			* COST: 0
			* can be disabled by: sanction
			@throws runtime_error: illegal move when blocked
		*/
		virtual void gather();

		/** @brief Player takes 2 coins from treasury
			* COST: 0
			* can be disabled by: certain roles/operations
			@throws runtime_error: illegal move when blocked

		*/		
		virtual void tax();

		/** @brief Player pays coins to get an additional action
			* COST: 4
			* can be disabled by: NA
			@throws runtime_error: illegal move when blocked
		*/		
		virtual void bribe();

		/** @brief Player selects a player (not himself) and takes from him 1 coin
		 	* @note: cannot be used twice on the same player in a row
			* COST: 0
			* can be disabled by: NA
		*/		
		virtual void arrest(Player& o);

		/** @brief Player selects a player (not himself) and disables his (gather,tax) actions until this player next turn
			* COST: 3
			* can be disabled by: by SPY only
			@throws runtime_error: If player picked himself to sanction
		*/		
		virtual void sanction(Player& o);

		/** @brief Player selects a player (not himself) and removes him from the game
			* COST: 7
			* can be disabled by: General
			@param o The other player
		*/		
		virtual void coup(Player& o);

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
		 * @note: Only Judge and Gover
		 * COST: 0
		 * @param o The other player we want to undo the action
		 */
		virtual void undo(Player& o);

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

		/**
		 * @brief Checks if the player is in the current game
		 * 
		 * @return true Is playin
		 * @return false Got removed from game
		 */
		virtual bool validate_active();

		/**
		 * @brief Removes from *This* player coins
		 * 
		 * @param n The amount of coins to take
		 * @throw runtime_error: if the player doesnt have the needed money
		 */
		virtual void remove_coins(int n);

		/**
		 * @brief Checks if the players turn
		 * 
		 * @throws runtime_error: Illegal move, tried to activate an illegal operation when not in player turn
		 */
		virtual void isMyTurn();

		/**
		 * @brief Get the Name object
		 * 
		 * @return std::string 
		 */
		virtual std::string getName();

		/**
		 * @brief Sets Active arrtibute of the object
		 * 
		 * @param t 
		 */
		virtual void setActive(bool t);
};
}