#include <iostream>
#include "../../include/players/Player.hpp"

using namespace coup;
		
		void Player::setGame(Game& game){
			this->setGame(game);
		}

		void Player::setName(const std::string& name) { 
			player_name = name; 
		}

		void Player::setRole(Role& r) {
            this->player_role = r;
		}

		Role Player::getRole() {
            return this->player_role;
		}

		 void Player::gather() {
			this->isMyTurn(); // Check if its my turn
			if(!IsOver10Coins()){
				if(is_operation_blocked(Operation::GATHER)){
					throw std::runtime_error("Player is under sanction, illegal move");
				}
				this->current_game.take_coins(1);
				this->addCoins(1);
				if (is_operation_blocked(Operation::EXTRA_TURN))
				{
					unblock_operation(Operation::EXTRA_TURN);
				}
				else{
					this->current_game.next_turn(); // next round
				}
			}
			else{
				throw std::runtime_error("Gather is disabled, got over or equal to 10 coins, illegal move");
			}
		 }

		 void Player::tax() {
			this->isMyTurn(); // Check if its my turn
			if(!IsOver10Coins()){
				if(is_operation_blocked(Operation::TAX)){
					throw std::runtime_error("Player is under sanction, illegal move");
				}
				this->current_game.take_coins(2);
				this->addCoins(2);
				if (is_operation_blocked(Operation::EXTRA_TURN))
				{
					unblock_operation(Operation::EXTRA_TURN);
				}
				else{
					this->current_game.next_turn(); // next round
					}
				}
			else{
				throw std::runtime_error("Tax is disabled, got over or equal to 10 coins, illegal move");
			}
		}

		 void Player::bribe() {
			this->isMyTurn(); // Check if its my turn
			if(!IsOver10Coins()){

				this->addCoins(-4);
				this->current_game.add_coins(4);

				
				// Check with all the judges if they want to block bribe
				if(current_game.check_judge_intervention(*this)){
					// Bribe was blocked by a Judge
					// Coins already spent, loser
					if (is_operation_blocked(Operation::EXTRA_TURN)){
						unblock_operation(Operation::EXTRA_TURN);
						return;
					}
					else{
						this->current_game.next_turn(); // next round
						return;
					}
				}
				else{
					block_operation_with_timer(Operation::EXTRA_TURN);
				}
			}
			else{
				throw std::runtime_error("Bribe is disabled, got over or equal to 10 coins, illegal move");
			}
		 }
	
		 void Player::arrest(Player& o) { 
			this->isMyTurn(); // Check if its my turn
			if(!IsOver10Coins()){
				if(is_operation_blocked(Operation::ARREST)){
					throw std::runtime_error("Player arrest action is disabled, illegal move");
				}
				o.block_operation_with_timer(Operation::ARREST);
				o.addCoins(-1);
				this->addCoins(1);
				if (is_operation_blocked(Operation::EXTRA_TURN))
				{
					unblock_operation(Operation::EXTRA_TURN);
				}
				else{
					this->current_game.next_turn(); // next round
				}
			}
			else{
				throw std::runtime_error("Arrest is disabled, got over or equal to 10 coins, illegal move");
			}
		 }
	
		 void Player::sanction(Player& o)  {
			this->isMyTurn(); // Check if its my turn
			if(!IsOver10Coins()){
				if(&(*this) == &o){ // check if im trying to sanction my self
					throw std::runtime_error("Cannot sanction yourself, illegal move");
				}
				this->addCoins(-3);
				block_operation_with_timer(Operation::GATHER);
				block_operation_with_timer(Operation::TAX);
				this->current_game.add_coins(3);
				if (is_operation_blocked(Operation::EXTRA_TURN))
				{
					unblock_operation(Operation::EXTRA_TURN);
				}
				else{
					this->current_game.next_turn(); // next round
				}
			}
			else{
				throw std::runtime_error("Sanction is disabled, got over or equal to 10 coins, illegal move");
			}
		 }
		
		 void Player::coup(Player& o) {
			this->isMyTurn(); // Check if its my turn
			if(!IsOver10Coins()){

				this->addCoins(-7);
				current_game.add_coins(7);

				// Check with all the generals if they want to undo coup
				if(current_game.check_general_intervention(*this,o)){
					// Coup was blocked by a General
					// Coins already spent, but target is not eliminated
					if (is_operation_blocked(Operation::EXTRA_TURN)){
						unblock_operation(Operation::EXTRA_TURN);
						return;
					}
					else{
						this->current_game.next_turn(); // next round
						return;
					}
				}

				// No General interventio - proceed with coup
				o.is_active = false; // set player non-active
				this->current_game.removePlayer(o.getName());

				if (is_operation_blocked(Operation::EXTRA_TURN))
				{
					unblock_operation(Operation::EXTRA_TURN);
				}
				else{
					this->current_game.next_turn(); // next round
				}
			}
			else{
				throw std::runtime_error("Coup is disabled, got over or equal to 10 coins, illegal move");
			}
		 }

		void Player::undo(Player& o){
			throw std::runtime_error("This class role undo actions"); // default cannot play 'undo'
		}

		bool Player::validate_active(){
			if(!this->is_active){
				throw std::runtime_error("Player " + this->player_name + " is not active");
			}
			else{
				return true;
			}
		}
		
		void Player::addCoins(int n){
			int totalCoins = this->num_coins;
			// Take whats left if (n > num_coins)
			if(totalCoins + n < 0){
				throw std::runtime_error("Player "+this->player_name+" doesn't have enough money");
			}
			this->num_coins +=n;		
		}

		void Player::isMyTurn(){
			if(this->current_game.turn() != this->player_name){
				throw std::runtime_error("Not player turn, Illegal move");
			}
		}

		std::string Player::getName(){
			return this->player_name;
		}

		void Player::setActive(bool t){
			this->is_active = t;
		}

		bool Player::IsOver10Coins(){
			return this->num_coins>=10;
		}