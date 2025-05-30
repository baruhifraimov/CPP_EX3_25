// baruh.ifraimov@gmail.com
#pragma once
#include <iostream>
#include "Player.hpp"

namespace coup{
class Spy : public Player {
	private:

	public:
		// Constructor
		Spy(Game& game, const std::string& name) : Player(game, name) {
			this->setRole(Role::SPY);
		}
		
		 /**
		  * @brief Able to view other player coins
		  * NOTE: This operation doesnt cost coins/round 
		  * @param p 
		  */
		 void view_coins(Player &p);

		 /**
		  * @brief Disables arrest operation on a second player
		  * NOTE: This operation doesnt cost coins/round 
		  * @param p The player we want to disable the 'arrest' op
		  * @throw runtime_error when trying to disable yourself
		  * @throw when coins are over or equal to 10
		  */
		 void disable_arrest(Player &p);

};
}