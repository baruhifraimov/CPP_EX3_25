#include <iostream>
#include "../../include/players/General.hpp"
#include "../../include/players/Player.hpp"

// if attacked by 'arrest', recieve 1 coin; TODO
using namespace coup;
		
		 void General::gather() {

		 }

		 void General::tax() {

		 }

		 void General::bribe() {

		 }
	
		 void General::arrest(Player& o) {

		 }
	
		 void General::sanction(Player& o)  {

		 }
		
		 void General::coup(Player& o) {

		 }
		
		void General::preventCoup(Player& o){

		}

		void General::undo(Player& o){
			throw std::runtime_error("This class role undo actions");
		}
