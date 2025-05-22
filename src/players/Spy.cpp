#include <iostream>
#include "../../include/players/Spy.hpp"
#include "../../include/players/Player.hpp"

using namespace coup;
		
		 void Spy::gather() {

		 }

		 void Spy::tax() {

		 }

		 void Spy::bribe() {

		 }
	
		 void Spy::arrest(Player& o) {

		 }
	
		 void Spy::sanction(Player& o)  {

		 }
		
		 void Spy::coup(Player& o) {

		 }

		void Spy::viewCoins(Player &p){

		}

		void Spy::disableArrest(Player &p){

		}

		void Spy::undo(Player& o){
			
			throw std::runtime_error("This class role undo actions");
			
		}
