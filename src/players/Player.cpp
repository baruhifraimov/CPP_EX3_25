#include <iostream>
#include "../../include/players/Player.hpp"

using namespace coup;
		
		 void Player::gather() {
			this->isMyTurn(); // Check if its my turn
			if(is_operation_blocked(Operation::GATHER)){
				throw std::runtime_error("Player is under sanction, illegal move");
			}
			this->current_game.take_coins(1);
			this->num_coins+=1;
			if (is_operation_blocked(Operation::EXTRA_TURN))
			{
				unblock_operation(Operation::EXTRA_TURN);
			}
			else{
				this->current_game.next_turn(); // next round
			}
		 }

		 void Player::tax() {
			this->isMyTurn(); // Check if its my turn
			if(is_operation_blocked(Operation::TAX)){
				throw std::runtime_error("Player is under sanction, illegal move");
			}
			this->current_game.take_coins(2);
			this->num_coins+=2;
			if (is_operation_blocked(Operation::EXTRA_TURN))
			{
				unblock_operation(Operation::EXTRA_TURN);
			}
			else{
				this->current_game.next_turn(); // next round
			}
		 }

		 void Player::bribe() {
			this->isMyTurn(); // Check if its my turn
			if(is_operation_blocked(Operation::BRIBE)){
				throw std::runtime_error("Player bribe action is disabled, illegal move");
			}
			else{
				// TODO The bribe allows the player to play another action!
				this->remove_coins(4);
				this->current_game.add_coins(4);
				this->blocked_operations |= Operation::EXTRA_TURN;
			}
		 }
	
		 void Player::arrest(Player& o) { //REDO THE METHOD, NEED TO CHECK IF THE PLAYER IS ALREADY ARRESTED, CANNOT DO ARREST TWICE RAPIDLY
			this->isMyTurn(); // Check if its my turn
			if(is_operation_blocked(Operation::ARREST)){
				throw std::runtime_error("Player arrest action is disabled, illegal move");
			}
			o.block_operation_with_timer(Operation::ARREST);
			o.remove_coins(1);
			this->num_coins +=1;
			if (is_operation_blocked(Operation::EXTRA_TURN))
			{
				unblock_operation(Operation::EXTRA_TURN);
			}
			else{
				this->current_game.next_turn(); // next round
			}
		 }
	
		 void Player::sanction(Player& o)  {
			this->isMyTurn(); // Check if its my turn
			if(&(*this) == &o){ // check if im trying to sanction my self
				throw std::runtime_error("Cannot sanction yourself, illegal move");
			}
			this->remove_coins(3);
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
		
		 void Player::coup(Player& o) {
			this->isMyTurn(); // Check if its my turn
			this->remove_coins(7);
			current_game.add_coins(7);
			if(is_operation_blocked(Operation::COUP)){
				// i dont think we need to throw an error, we need to see how to prevent this
				// if coup disabled, player still pays coins
				throw std::runtime_error("Play COUP action is blocked by General, illegal move");
			}
			o.is_active = false; // set player non-active
			this->current_game.removePlayer(o.player_name);
			current_game.next_turn();
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
		
		void Player::remove_coins(int n){
			int totalCoins = this->num_coins;
			// Take whats left if (n > num_coins)
			if(totalCoins - n < 0){
				throw std::runtime_error("Player "+this->player_name+" doesn't have enough money");
			}
			this->num_coins -=n;		
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