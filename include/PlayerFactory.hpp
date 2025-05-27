#pragma once

#include <iostream>

#include "./players/Player.hpp"
#include "./players/Baron.hpp"
#include "./players/Governor.hpp"
#include "./players/Judge.hpp"
#include "./players/Merchant.hpp"
#include "./players/Spy.hpp"


namespace coup{
	class PlayerFactory{
		public:
		/**
		 * @brief Create and register a new Player of the given role.
		 * @param game  The Game instance to add this player to
		 * @param name  The player’s name
		 * @param role  The role enum (Baron, General, Judge, …)
		 * @return Pointer to the newly created Player
		 * @throws std::invalid_argument if role is unrecognized
		 */
		static Player& create_player(Game& game, const std::string& name, Role role);
	};
}