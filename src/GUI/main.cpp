
// Game headers - adjust paths as needed if they're in a different directory
#include "../../include/players/Player.hpp"
#include "../../include/players/Governor.hpp"
#include "../../include/players/Spy.hpp"
#include "../../include/players/Baron.hpp"
#include "../../include/players/General.hpp"
#include "../../include/players/Judge.hpp"
#include "../../include/players/Merchant.hpp"
#include "../../include/Game.hpp"

// Standard library headers
#include <iostream>
#include <vector>
#include <stdexcept>
#include <exception>

// SFML headers
#include <SFML/Graphics.hpp>

int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Main loop: runs until the window is closed
    while (window.isOpen()) {
        sf::Event event;
        // Process events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Clear the screen with a color
        window.clear(sf::Color::Black);

        // Draw something simple: a circle
        sf::CircleShape circle(50.f);
        circle.setFillColor(sf::Color::Green);
        circle.setPosition(375.f, 275.f);
        window.draw(circle);

        // Display what was drawn
        window.display();
    }

    return 0;
}