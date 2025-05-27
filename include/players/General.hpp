#pragma once
#include <iostream>
#include "Player.hpp"

// if attacked by 'arrest', recieve 1 coin; TODO
namespace coup{
class General : public Player {
	private:
	
	public:
		// Constructor
		General(Game& game, const std::string& name) : Player(game, name) {
			this->setRole(Role::GENERAL);
		}

		/**
		 * @brief General's special ability to block a coup action
		 * 
		 * This method allows a General to intervene and block an attempted coup
		 * by another player. When called, it prompts the General player for 
		 * confirmation and, if approved, deducts 5 coins from the General's 
		 * treasury as the cost of intervention.
		 * 
		 * @param attacker The player attempting the coup action
		 * @param target The player being targeted by the coup
		 * @param shouldBlock default is false, if true the judge will block
		 * @return true If the General successfully intervenes (has enough coins and confirms)
		 * @return false If the General declines to intervene or lacks sufficient coins
		 * 
		 */
		bool undo(Player& attacker, Player& target, bool shouldBlock = false);


};
}