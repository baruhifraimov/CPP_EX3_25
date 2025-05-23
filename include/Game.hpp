#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Player.hpp"

#define MIN_PLAYERS 2
#define MAX_PLAYERS 6

namespace coup{
class Game {
	private:
		std::vector<Player*> player_objects; 
		std::vector<std::string> player_names; // (might delete later)
		int cur_round,index;
		int treasury; // coins bank
	public:

		// Constructor
		Game() : player_names(),
				 player_objects(),
				 cur_round(0),
				 index(0),
				 treasury(6) // default 6 coins (cause 6 players max)
				 {
			
		}
		// Copy constructor
		Game(Game &o) : player_names(o.player_names), cur_round(o.cur_round){

		}
		// Destructor
		virtual ~Game(){
			
		}
		// Copy assignment operator
		Game& operator=(Game& o)
			{	// Check self assignment
				if (this == &o){
				return *this;
			}
			
			// Copy data
			this->cur_round = o.cur_round;
			this->player_names = o.player_names;
			
			return *this;

		}

		/**
		 * @brief Prints name of the current player round turn
		 * @throw If there is no players, throw an error
		 */
		std::string turn();

		/**
		 * @brief Returns live players name
		 * 
		 * @return vector<string> Players name
		 */
		std::vector<std::string> players();

		/**
		 * @brief Returns the name of the winner
		 * @throw If game is still live, throw an error
		 * @return std::string Name of the winner 
		 */
		std::string winner();

		/**
		 * @brief Register player object with the game
		 * 
		 * @param player 
		 */
		void registerPlayer(Player* player);

		/**
		 * @brief Adds player name to the current game
		 * 
		 */
		void addPlayer(std::string name);

		/**
		 * @brief Removes the player based on name
		 * 
		 * @param name Player name
		 */
		void removePlayer(std::string name);

		/**
		 * @brief Take coins from the treasury
		 * @param amount Number of coins to take
		 * @throw std::runtime_error If not enough coins in treasury
		 */
		void take_coins(int n);

		/**
		 * @brief Add coins to the treasury
		 * @param amount Number of coins to add
		 */
		void add_coins(int n);

		/**
		 * @brief Goes to the next round
		 * 
		 */
		void next_turn();

		/**
		 * @brief Check with all the existing Judges if they want to disable the current
		 *  coup request by the attacker, if yes general should pay 5 coins and the attacker loses his
		 * coins that he paid for the coup action
		 * 
		 * @param attacker The one who initiated the attack
		 * @param target Unlucky one 
		 * @return true If a prevention initiated by a certain General
		 * @return false else
		 */
		bool check_general_intervention(Player& attacker, Player& target);

		/**
		 * @brief Check with all the existing Judges if they want to disable the current
		 *  bribe request by the attacker, if yes Judge doesn't pay a penny, but the attacker
		 * loses his coins that he spent on the bribe
		 * 
		 * @param attacker The one who initiated the attack
		 * @return true If a prevention initiated by a certain Judge
		 * @return false else
		 */
		bool check_judge_intervention(Player& attacker);

		/**
		 * @brief Get the generals object
		 * 
		 * @return std::vector<Player*> 
		 */
		std::vector<Player*> get_generals();

		/**
		 * @brief Get the judges object
		 * 
		 * @return std::vector<Player*> 
		 */
		std::vector<Player*> get_judges();

		/**
		 * @brief Ask the general if he wants to intervene in the attack
		 * 
		 * @param general 
		 * @param attacker 
		 * @param target 
		 * @return true 
		 * @return false 
		 */
		bool ask_general_intervention(Player& general, Player& attacker, Player& target);

		/**
		 * @brief Ask the judge if he wants to intervene in the attack
		 * 
		 * @param judge 
		 * @param attacker 
		 * @return true 
		 * @return false 
		 */
		bool ask_judge_intervention(Player& judge, Player& attacker);
};
}