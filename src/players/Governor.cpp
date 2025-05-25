#include <iostream>
#include "../../include/players/Governor.hpp"
#include "../../include/players/Player.hpp"

using namespace coup;

		void Governor::block_tax(Player& o){
			this->isMyTurn(); // Check if its my turn
			if(!IsOver10Coins()){
				this->block_operation_with_timer(Operation::TAX);
				if (is_operation_blocked(Operation::EXTRA_TURN))
				{
					unblock_operation(Operation::EXTRA_TURN);
				}
				else{
					this->getGame().next_turn(); // next round
					}
				}
			else{
				throw std::runtime_error("block_tax is disabled, got over or equal to 10 coins, illegal move");
			}
		}
