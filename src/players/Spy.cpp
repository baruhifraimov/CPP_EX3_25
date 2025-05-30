// baruh.ifraimov@gmail.com
#include <iostream>
#include "../../include/players/Spy.hpp"
#include "../../include/players/Player.hpp"

using namespace coup;

		 void Spy::view_coins(Player &p){
			this->isMyTurn(); // Check if its my turn
			std::cout << "Player: " + p.player_name + "has " 
					  << p.num_coins << " coins" << std::endl;
		}

		void Spy::disable_arrest(Player &o){
			this->isMyTurn(); // Check if its my turn
			if(!IsOver10Coins()){
				// Block the target's TAX operation using the existing mechanism
				if(&(*this) == &o){ // check if im trying to sanction my self
					throw std::runtime_error("Cannot Disable Arrest yourself, illegal move");
				}
				o.block_operation_with_timer(Operation::DISABLE_ARREST);
			}
			else{
				throw std::runtime_error("Disable Arrest is disabled, got over or equal to 10 coins, illegal move");
			}
		}
