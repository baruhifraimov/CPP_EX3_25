#include <iostream>
#include "../../include/players/Judge.hpp"
#include "../../include/players/Player.hpp"

// if attacked by 'sanction', player who played sanction pays extra 1 coin to treasury
using namespace coup;

bool Judge::undo(Player& o, bool shouldBlock) {
    std::cout << this->getName() << " (Judge), blocking " 
              << o.getName() << "'s bribe action: " << (shouldBlock ? "YES" : "NO") << std::endl;
    
    if (shouldBlock) {
            return true;
        } else {
            return false;
        }
    }