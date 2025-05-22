#include <iostream>
#include "../../include/players/Baron.hpp"
#include "../../include/players/Player.hpp"

// if attacked by 'sanction', recieve 1 coin; TODO
using namespace coup;
		
		 void Baron::gather() {

		 }

		 void Baron::tax() {

		 }

		 void Baron::bribe() {

		 }
	
		 void Baron::arrest(Player& o) {

		 }
	
		 void Baron::sanction(Player& o)  {

		 }
		
		 void Baron::coup(Player& p) {

		 }
		
		void Baron::invest(){

		}

		void Baron::undo(Player& o){
			throw std::runtime_error("This class role undo actions");
		}
