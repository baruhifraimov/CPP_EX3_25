#pragma once
#include <iostream>
#include "Player.hpp"

namespace coup{
class Spy : public Player {
	private:

	public:
		Spy(Game& game, const std::string& name) : Player(game, name) {}

		/** @brief Player takes 1 coin from treasury
			* COST: 0
			* can be disabled by: sanction
		*/
		 void gather() override;

		/** @brief Player takes 2 coins from treasury
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
		
		 /**
		  * @brief Able to view other player coins
		  * NOTE: This operation doesnt cost coins/round 
		  * @param p 
		  */
		 void viewCoins(Player &p);

		 /**
		  * @brief Disables arrest operation on a second player
		  * NOTE: This operation doesnt cost coins/round 
		  * @param p The player we want to disable the 'arrest' op
		  */
		 void disableArrest(Player &p);

		 /**
		 * @brief General cannot undo roles 
		 * 
		 * @param o The other player that he dreams to undo but he can't
		 */
		void undo(Player& o) override {
			throw std::runtime_error("This class role undo actions");
		}

};
}