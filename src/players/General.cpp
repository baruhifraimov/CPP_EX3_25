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
            try{
            this->addCoins(-5);
            }catch (const std::exception& e){
                std::cout << "General doesn't have enough coins to block!" << std::endl;
                this->getGame().set_general_intervention(false);
                return false;
            }
            this->getGame().add_coins(5);
            this->getGame().set_general_intervention(true);
            return true;
        
    }
    
    return false;
}
