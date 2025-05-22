#pragma once
#include <iostream>
#include "Player.hpp"

// if attacked by 'arrest', recieve 1 coin; TODO
namespace coup{
class General : public Player {
	private:
	
	public:
	
		General(Game& game, const std::string& name) : Player(game, name) {}

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
			* @param o The other player we select
			* can be disabled by: NA
		*/		
		 void arrest(Player& o) override;

		/** @brief Player selects a player (not himself) and disables his (gather,tax) actions until this player next turn
			* COST: 3
			* @param o The other player we select
			* can be disabled by: NA
		*/		
		 void sanction(Player& o) override;

		/** * @brief Player selects a player (not himself) and removes him from the game
			* COST: 7
			* can be disabled by: certain conditions
		 	* @param o The other player we select
		*/		
		 void coup(Player& o) override;
		
		/**
		 * @brief Prevents player coup (can be himself)
		 * NOTE: The player that activated coup loses his coins that he paid for the coup op
		 * COST: 5
		 * @param o The other player we select
		 */
		void preventCoup(Player& o);
		
		/**
		 * @brief General cannot undo roles 
		 * 
		 * @param o The other player that he dreams to undo but he can't
		 */
		void undo(Player& o) override;

};
}