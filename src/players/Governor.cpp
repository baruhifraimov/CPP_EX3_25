// baruh.ifraimov@gmail.com
#include <iostream>
#include "../../include/players/Governor.hpp"
#include "../../include/players/Player.hpp"

using namespace coup;

	void Governor::block_tax(Player& o) {
		// Check if it's this Governor's turn
		this->isMyTurn();
		if(!IsOver10Coins()){
		// Make sure target isn't the Governor itself
		if (&o == this) {
			throw std::runtime_error("Cannot block your own tax, illegal move");
		}
		
		// Block the target's TAX operation using the existing mechanism
		o.block_operation_with_timer(Operation::TAX);
		
		// Handle turn advancement
		if (is_operation_blocked(Operation::EXTRA_TURN)) {
			unblock_operation(Operation::EXTRA_TURN);
		} else {
			this->getGame().next_turn(); // next round
		}
	}
	}
