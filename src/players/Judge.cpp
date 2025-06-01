// baruh.ifraimov@gmail.com
#include <iostream>
#include "../../include/players/Judge.hpp"
#include "../../include/players/Player.hpp"

// if attacked by 'sanction', player who played sanction pays extra 1 coin to treasury
using namespace coup;

bool Judge::undo(Player& o, bool shouldBlock) {
    std::cout << this->getName() << " (Judge), blocking " 
              << o.getName() << "'s bribe action: " << (shouldBlock ? "YES" : "NO") << std::endl;
              std::cout << "Activating JUDGE UNDO"<< std::endl;

    if (shouldBlock) {
            this->getGame().set_judge_intervention(true);
            
            o.unblock_operation(Operation::EXTRA_TURN);
            return true;
        } else {
            this->getGame().set_judge_intervention(false);
            return false;
        }
    }