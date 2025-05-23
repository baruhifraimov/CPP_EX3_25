#pragma once
#include <iostream>
#include "Player.hpp"

// if attacked by 'arrest', he looses 2 coins instead of giving 1 coin to the player
// if started turn with >=3 coins, add +=1 coin to his coins 
namespace coup{
class Merchant : public Player {
	private:

	public:
		// Constructor
		Merchant(Game& game, const std::string& name) : Player(game, name) {}


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
		 * @brief General cannot undo roles 
		 * 
		 * @param o The other player that he dreams to undo but he can't
		 */
		void undo(Player& o) override;

		/**
		 * @brief Merchant special sauce to add an additional coin to his pocket
		 * Checks merchant total coins, if >= 3, add +1, else do nothing
		 */
		void addCoinsBuff();
};
}