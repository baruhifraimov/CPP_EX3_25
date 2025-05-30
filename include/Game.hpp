// baruh.ifraimov@gmail.com
#pragma once

#include <iostream>
#include <vector>
#include <string>
// #include "./players/Player.hpp"

#define MIN_PLAYERS 2
#define MAX_PLAYERS 6
#define INITIAL_TREASURY 9999
namespace coup{

class Player;
class Game {
	private:
		std::vector<Player*> player_objects; 
		std::vector<Player*> dead_player_objects; // because player_objects gave memmory leaks so this will prevent it
		int cur_round,index;
		int treasury; // coins bank
		Player* target_player;
		bool general_intervention;
		bool judge_intervention;
		Player* lastArrestedPlayer; // Pointer to track the last arrested player


	public:

		// Constructor
		Game() : 
				 player_objects(),
				 cur_round(0),
				 index(0),
				 treasury(INITIAL_TREASURY), // default 6 coins (cause 6 players max)
				 target_player(nullptr),
				 general_intervention(false),
				 judge_intervention(false),
				 lastArrestedPlayer(nullptr){

			// Reserve space for maximum number of players to avoid reallocations
			player_objects.reserve(MAX_PLAYERS);
			dead_player_objects.reserve(MAX_PLAYERS);
		}
		// dtor
		~Game();

		// Disable copy constructor and operator=
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

		// Copy assignment operator
		Game& operator=(Game& o)
			{	// Check self assignment
				if (this == &o){
				return *this;
			}
			
			// Copy data
			this->cur_round = o.cur_round;
			this->index = o.index;
			this->player_objects = o.player_objects;
			this->treasury = o.treasury;
			this->target_player = o.target_player;
			this->general_intervention = o.general_intervention;
			this->judge_intervention = o.judge_intervention;
			lastArrestedPlayer = o.lastArrestedPlayer;
			
			return *this;

		}

		/**
		 * @brief Begin play. Must be called once all players have registered.
		 * @throw std::runtime_error if player count is < MIN_PLAYERS or > MAX_PLAYERS
		 */
		void start_game();

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
		 * @brief Register player object to the current game
		 * 
		 * @param player 
		 */
		void registerPlayer(Player* player);


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
		 * @brief Get the players objects vector
		 * 
		 * @return std::vector<Player*> 
		 */
		std::vector<Player*> get_players_objects();

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
		
		/**
		 * @brief Getting the current playing player 
		 * (like turn() but returns a Player object)
		 * 
		 * @return Player& 
		 */
		Player* get_current_player();

		/**
		 * @brief Set the target player object
		 * 
		 * @param o 
		 */
		void set_target_player(Player& o);

		/**
		 * @brief Get the target player object
		 * 
		 * @return Player& 
		 */
		Player& get_target_player();

		/**
		 * @brief resets the game state to its initial configuration, 
		 * preparing it for a new game or restarting the current one.
		 * 
		 */
		void reset();

		/**
		 * @brief Get the general intervention flag
		 * @return true if a general has intervened in the current action
		 * @return false otherwise
		 */
		bool get_general_intervention() const;

		/**
		 * @brief Set the general intervention flag
		 * @param value The new value for the flag
		 */
		void set_general_intervention(bool value);

		/**
		 * @brief Get the judge intervention flag
		 * @return true if a judge has intervened in the current action
		 * @return false otherwise
		 */
		bool get_judge_intervention() const;

		/**
		 * @brief Set the judge intervention flag
		 * @param value The new value for the flag
		 */
		void set_judge_intervention(bool value);

		/**
		 * @brief Get the Last Arrested Player object
		 * 
		 * @return Player* 
		 */
		Player* getLastArrestedPlayer() const;

		/**
		 * @brief Set the Last Arrested Player object
		 * 
		 * @param player 
		 */
		void setLastArrestedPlayer(Player* player);
};
}