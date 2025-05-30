// baruh.ifraimov@gmail.com
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
		Merchant(Game& game, const std::string& name) : Player(game, name) {
			this->setRole(Role::MERCHANT);
		}
};
}