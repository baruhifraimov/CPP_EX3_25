#include <iostream>
#include "../../include/players/General.hpp"
#include "../../include/players/Player.hpp"

using namespace coup;

		bool General::undo(Player& attacker, Player& target) {
		// Show intervention prompt
		std::cout << this->getName() << " (General), do you want to block " 
				<< attacker.getName() << "'s coup on " << target.getName() 
				<< "? (y/n): ";
				
		char response;
		std::cin >> response;
		
		if (response == 'y' || response == 'Y') {
			// Check if General has enough coins
			if (this->coins() >= 5) {
				this->addCoins(-5);
				this->getGame().add_coins(5);
				std::cout << "General " << this->getName() << " blocks the coup!" << std::endl;
				return true;
			} else {
				std::cout << "You don't have enough coins to block!" << std::endl;
				return false;
			}
		}
		
		return false;
}
