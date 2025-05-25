#include <iostream>
#include "../../include/players/Spy.hpp"
#include "../../include/players/Player.hpp"

using namespace coup;

		 void Spy::view_coins(Player &p){
			std::cout << "Player: " + p.player_name + "has " 
					  << p.num_coins << " coins" << std::endl;
		}

		void Spy::disable_arrest(Player &p){
			this->isMyTurn(); // Check if its my turn
			if(!IsOver10Coins()){
				
				if (is_operation_blocked(Operation::EXTRA_TURN))
				{
					unblock_operation(Operation::EXTRA_TURN);
				}
				else{
					this->getGame().next_turn(); // next round
				}
			}
			else{
				throw std::runtime_error("Gather is disabled, got over or equal to 10 coins, illegal move");
			}
		}
