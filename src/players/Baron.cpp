// baruh.ifraimov@gmail.com
#include <iostream>
#include "../../include/players/Baron.hpp"
// if attacked by 'sanction', recieve 1 coin; TODO
using namespace coup;
	
		void Baron::invest(){
			this->isMyTurn(); // Check if its my turn
			if(!IsOver10Coins()){
				if(this->coins() < 3){
					throw std::runtime_error("Invest action is illegal, below 3 coins");
				}
				std::cout << "Activating INVEST"<< std::endl;
				this->addCoins(-3);
				this->addCoins(6);
				if (is_operation_blocked(Operation::EXTRA_TURN))
				{
					std::cout << "XTRA TURN";
					unblock_operation(Operation::EXTRA_TURN);
				}
				else{
					this->getGame().next_turn(); // next round
				}
			}
			else{
				throw std::runtime_error("Invest is disabled, got over or equal to 10 coins, illegal move");
			}
		}
