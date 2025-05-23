#pragma once

// can be up to 6 players, default is 2
// if player got 10 coins, he needs to use 'coup'
#include <iostream>
#include "Operations.hpp"
#include "Game.hpp"
#include "Role.hpp"

namespace coup{
class Player{
	private:
		std::string player_name;
		int num_coins;
		Game &current_game;
		Operation blocked_operations;
		bool is_active;
		Role player_role;

	public:
		// Constructor
		Player(Game& game,std::string name): 
											player_name(name), 
											num_coins(0), 
											current_game(game),
											blocked_operations(Operation::NONE),
											is_active(true),
											player_role() {

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
		 * @brief Set the Game object to the player
		 * 
		 * @param game Game object
		 */
		void setGame(Game& game);

		/**
		 * @brief Get the player Active field
		 * 
		 * @return true If the player still playing
		 * @return false Else
		 */
		bool getActive();

		/**
		 * @brief Set player name
		 * @param name New player name
		 */
		void setName(const std::string& name);

		/**
		 * @brief Set the player Role
		 * 
		 * @param r Role
		 */
        void setRole(Role& r);

		/**
		 * @brief Get the player Role
		 * 
		 * @param r Role
		 */
        Role getRole();

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
		 * @brief Retrieves the coins of the current player
		 * 
		 * @return int num of coins
		 */
		virtual int coins() const{
			return this->num_coins;
		}

		/**
		 * @brief Undo last Player action
		 * @note: Only Judge and Governor
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
		 * @brief Adds from *This* player coins
		 * 
		 * @param n The amount of coins (can be negative)
		 * @throw runtime_error: if the player doesn't have the needed money (on negative val)
		 */
		virtual void addCoins(int n);


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
		 * @brief Sets Active attribute of the object
		 * 
		 * @param t 
		 */
		virtual void setActive(bool t);

		/**
		 * @brief Checks if the player got >=10 coins
		 * 
		 * @return returns true if over or equal to 10 else false
		 */
		virtual bool IsOver10Coins();

	// Protected methods, hidden from the public
	protected:
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
		 * Initializes timer with 1 for each new block
		 * @param op The operation that we activated the block
		 */
		void block_operation_with_timer(Operation op) {
			blocked_operations |= op;
			
			// Initialize timer for each bit that's set
			for (int i = 0; i < 8; i++) {
				// Check if this bit is set in op
				if ((static_cast<u_int8_t>(op) & (1 << i)) != 0 & i != 7) {
					// Check if ARREST attribute in operation is ON, means someone activated arrest this round on this exact player
					// which results an illegal move
					if(i == 3 && block_timers[i] == 1){
						throw std::runtime_error("Player cannot be arrested twice in a lap, illegal move");
					}
					block_timers[i] = 1;  // Start timer at 1
				}
			}
		}	

	};
}