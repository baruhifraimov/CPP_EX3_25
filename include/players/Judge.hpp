#pragma once
#include <iostream>
#include "Player.hpp"

// if attacked by 'sanction', player who played sanction pays extra 1 coin to treasury
namespace coup{
class Judge : public Player {
	private:

	public:
		// Constructor
		Judge(Game& game, const std::string& name) : Player(game, name) {
			this->setRole(Role::JUDGE);
		}

		/**	
		* @brief Judge's special ability to block a bribe action
		* 
		* This method allows a Judge to intervene and block an attempted bribe
		* by another player. When called, it prompts the Judge player for 
		* confirmation and, if approved, forces the player who attempted the bribe
		* to pay an additional coin to the treasury as a penalty.
		* 
		* @param o The player attempting the bribe action
		* @param shouldBlock default is false, if true the judge will block
		* @return true If the Judge successfully blocks the bribe
		* @return false If the Judge declines to intervene
		*/
   		bool undo(Player& o, bool shouldBlock = false);
};
}