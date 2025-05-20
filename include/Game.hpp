#pragma once
#include <iostream>

class Game {
	private:

	public:

		/**
		 * @brief Prints name of the current player round turn
		 * 
		 */
		void turn();

		/**
		 * @brief Retunrs live players name
		 * 
		 * @return vector<string> Players name
		 */
		vector<string> players(){};

		/**
		 * @brief Returns the name of the winner
		 * @throw If game is still live, throw an error
		 * @return std::string Name of the winner 
		 */
		std::string winner(){};

};