// baruh.ifraimov@gmail.com
#include <iostream>
#include "../../include/players/Player.hpp"

using namespace coup;
		
		void Player::setGame(Game& game){
			current_game = &game;
		}

		void Player::setName(const std::string& name) { 
			player_name = name; 
		}

		void Player::setRole(Role r) {
            this->player_role = r;
		}

		Role Player::getRole() {
            return this->player_role;
		}

		 void Player::gather() {
			std::cout << "Activating gather";
			this->isMyTurn(); // Check if its my turn
			if(!IsOver10Coins()){
				if(is_operation_blocked(Operation::GATHER)){
					throw std::runtime_error("Player Gather is blocked, illegal move");
				}
				this->current_game->add_coins(-1);
				this->addCoins(1);
				if (is_operation_blocked(Operation::EXTRA_TURN))
				{
					unblock_operation(Operation::EXTRA_TURN);
				}
				else{
					this->current_game->next_turn(); // next round
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
					throw std::runtime_error("Player Tax is blocked, illegal move");
				}
				if(this->player_role == Role::GOVERNOR){
					this->current_game->add_coins(-3);
					this->addCoins(3);
				}
				else{
					this->current_game->add_coins(-2);
					this->addCoins(2);
				}
				if (is_operation_blocked(Operation::EXTRA_TURN))
				{
					unblock_operation(Operation::EXTRA_TURN);
				}
				else{
					this->current_game->next_turn(); // next round
					}
				}
			else{
				throw std::runtime_error("Tax is disabled, got over or equal to 10 coins, illegal move");
			}
		}

		 void Player::bribe() {
			this->isMyTurn(); // Check if its my turn
			if(this->coins()<4){
				throw std::runtime_error("Not enough coins to execute Bribe");
			}
			if(!IsOver10Coins()){

				this->addCoins(-4);
				this->current_game->add_coins(4);

				
				// Check with all the judges if they want to block bribe

				if(current_game->get_judge_intervention()){
					// Bribe was blocked by a Judge
					// Coins already spent, loser
					unblock_operation(Operation::EXTRA_TURN);
					this->current_game->next_turn(); // next round
					return;
				}
					// Bribe succeeded - grant extra turn
					std::cout << "DIDNT BLOCK " << current_game->turn() << std::endl;
					block_operation_with_timer(Operation::EXTRA_TURN);
				
			}
			else{
				throw std::runtime_error("Bribe is disabled, got over or equal to 10 coins, illegal move");
			}
		 }
	
		 void Player::arrest(Player& o) { 
			this->isMyTurn(); // Check if its my turn
			if(!IsOver10Coins()){

				if(is_operation_blocked(Operation::DISABLE_ARREST)){
					throw std::runtime_error("Player arrest action is disabled, illegal move");
				}

				// Check if this player was the last arrested player
				if (current_game->getLastArrestedPlayer() == &o) {
					throw std::runtime_error("Cannot arrest the same player twice in succession");
				}

				 // Set this player as the last arrested player
				 current_game->setLastArrestedPlayer(&o);

				o.block_operation_with_timer(Operation::ARREST);

				// If the player we want to attack is Merchant, take 2 and add to bank
				if(o.getRole() == Role::MERCHANT){
					o.addCoins(-2);
					current_game->add_coins(2);
				}
				else if(o.getRole() == Role::GENERAL){
					// SIMULATING -- 
					o.addCoins(-1);
					this->addCoins(1);
					// Giving back to the general the money that been taken from him
					o.addCoins(1);
					current_game->add_coins(-1);
				}
				else{
					o.addCoins(-1);
					this->addCoins(1);
				}

				if (is_operation_blocked(Operation::EXTRA_TURN)){
				unblock_operation(Operation::EXTRA_TURN);
				}
				else{
					this->current_game->next_turn(); // next round
				}
			}
			else{
				throw std::runtime_error("Arrest is disabled, got over or equal to 10 coins, illegal move");
			}
		 }
	
		 void Player::sanction(Player& o)  {
			this->isMyTurn(); // Check if its my turn
			if(this->coins()<3){
				throw std::runtime_error("Not enough coins to execute Sanction");
			}
			if(!IsOver10Coins()){
				if(&(*this) == &o){ // check if im trying to sanction my self
					throw std::runtime_error("Cannot sanction yourself, illegal move");
				}
				o.block_operation_with_timer(Operation::GATHER);
				o.block_operation_with_timer(Operation::TAX);

				// Check if Baron, if yes use his special ability
				// If casted on Baron, give him one additional coin fr0m bank
				if(o.getRole() == Role::BARON){
					this->addCoins(-3);
					this->current_game->add_coins(3);	
					o.addCoins(1);
					current_game->add_coins(-1);
					
				}
				// Check if Judge, if yes use his special ability
				// If casted on Judge, pay extra coin to treasury 
				else if(o.getRole() == Role::JUDGE){
					this->addCoins(-4);
					this->current_game->add_coins(4);
				}
				else{
					this->addCoins(-3);
					this->current_game->add_coins(3);	
				}

				if (is_operation_blocked(Operation::EXTRA_TURN))
				{
					unblock_operation(Operation::EXTRA_TURN);
				}
				else{
					this->current_game->next_turn(); // next round
				}
				
			}
			else{
				throw std::runtime_error("Sanction is disabled, got over or equal to 10 coins, illegal move");
			}
		 }
		
		 void Player::coup(Player& o) {
            this->isMyTurn(); // Check if its my turn

			// --- NOTED BECAUSE WE USE THE GUI TO PAY THROUGH THE PLAYER, SECOND FUNCTION IS PAY-COUP ---
			// --- SHOW CASING THE NOTE FOR REFERENCE ---
            // if(this->coins()<7){
            //     throw std::runtime_error("Not enough coins to execute Coup");
            // }
            // this->addCoins(-7); // Always deduct coins for attempting coup
            // current_game->add_coins(7); // Coins go to treasury or are just removed

            // Check if a general intervention has ALREADY been flagged as successful
            if(current_game->get_general_intervention()){
                // Coup was blocked (flag was set by Window.cpp). Coins are spent.
                // Turn advancement should be handled by Window.cpp when the block occurs.
                if (is_operation_blocked(Operation::EXTRA_TURN)){ // This logic might be for other scenarios
                    unblock_operation(Operation::EXTRA_TURN);
                }
					this->current_game->next_turn(); // Blocked coup, next player's turn
            }
            else{
                // No successful general intervention was flagged. Proceed with elimination.
                o.is_active = false; // set player non-active
                this->current_game->removePlayer(o.getName());

                if (is_operation_blocked(Operation::EXTRA_TURN)) {
                    unblock_operation(Operation::EXTRA_TURN);
                    // Player gets an extra turn, so don't call next_turn here.
                }
                else{
                    this->current_game->next_turn(); // Normal coup, next player's turn
                }
            }
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
				throw std::runtime_error("Player " + this->player_name + " doesn't have enough money");
			}
			this->num_coins +=n;		
		}

		void Player::isMyTurn(){
			if(this->current_game->turn() != this->player_name){
				throw std::runtime_error("[" + to_string(this->player_role) + ":" + this->player_name + "] Not player turn, Illegal move");
			}
		}

		std::string Player::getName(){
			return this->player_name;
		}

		void Player::setActive(bool t){
			this->is_active = t;
		}

		bool Player::getActive(){
			return this->is_active;
		}

		bool Player::IsOver10Coins(){
			return this->num_coins>=10;
		}

		Game& Player::getGame(){
			return *this->current_game;
		}

		bool Player::is_operation_blocked(Operation op) {
			return (static_cast<u_int8_t>(blocked_operations) & static_cast<u_int8_t>(op)) != 0;
		}

		void Player::unblock_operation(Operation op) {
			blocked_operations = static_cast<Operation>(static_cast<u_int8_t>(blocked_operations) & ~static_cast<u_int8_t>(op));
		}

		void Player::block_operation_with_timer(Operation op) {
					blocked_operations |= op;
			
			// Initialize timer for each bit that's set
			for (int i = 0; i < 8; i++) {
				// Check if this bit is set in op
				if ((static_cast<u_int8_t>(op) & (1 << i)) != 0 & i != 7) {
					// Check if ARREST attribute in operation is ON, means someone activated arrest this round on this exact player
					// which results an illegal move
					if(i == 3 && block_timers[i] == 1){
						throw std::runtime_error("Player cannot be arrested twice in a lap, illegal move");
					}
					block_timers[i] = 1;  // Start timer at 1
				}
			}
		}
		int Player::coins() const {
			return this->num_coins;
		}

		void Player::update_block_timers(){
			for (int i = 0; i < 8; i++) {
				// If this bit is blocked and has a timer
				if ((static_cast<u_int8_t>(blocked_operations) & (1 << i)) != 0 && block_timers[i] > 0) {
					block_timers[i]++;  // Increment timer
					
					// If timer reaches above 2, unblock this operation
					if (block_timers[i] > 2) {
						blocked_operations = static_cast<Operation>(
							static_cast<u_int8_t>(blocked_operations) & ~(1 << i));
						block_timers[i] = 0;  // Reset timer
					}
				}
			}
		}

		void Player::pay_coup_cost(Player& target) {
			this->isMyTurn(); // Ensure it's the player's turn
			if (this->coins() < 7) {
				throw std::runtime_error("Not enough coins (need 7) to attempt Coup");
			}
			else if(this == &target){
				throw std::runtime_error("Cannot coup yourself!");
			}
			this->addCoins(-7);         // Deduct cost from player
			current_game->add_coins(7); // Add coins to treasury (or however it's handled)
		}