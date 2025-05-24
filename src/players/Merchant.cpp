#include <iostream>
#include "../../include/players/Merchant.hpp"
#include "../../include/players/Player.hpp"

// if attacked by 'arrest', he looses 2 coins instead of giving 1 coin to the player
// if started turn with >=3 coins, add +=1 coin to his coins 
using namespace coup;
		
		 void Merchant::gather() {

		 }

		 void Merchant::tax() {

		 }

		 void Merchant::bribe() {

		 }
	
		 void Merchant::arrest(Player& o) {

		 }
	
		 void Merchant::sanction(Player& o)  {

		 }
		
		 void Merchant::coup(Player& o) {

		 }

		void Merchant::undo(Player& o){
			
			throw std::runtime_error("This class role undo actions");
			
		}
		
		void Merchant::addCoinsBuff(){
			if(this->coins() >= 3){
				this->addCoins(1);
			}
		}
