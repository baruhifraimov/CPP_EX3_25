#include <iostream>
#include "../../include/players/Judge.hpp"
#include "../../include/players/Player.hpp"

// if attacked by 'sanction', player who played sanction pays extra 1 coin to treasury
using namespace coup;

		bool Judge::undo(Player& o){
			std::cout	<< this->getName() << " (Judge), do you want to block " 
						<< o.getName() << "'s bribe action? (y/n): ";

			char response;
			std::cin >> response;
			
			if (response == 'y' || response == 'Y') {
				// General pays cost to block
				if (this->coins() >= 5) {
					this->addCoins(-5);
					this->getGame().add_coins(5);
					return true;
				} else {
					std::cout << "Judge doesn't have enough coins to block!" << std::endl;
					return false;
				}
			}
			
			return false;
		}
