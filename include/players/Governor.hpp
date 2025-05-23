#pragma once
#include <iostream>
#include "Player.hpp"

namespace coup{
class Governor : public Player {
	private:

	public:
		// Constructor
		Governor(Game& game, const std::string& name) : Player(game, name) {}


		/** @brief Player takes 1 coin from treasury
			* COST: 0
			* can be disabled by: sanction
		*/
		 void gather() override;

		/** @brief Player takes 3 (+1 from base class) coins from treasury
			* COST: 0
			* can be disabled by: certain roles/operations
		*/		
		 void tax() override;

		/** @brief Player pays to get an additional action
			* COST: 2
			* can be disabled by: NA
		*/		
		 void bribe() override;

		/** @brief Player selects a player (not himself) and takes from him 1 coin
		 	* NOTES: cannot be used twice on the same player in a row
			* COST: 0
			* can be disabled by: NA
		*/		
		 void arrest(Player& o) override;

		/** @brief Player selects a player (not himself) and disables his (gather,tax) actions until this player next turn
			* COST: 3
			* can be disabled by: NA
		*/		
		 void sanction(Player& o) override;

		/** * @brief Player selects a player (not himself) and removes him from the game
			* COST: 7
			* can be disabled by: certain conditions
		*/		
		 void coup(Player& p) override;

		 /** * @brief Can *ONLY* disable tax action from oher players
		  * 
		  * @param o The other player we want to disable his tax action
		  */
		 void undo(Player& o) override;
	};
}