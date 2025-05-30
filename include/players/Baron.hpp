// baruh.ifraimov@gmail.com
#pragma once
#include <iostream>
#include "Player.hpp"
#include "../Role.hpp"

// if attacked by 'sanction', recieve 1 coin; TODO
namespace coup{
class Baron : public Player {
	private:
	
	public:
		// Constructor
		Baron(Game& game, const std::string& name) : Player(game, name) {
			this->setRole(Role::BARON);
		}
		/**
		 * @brief Invest 3 coind to recieve 6 coins from treasury
		 * 
		 */
		void invest();

};
}