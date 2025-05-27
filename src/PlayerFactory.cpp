#include <iostream>
#include "../include/players/Player.hpp"
#include "../include/PlayerFactory.hpp"
#include "../include/players/General.hpp"
#include "../include/players/Judge.hpp"
#include "../include/players/Baron.hpp"
#include "../include/players/Governor.hpp"
#include "../include/players/Spy.hpp"
#include "../include/players/Merchant.hpp"


namespace coup{
Player& PlayerFactory::create_player(Game& game, const std::string& name, Role role){
	Player *p = nullptr;
	switch (role){
		case Role::BARON:
			p = new Baron(game,name);
			break;
		case Role::GOVERNOR:
			p = new Governor(game,name);
			break;
		case Role::JUDGE:
			p = new Judge(game,name);
			break;
		case Role::MERCHANT:
			p = new Merchant(game,name);
			break;
		case Role::SPY:
			p = new Spy(game,name);
			break;
		case Role::GENERAL:
			p = new General(game,name);
			break;
		default:
			throw std::invalid_argument("Unknown ROLE input");
	}
	game.registerPlayer(p);
	return *p;
}
}