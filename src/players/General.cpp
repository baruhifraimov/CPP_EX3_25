#include <iostream>
#include "../../include/players/General.hpp"
#include "../../include/players/Player.hpp"

using namespace coup;

bool General::undo(Player& attacker, Player& target, bool shouldBlock) {
    std::cout << this->getName() << " (General), blocking " 
              << attacker.getName() << "'s coup on " << target.getName() 
              << ": " << (shouldBlock ? "YES" : "NO") << std::endl;
    
    if (shouldBlock) {
        // General pays cost to block
        if (this->coins() >= 5) {
            this->addCoins(-5);
            this->getGame().add_coins(5);
            return true;
        } else {
            std::cout << "General doesn't have enough coins to block!" << std::endl;
            return false;
        }
    }
    
    return false;
}
