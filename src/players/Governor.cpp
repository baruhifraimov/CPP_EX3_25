#include <iostream>
#include "../../include/players/Governor.hpp"
#include "../../include/players/Player.hpp"

// if attacked by 'sanction', recieve 1 coin; TODO
using namespace coup;
		
		 void Governor::gather() {

		 }

		 void Governor::tax() {

		 }

		 void Governor::bribe() {

		 }
	
		 void Governor::arrest(Player& o) {

		 }
	
		 void Governor::sanction(Player& o)  {

		 }
		
		 void Governor::coup(Player& p) {

		 }

		void Governor::undo(Player& o){
			throw std::runtime_error("This class role undo actions");
		}
