#include "../../include/GUI/Window.hpp"

#include "../../include/players/Player.hpp"
#include "../../include/players/General.hpp"
#include "../../include/players/Judge.hpp"
#include "../../include/players/Baron.hpp"
#include "../../include/players/Governor.hpp"
#include "../../include/players/Spy.hpp"

#include "../../include/Game.hpp"
#include "../../include/Role.hpp"
#include "../../include/Operations.hpp"
#include "../../include/PlayerFactory.hpp"

#include <ctime> 
#include <iostream>
#include <cstdlib> //for rand()
#include <random>

const sf::Color CREAM_TEXT(0xF5, 0xF1, 0xD0); // #F5F1D0 - Soft ivory/cream color

using namespace coup;

Window::Window(Game& current_game) : window(sf::VideoMode(768,672), "COUP Game"),current_game(&current_game) {
	
    if (!font.loadFromFile("./src/GUI/fonts/PressStart2P-Regular.ttf")) {
        std::cerr << "Error: Could not load PressStart2P-Regular.ttf font!" << std::endl;
        std::cerr << "Please make sure the font file is in the fonts/ directory." << std::endl;
        // Try to continue without custom font - SFML will use default
    }
    initializeComponents();
}

Window::~Window() {
}

void Window::initializeComponents() {
    // Title
    titleText.setFont(font);
    titleText.setString("COUP");
    titleText.setCharacterSize(72);
    titleText.setFillColor(CREAM_TEXT);
    titleText.setPosition(350, 50);
	{
		sf::FloatRect tb = titleText.getLocalBounds();
		titleText.setOrigin(tb.left + tb.width/2.f,
							tb.top  + tb.height/2.f);
		// place it at half the window width, y=50
		titleText.setPosition(window.getSize().x/2.f, 50.f);
	  }
    
	// START button with modern design (in initializeComponents())
	startButton.setSize(sf::Vector2f(200, 60));
	startButton.setFillColor(sf::Color(59, 89, 152)); // Facebook blue color (same as Play Again)
	startButton.setOutlineThickness(2);
	startButton.setOutlineColor(sf::Color(255, 255, 255)); // White outline
	startButton.setPosition(window.getSize().x/2.f - 220, 250); // Positioned like GameOver buttons
		
	startButtonText.setFont(font);
	startButtonText.setString("START");
	startButtonText.setCharacterSize(24);
	startButtonText.setFillColor(CREAM_TEXT); // White text on blue background
	centerTextInButton(startButtonText, startButton);
		
	// EXIT button with modern design
	exitButton.setSize(sf::Vector2f(200, 60));
	exitButton.setFillColor(sf::Color(220, 53, 69)); // Bootstrap danger red (same as GameOver)
	exitButton.setOutlineThickness(2);
	exitButton.setOutlineColor(sf::Color(255, 255, 255)); // White outline
	exitButton.setPosition(window.getSize().x/2.f + 20, 250); // Positioned like GameOver buttons
		
	exitButtonText.setFont(font);
	exitButtonText.setString("EXIT");
	exitButtonText.setCharacterSize(24);
	exitButtonText.setFillColor(CREAM_TEXT); // White text on red background
	centerTextInButton(exitButtonText, exitButton);
    
    // Name and email at bottom
    nameText.setFont(font);
    nameText.setString("Baruh Ifraimov");
    nameText.setCharacterSize(20);
    nameText.setFillColor(CREAM_TEXT);
    nameText.setPosition(50, 520);
    
    emailText.setFont(font);
    emailText.setString("baruh.ifraimov@gmail.com");
    emailText.setCharacterSize(20);
    emailText.setFillColor(CREAM_TEXT);
    emailText.setPosition(50, 550);

	// — AskCount setup —
	promptText.setFont(font);
	promptText.setString("How many players? (2-6)");
	promptText.setCharacterSize(24);
	promptText.setFillColor(CREAM_TEXT);

	// Center the prompt text horizontally
	sf::FloatRect promptBounds = promptText.getLocalBounds();
	promptText.setOrigin(promptBounds.left + promptBounds.width/2.f, 0);
	promptText.setPosition(window.getSize().x/2.f, 120);

	inputText.setFont(font);
	inputText.setCharacterSize(24);
	inputText.setFillColor(sf::Color::Yellow);

	// Center the input text under the prompt
	sf::FloatRect inputBounds = inputText.getLocalBounds();
	inputText.setOrigin(0, 0); // Reset origin since we'll center dynamically in render
	inputText.setPosition(window.getSize().x/2.f, 160);

	errorText.setFont(font);
	errorText.setCharacterSize(20);
	errorText.setFillColor(sf::Color::Red);
	// Center the error text horizontally and position it in the bottom half
	sf::FloatRect errorBounds = errorText.getLocalBounds();
	errorText.setOrigin(errorBounds.left + errorBounds.width/2.f, errorBounds.top + errorBounds.height/2.f);
	errorText.setPosition(window.getSize().x/2.f, 400);

	// --- AskNames setup ---
	namePrompt.setFont(font);
	namePrompt.setCharacterSize(24);
	namePrompt.setFillColor(CREAM_TEXT);

	// Center the name prompt text horizontally
	sf::FloatRect namePromptBounds = namePrompt.getLocalBounds();
	namePrompt.setOrigin(namePromptBounds.left + namePromptBounds.width/2.f, 0);
	namePrompt.setPosition(window.getSize().x/2.f, 120);

	nameInputText.setFont(font);
	nameInputText.setCharacterSize(24);
	nameInputText.setFillColor(sf::Color::Yellow);

	// Center the name input text horizontally
	nameInputText.setOrigin(0, 0); // Reset origin since we'll center dynamically in handleEvents
	nameInputText.setPosition(window.getSize().x/2.f - 150, 160);
    
	// Restart button for AskNames screen
	restartButton.setSize(sf::Vector2f(120, 40));
	restartButton.setFillColor(sf::Color(59, 89, 152)); // Same blue as Play Again button
	restartButton.setOutlineThickness(2);
	restartButton.setOutlineColor(sf::Color(255, 255, 255)); // White outline
	restartButton.setPosition(window.getSize().x - 140, 20); // Top right corner
	
	restartButtonText.setFont(font);
	restartButtonText.setString("RESTART");
	restartButtonText.setCharacterSize(16);
	restartButtonText.setFillColor(CREAM_TEXT); // Same cream text color
	centerTextInButton(restartButtonText, restartButton);
	
    // NEXT button (hidden until last name entered)
	nextButton.setSize({150, 50});  // Slightly wider for retro look
	nextButton.setFillColor(sf::Color(59, 89, 152));  // Classic blue color
	nextButton.setOutlineThickness(2);  // Thick border for retro pixel look
	nextButton.setOutlineColor(sf::Color(255, 255, 255));  // Light blue border

	// Center the button horizontally
	nextButton.setPosition((window.getSize().x - 150) / 2, 400);  // Centered based on window width

	// Create "3D" effect with a shadow button underneath
	sf::RectangleShape nextButtonShadow;
	nextButtonShadow.setSize({150, 50});
	nextButtonShadow.setFillColor(sf::Color(25, 25, 100));  // Darker blue for shadow
	nextButtonShadow.setPosition((window.getSize().x - 150) / 2 + 4, 404);  // Offset for shadow effect

	// label
	nextButtonText.setFont(font);
	nextButtonText.setString("NEXT");
	nextButtonText.setCharacterSize(24);
	nextButtonText.setFillColor(CREAM_TEXT);  // White text stands out better on blue
	nextButtonText.setStyle(sf::Text::Bold);  // Bold text for emphasis
	{
	auto tb = nextButtonText.getLocalBounds();
	nextButtonText.setOrigin(tb.left+tb.width/2, tb.top+tb.height/2);
	auto bp = nextButton.getPosition();
	auto bs = nextButton.getSize();
	nextButtonText.setPosition(bp.x+bs.x/2, bp.y+bs.y/2);
	}

	// --- Play screen setup ---
	// Current player role display (right side)
	currentPlayerRoleText.setFont(font);
	currentPlayerRoleText.setCharacterSize(28);
	currentPlayerRoleText.setFillColor(CREAM_TEXT);
	currentPlayerRoleText.setPosition(450, 120);

	// Error message display
	errorMessageText.setFont(font);
	errorMessageText.setCharacterSize(16);
	errorMessageText.setFillColor(sf::Color::Red);
	errorMessageText.setPosition(50, 430);  // Positioned just above action buttons

	// Action prompt box (center bottom)
	actionPromptBox.setSize({400, 60});  // Made slightly smaller height
	actionPromptBox.setFillColor(sf::Color(30, 30, 30));  // Darker background
	actionPromptBox.setOutlineThickness(2);
	actionPromptBox.setOutlineColor(sf::Color::Red);
	actionPromptBox.setPosition(184, 600);  // Moved down near bottom of screen (672px height)

	actionPromptText.setFont(font);
	actionPromptText.setCharacterSize(18);
	actionPromptText.setFillColor(CREAM_TEXT);
	// Use the centerTextInButton helper to properly center text in the box
	centerTextInButton(actionPromptText, actionPromptBox);
	setActionPrompt("CHOOSE ACTION");

	// Action buttons - improved layout for 768px window width
	sf::Vector2f buttonSize(110, 45);  // Slightly smaller but more consistent sizing
	sf::Color buttonColor(40, 44, 52);  // More modern dark color

	// Calculate proper spacing for 6 buttons across 768px width
	float buttonGap = 15;
	float startX = (768 - (6 * buttonSize.x + 5 * buttonGap)) / 2.0f;  // Center horizontally with correct window width
	float buttonY = 470;  // Position above action prompt box

	/// Main action buttons
	gatherButton.setSize(buttonSize);
	gatherButton.setFillColor(buttonColor);
	gatherButton.setOutlineThickness(2);
	gatherButton.setOutlineColor(sf::Color(255, 255, 255)); // White border
	gatherButton.setPosition(startX, buttonY);

	taxButton.setSize(buttonSize);
	taxButton.setFillColor(buttonColor);
	taxButton.setOutlineThickness(2);
	taxButton.setOutlineColor(sf::Color(255, 255, 255)); // White border
	taxButton.setPosition(startX + buttonSize.x + buttonGap, buttonY);

	bribeButton.setSize(buttonSize);
	bribeButton.setFillColor(buttonColor);
	bribeButton.setOutlineThickness(2);
	bribeButton.setOutlineColor(sf::Color(255, 255, 255)); // White border
	bribeButton.setPosition(startX + 2 * (buttonSize.x + buttonGap), buttonY);

	arrestButton.setSize(buttonSize);
	arrestButton.setFillColor(buttonColor);
	arrestButton.setOutlineThickness(2);
	arrestButton.setOutlineColor(sf::Color(255, 255, 255)); // White border
	arrestButton.setPosition(startX + 3 * (buttonSize.x + buttonGap), buttonY);

	sanctionButton.setSize(buttonSize);
	sanctionButton.setFillColor(buttonColor);
	sanctionButton.setOutlineThickness(2);
	sanctionButton.setOutlineColor(sf::Color(255, 255, 255)); // White border
	sanctionButton.setPosition(startX + 4 * (buttonSize.x + buttonGap), buttonY);

	coupButton.setSize(buttonSize);
	coupButton.setFillColor(buttonColor);
	coupButton.setOutlineThickness(2);
	coupButton.setOutlineColor(sf::Color(255, 255, 255)); // White border
	coupButton.setPosition(startX + 5 * (buttonSize.x + buttonGap), buttonY);

	// Role-specific buttons - positioned in a row below main buttons
	sf::Vector2f roleButtonSize(120, 40);
	sf::Color roleButtonColor(60, 90, 120);  // More vibrant blue for role buttons

	float roleStartX = (768 - (3 * roleButtonSize.x + 2 * buttonGap)) / 2.0f;  // Center for max 3 buttons
	float roleButtonY = 525;  // Below main buttons, above action prompt

	// Baron button
	baronInvestButton.setSize(roleButtonSize);
	baronInvestButton.setFillColor(roleButtonColor);
	baronInvestButton.setOutlineThickness(2);
	baronInvestButton.setOutlineColor(sf::Color(255, 255, 255)); // White border
	baronInvestButton.setPosition(roleStartX, roleButtonY);

	// Spy buttons
	spyViewCoinsButton.setSize(roleButtonSize);
	spyViewCoinsButton.setFillColor(roleButtonColor);
	spyViewCoinsButton.setOutlineThickness(2);
	spyViewCoinsButton.setOutlineColor(sf::Color(255, 255, 255)); // White border
	spyViewCoinsButton.setPosition(roleStartX, roleButtonY);

	spyBlockArrestButton.setSize(roleButtonSize);
	spyBlockArrestButton.setFillColor(roleButtonColor);
	spyBlockArrestButton.setOutlineThickness(2);
	spyBlockArrestButton.setOutlineColor(sf::Color(255, 255, 255)); // White border
	spyBlockArrestButton.setPosition(roleStartX + roleButtonSize.x + buttonGap, roleButtonY);

	// Governor button
	governorBlockTaxButton.setSize(roleButtonSize);
	governorBlockTaxButton.setFillColor(roleButtonColor);
	governorBlockTaxButton.setOutlineThickness(2);
	governorBlockTaxButton.setOutlineColor(sf::Color(255, 255, 255)); // White border
	governorBlockTaxButton.setPosition(roleStartX, roleButtonY);

	// Main action buttons text setup
	gatherButtonText.setFont(font);
	gatherButtonText.setString("GATHER");
	gatherButtonText.setCharacterSize(14);
	gatherButtonText.setFillColor(CREAM_TEXT);
	centerTextInButton(gatherButtonText, gatherButton);

	taxButtonText.setFont(font);
	taxButtonText.setString("TAX");
	taxButtonText.setCharacterSize(14);
	taxButtonText.setFillColor(CREAM_TEXT);
	centerTextInButton(taxButtonText, taxButton);

	bribeButtonText.setFont(font);
	bribeButtonText.setString("BRIBE");
	bribeButtonText.setCharacterSize(14);
	bribeButtonText.setFillColor(CREAM_TEXT);
	centerTextInButton(bribeButtonText, bribeButton);

	arrestButtonText.setFont(font);
	arrestButtonText.setString("ARREST");
	arrestButtonText.setCharacterSize(14);
	arrestButtonText.setFillColor(CREAM_TEXT);
	centerTextInButton(arrestButtonText, arrestButton);

	sanctionButtonText.setFont(font);
	sanctionButtonText.setString("SANCTION");
	sanctionButtonText.setCharacterSize(14);
	sanctionButtonText.setFillColor(CREAM_TEXT);
	centerTextInButton(sanctionButtonText, sanctionButton);

	coupButtonText.setFont(font);
	coupButtonText.setString("COUP");
	coupButtonText.setCharacterSize(14);
	coupButtonText.setFillColor(CREAM_TEXT);
	centerTextInButton(coupButtonText, coupButton);

	// Role-specific button text setup
	baronInvestButtonText.setFont(font);
	baronInvestButtonText.setString("INVEST");
	baronInvestButtonText.setCharacterSize(14);
	baronInvestButtonText.setFillColor(CREAM_TEXT);
	fitTextInButton(baronInvestButtonText, baronInvestButton);

	spyViewCoinsButtonText.setFont(font);
	spyViewCoinsButtonText.setString("VIEW COINS");
	spyViewCoinsButtonText.setCharacterSize(14);
	spyViewCoinsButtonText.setFillColor(CREAM_TEXT);
	fitTextInButton(spyViewCoinsButtonText, spyViewCoinsButton);

	spyBlockArrestButtonText.setFont(font);
	spyBlockArrestButtonText.setString("BLOCK ARREST");
	spyBlockArrestButtonText.setCharacterSize(14);
	spyBlockArrestButtonText.setFillColor(CREAM_TEXT);
	fitTextInButton(spyBlockArrestButtonText, spyBlockArrestButton);

	governorBlockTaxButtonText.setFont(font);
	governorBlockTaxButtonText.setString("BLOCK TAX");
	governorBlockTaxButtonText.setCharacterSize(14);
	governorBlockTaxButtonText.setFillColor(CREAM_TEXT);
	fitTextInButton(governorBlockTaxButtonText, governorBlockTaxButton);


	// Intervention UI setup
	interventionBox.setSize(sf::Vector2f(600, 200));
	interventionBox.setFillColor(sf::Color(40, 40, 40, 230));  // Semi-transparent dark gray
	interventionBox.setPosition(100, 100);  // Upper middle of screen (moved from y=250 to y=100)

	interventionPromptText.setFont(font);
	interventionPromptText.setCharacterSize(16);
	interventionPromptText.setFillColor(CREAM_TEXT);
	interventionPromptText.setPosition(120, 130);  // Adjusted to match new box position

	// Yes/No buttons for intervention
	yesButton.setSize(sf::Vector2f(100, 40));
	yesButton.setFillColor(sf::Color(0, 150, 0));  // Green
	yesButton.setPosition(200, 230);  // Moved up to match new box position

	noButton.setSize(sf::Vector2f(100, 40));
	noButton.setFillColor(sf::Color(150, 0, 0));   // Red
	noButton.setPosition(400, 230);  // Moved up to match new box position

	yesButtonText.setFont(font);
	yesButtonText.setString("YES");
	yesButtonText.setCharacterSize(16);
	yesButtonText.setFillColor(CREAM_TEXT);
	centerTextInButton(yesButtonText, yesButton);

	noButtonText.setFont(font);
	noButtonText.setString("NO");
	noButtonText.setCharacterSize(16);
	noButtonText.setFillColor(CREAM_TEXT);
	centerTextInButton(noButtonText, noButton);

	// --- GameOver screen setup ---
	// Game Over screen components
    // Semi-transparent background overlay
    gameOverBackground.setSize(sf::Vector2f(window.getSize()));
    gameOverBackground.setFillColor(sf::Color(0, 0, 0, 200)); // Semi-transparent black
    
    // Game Over text
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color(255, 215, 0)); // Gold color
    gameOverText.setStyle(sf::Text::Bold);
    
    // Center the game over text
    sf::FloatRect tbGameOver = gameOverText.getLocalBounds();
    gameOverText.setOrigin(tbGameOver.left + tbGameOver.width/2.f, tbGameOver.top + tbGameOver.height/2.f);
    gameOverText.setPosition(window.getSize().x/2.f, 150);
    
    // Winner text
    winnerText.setFont(font);
    winnerText.setCharacterSize(32);
    winnerText.setFillColor(CREAM_TEXT);
    winnerText.setPosition(window.getSize().x/2.f, 250); // Positioning will be done when we know the winner
    
    // Trophy icon
    trophyTexture.loadFromFile("assets/trophy.png"); // Make sure to have this image in your assets folder
    trophySprite.setTexture(trophyTexture);
    trophySprite.setScale(0.5f, 0.5f); // Adjust scale as needed
    // Center the trophy
    sf::FloatRect spriteBounds = trophySprite.getLocalBounds();
    trophySprite.setOrigin(spriteBounds.width/2, spriteBounds.height/2);
    trophySprite.setPosition(window.getSize().x/2.f, 350);
    
    // Play Again button
    playAgainButton.setSize(sf::Vector2f(200, 60));
    playAgainButton.setFillColor(sf::Color(59, 89, 152)); // Facebook blue color
    playAgainButton.setOutlineThickness(2);
    playAgainButton.setOutlineColor(sf::Color(255, 255, 255));
    playAgainButton.setPosition(window.getSize().x/2.f - 220, 450);
    
    playAgainButtonText.setFont(font);
    playAgainButtonText.setString("PLAY AGAIN");
    playAgainButtonText.setCharacterSize(24);
    playAgainButtonText.setFillColor(CREAM_TEXT);
    centerTextInButton(playAgainButtonText, playAgainButton);
    
    // Exit button
    exitGameButton.setSize(sf::Vector2f(200, 60));
    exitGameButton.setFillColor(sf::Color(220, 53, 69)); // Bootstrap danger red
    exitGameButton.setOutlineThickness(2);
    exitGameButton.setOutlineColor(sf::Color(255, 255, 255));
    exitGameButton.setPosition(window.getSize().x/2.f + 20, 450);
    
    exitGameButtonText.setFont(font);
    exitGameButtonText.setString("EXIT");
    exitGameButtonText.setCharacterSize(24);
    exitGameButtonText.setFillColor(CREAM_TEXT);
    centerTextInButton(exitGameButtonText, exitGameButton);
}



void Window::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

// Helper function to center text in buttons
void Window::centerTextInButton(sf::Text& text, const sf::RectangleShape& button) {
    sf::FloatRect tb = text.getLocalBounds();
    text.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
    sf::Vector2f bp = button.getPosition();
    sf::Vector2f bs = button.getSize();
    text.setPosition(bp.x + bs.x/2.f, bp.y + bs.y/2.f);
}

void Window::handleEvents() {
    sf::Event ev;
    while (window.pollEvent(ev)) {

        if (ev.type == sf::Event::Closed) {
            window.close();
        }
        
        if(screen==Screen::Menu && ev.type==sf::Event::MouseButtonPressed){
            auto mp=sf::Vector2f(ev.mouseButton.x,ev.mouseButton.y);
            if(isButtonClicked(startButton,mp)){
                screen=Screen::AskCount;
                countInput.clear();
                errorText.setString("");
            }
            else if(isButtonClicked(exitButton,mp)){
                window.close();
            }
        }
        else if(screen==Screen::AskCount){
			if(ev.type==sf::Event::TextEntered) {
				char c = static_cast<char>(ev.text.unicode);
				if(std::isdigit(c) && countInput.size()<2) {
					countInput.push_back(c);
				}
				else if(c=='\b' && !countInput.empty()) {
					countInput.pop_back();
				}
				inputText.setString(countInput);
				
				// Re-center the input text after changing its content
				sf::FloatRect inputBounds = inputText.getLocalBounds();
				inputText.setOrigin(inputBounds.width/2.f, 0);
				inputText.setPosition(window.getSize().x/2.f, 160);
			}
            else if(ev.type==sf::Event::KeyPressed && ev.key.code==sf::Keyboard::Enter){
				if(!countInput.empty()){
					int n = std::stoi(countInput);
					if(n >= MIN_PLAYERS && n <= MAX_PLAYERS){
						// Init AskNames
						this->num_of_players = n;
						index_nop  = 0;
						nameInput.clear();
						namePrompt.setString("Player 1 of " + std::to_string(n) + " name:");

						// Re-center the name prompt after changing its content
						sf::FloatRect namePromptBounds = namePrompt.getLocalBounds();
						namePrompt.setOrigin(namePromptBounds.left + namePromptBounds.width/2.f, 0);
						namePrompt.setPosition(window.getSize().x/2.f, 120);

						nameInputText.setString("");
						screen = Screen::AskNames;
					} else {
						errorText.setString("Enter a number between 2 and 6");
						// Re-center the error text after changing its content
						sf::FloatRect errorBounds = errorText.getLocalBounds();
						errorText.setOrigin(errorBounds.left + errorBounds.width/2.f, errorBounds.top + errorBounds.height/2.f);
					}
				}
			}
		}
        
        // --- AskNames input & Enter ---
        else if (screen == Screen::AskNames) {
			// If we've finished names, only allow the NEXT‐button click:
			if (namesComplete) {
			  if (ev.type==sf::Event::MouseButtonPressed) {
				auto mp = sf::Vector2f(ev.mouseButton.x, ev.mouseButton.y);
				
				if (isButtonClicked(restartButton, mp)) {
					resetGame();
					return; // Exit handler after reset
				}

				if (isButtonClicked(nextButton,mp)) {
				  current_game->start_game();
				  screen = Screen::Play;
				}
			  }
			  continue; // _EVERY_ AskNames event is eaten here
			}
		  
			// 1) TEXT ENTRY for name
			if (ev.type==sf::Event::TextEntered) {
				char c = static_cast<char>(ev.text.unicode);
				if (std::isprint(c) && nameInput.size()<16) {
				  nameInput.push_back(c);
				} else if (c=='\b' && !nameInput.empty()) {
				  nameInput.pop_back();
				}
				nameInputText.setString(nameInput);
				
			  }
		  
			// 2) ENTER to register one name
			if (ev.type==sf::Event::KeyPressed && ev.key.code==sf::Keyboard::Enter) {
				if (!nameInput.empty()) {
					static std::random_device rd;
					static std::mt19937 gen(rd());
					// DEBUG
					static std::uniform_int_distribution<int> dis(0, 5);  // 0 to 5 inclusive
					Role role = static_cast<Role>(dis(gen));
					std::cout<< to_string(role);
				  try {
					PF.create_player(*current_game, nameInput, role);
					std::cout << "Player created successfully!" << std::endl;
				  } catch (const std::exception& e) {
					std::cout << "Exception caught: " << e.what() << std::endl;
					continue;
				  }
				  
				  // Check if we've registered all players
				  if (index_nop < num_of_players - 1) {
					// Still have more players to register
					++index_nop;
					nameInput.clear();
					namePrompt.setString(
						"Player " + std::to_string(index_nop+1) +
						" of " + std::to_string(num_of_players) +
						" name:"
					);
					
					// Re-center the name prompt after changing its content
					sf::FloatRect namePromptBounds = namePrompt.getLocalBounds();
					namePrompt.setOrigin(namePromptBounds.left + namePromptBounds.width/2.f, 0);
					namePrompt.setPosition(window.getSize().x/2.f, 120);
					
					nameInputText.setString("");
				} else {
					// We just registered the last player
					namesComplete = true;
					nameInput.clear();  // Clear the input
					nameInputText.setString("");  // Clear the display
				  }
				}
				continue;
			  }
		  
			// all other events in AskNames are consumed
			continue;
        }
		else if (screen == Screen::Play){
			   // PREVENT ALL OTHER INTERACTIONS DURING INTERVENTION
if (interventionState != InterventionState::NONE) {
    // Only handle intervention buttons during intervention
    if (ev.type == sf::Event::MouseButtonPressed) {
        auto mp = sf::Vector2f(ev.mouseButton.x, ev.mouseButton.y);
        
        // Track whether a valid button was clicked
        bool validButtonClicked = false;
        
        if (isButtonClicked(yesButton, mp)) {
            validButtonClicked = true;
            Player* intervenor = pendingInterventors[currentInterventorIndex];
            try {
                if (interventionState == InterventionState::WAITING_GENERAL) {
                    if (dynamic_cast<General*>(intervenor)->undo(*pendingAttacker, *pendingTarget, true)) {
                        current_game->set_general_intervention(true); // IMPORTANT: Set flag
                        setErrorMessage(errorMessageText, intervenor->getName() + " (General) blocked the coup!");
                        interventionState = InterventionState::NONE;
                        current_game->next_turn(); // Coup attempt (blocked) consumes the turn
                        return;  // Action blocked and handled.
                    }
                } else if (interventionState == InterventionState::WAITING_JUDGE) {
                    if (dynamic_cast<Judge*>(intervenor)->undo(*pendingAttacker, true)) {
                        current_game->set_judge_intervention(true); // IMPORTANT: Set flag
                        setErrorMessage(errorMessageText, intervenor->getName() + " (Judge) blocked the bribe!");
                        interventionState = InterventionState::NONE;
                        current_game->next_turn(); // REPLACE bribe() call with just advancing turn
                        return;  // Action blocked and handled.
                    }
                }
            } catch (const std::exception& e) {
                setErrorMessage(errorMessageText, "Intervention failed: " + std::string(e.what()));
                // If intervention itself fails, ensure flags are not incorrectly true
                if (interventionState == InterventionState::WAITING_GENERAL) current_game->set_general_intervention(false);
                if (interventionState == InterventionState::WAITING_JUDGE) current_game->set_judge_intervention(false);
            }
        }
        else if (isButtonClicked(noButton, mp)) {
            validButtonClicked = true;
            // Current intervenor doesn't want to block - pass false for shouldBlock
            Player* intervenor = pendingInterventors[currentInterventorIndex];
            
            if (interventionState == InterventionState::WAITING_GENERAL) {
                dynamic_cast<General*>(intervenor)->undo(*pendingAttacker, *pendingTarget, false);
            } else if (interventionState == InterventionState::WAITING_JUDGE) {
                dynamic_cast<Judge*>(intervenor)->undo(*pendingAttacker, false);
            }
        }
        
        // Only proceed to next intervenor if a valid button was clicked
        if (validButtonClicked) {
            // Move to next intervenor or execute action
            currentInterventorIndex++;
            if (currentInterventorIndex >= pendingInterventors.size()) {
                // No more intervenors, or all said "No".
                // The intervention flag (e.g., general_intervention) should be false here.
                try {
                    if (pendingActionType == "coup") {
                        // general_intervention flag is false, Player::coup will proceed with elimination.
                        pendingAttacker->coup(*pendingTarget);
                        // Message might be redundant if Player::coup handles its own state or turn changes view
                        setErrorMessage(errorMessageText, "COUPED " + pendingTarget->getName());
                    } else if (pendingActionType == "bribe") {
                        // judge_intervention flag is false.
                        pendingAttacker->getGame().set_judge_intervention(false);
                        pendingAttacker->bribe(); // Player::bribe should also check its flag
                        setErrorMessage(errorMessageText, "BRIBE SUCCESSFUL - EXTRA TURN");
                    }
                } catch (const std::exception& e) {
                    setErrorMessage(errorMessageText, "ERROR: " + std::string(e.what()));
                }
                interventionState = InterventionState::NONE;
            } else {
                // Ask next intervenor
                Player* nextIntervenor = pendingInterventors[currentInterventorIndex];
                if (interventionState == InterventionState::WAITING_GENERAL) {
                    interventionPromptText.setString(
                        nextIntervenor->getName() + " (General), do you want to block " +
                        pendingAttacker->getName() + "'s coup on " + pendingTarget->getName() + "?"
                    );
                } else {
                    interventionPromptText.setString(
                        nextIntervenor->getName() + " (Judge), do you want to block " +
                        pendingAttacker->getName() + "'s bribe?"
                    );
                }
            }
        }
    }
    // RETURN HERE - don't process any other events during intervention
    return;
}

			// NORMAL GAME PROCESSING - only when not in intervention
			if (ev.type == sf::Event::MouseButtonPressed) {
				auto mp = sf::Vector2f(ev.mouseButton.x, ev.mouseButton.y);
				if (actionState == ActionState::SELECTING_TARGET) {
        if (isButtonClicked(gatherButton, mp) || 
            isButtonClicked(taxButton, mp) || 
            isButtonClicked(bribeButton, mp) || 
            isButtonClicked(arrestButton, mp) || 
            isButtonClicked(sanctionButton, mp) || 
            isButtonClicked(coupButton, mp) || 
            isButtonClicked(baronInvestButton, mp) || 
            isButtonClicked(spyViewCoinsButton, mp) || 
            isButtonClicked(spyBlockArrestButton, mp) || 
            isButtonClicked(governorBlockTaxButton, mp)) {
            
            // Cancel the current target selection
            actionState = ActionState::CHOOSING_ACTION;
            setActionPrompt("CHOOSE ACTION");
            pendingAction = "";
            setErrorMessage(errorMessageText, "Previous action canceled");
        }
    }
				// Check for default action buttons
				if (isButtonClicked(gatherButton, mp)) {
					std::cout << "GATHER action clicked!" << std::endl;
					setErrorMessage(errorMessageText, "");  // Clear previous error
					try {
						Player* currentPlayer = current_game->get_current_player();
						if (currentPlayer) {
							currentPlayer->gather();
							std::cout << "Gather action performed successfully!" << std::endl;
						}
					} catch (const std::exception& e) {
						std::cout << "Gather failed: " << e.what() << std::endl;
						setErrorMessage(errorMessageText, "ERROR: " + std::string(e.what()));
					}
				}
				else if (isButtonClicked(taxButton, mp)) {
					std::cout << "TAX action clicked!" << std::endl;
					setErrorMessage(errorMessageText, "");  // Clear previous error
					try {
						Player* currentPlayer = current_game->get_current_player();
						if (currentPlayer) {
							currentPlayer->tax();
							std::cout << "Tax action performed successfully!" << std::endl;
						}
					} catch (const std::exception& e) {
						std::cout << "Tax failed: " << e.what() << std::endl;
						setErrorMessage(errorMessageText, "ERROR: " + std::string(e.what()));
					}
				}
				else if (isButtonClicked(bribeButton, mp)) {
					// reset flag
					this->current_game->set_judge_intervention(false); // Initially assume no intervention

					std::cout << "BRIBE action clicked!" << std::endl;
					setErrorMessage(errorMessageText, "");  // Clear previous error
					Player* attacker = current_game->get_current_player();
					if (attacker) {
						// Execute bribe immediately to deduct coins
						try {

							this->current_game->set_judge_intervention(false);
							// Now check for Judge intervention
							auto judges = current_game->get_judges();
							std::vector<Player*> eligibleJudges;
							for (auto* j : judges) {
								if (j != attacker) eligibleJudges.push_back(j);
							}

							// Set flag to true only if judges exist
							if (!eligibleJudges.empty()) {
								this->current_game->set_judge_intervention(true);
							}

							// This will deduct the 4 coins cost
							attacker->bribe();
							std::cout << "Bribe cost paid" << std::endl;

							
				
							if (!eligibleJudges.empty()) {
								// start Judge intervention
								interventionState      = InterventionState::WAITING_JUDGE;
								pendingInterventors    = std::move(eligibleJudges);
								currentInterventorIndex = 0;
								pendingAttacker        = attacker;
								pendingActionType      = "bribe";
								interventionPromptText.setString(
									pendingInterventors[0]->getName() +
									" (Judge), block " + attacker->getName() + "'s bribe?"
								);
								return;  // bail out now so we stay in intervention mode
							} else {
								// no Judges → bribe already executed, just show success
								setErrorMessage(errorMessageText, "BRIBE SUCCESSFUL - EXTRA TURN");
							}
						} catch (const std::exception& e) {
							std::cout << "Bribe failed: " << e.what() << std::endl;
							setErrorMessage(errorMessageText, "ERROR: " + std::string(e.what()));
						}
					}
				}
				else if (isButtonClicked(arrestButton, mp)) {
					std::cout << "ARREST action clicked!" << std::endl;
					setErrorMessage(errorMessageText, "");  // Clear previous error
					
					// Switch to target selection mode (like sanction and coup)
					actionState = ActionState::SELECTING_TARGET;
					pendingAction = "arrest";
					setActionPrompt("CLICK ON A PLAYER TO ARREST");
				}
				else if (isButtonClicked(sanctionButton, mp)) {
					std::cout << "SANCTION action clicked!" << std::endl;
					setErrorMessage(errorMessageText, "");  // Clear previous error
					
					// Switch to target selection mode
					actionState = ActionState::SELECTING_TARGET;
					pendingAction = "sanction";
					setActionPrompt("CLICK ON A PLAYER TO SANCTION");
				}
				else if (isButtonClicked(coupButton, mp)) {
					std::cout << "COUP action clicked!" << std::endl;
					setErrorMessage(errorMessageText, "");  // Clear previous error
					
					    
					// Reset the general intervention flag at the start of each new coup attempt
					current_game->set_general_intervention(false);

					// Switch to target selection mode
					actionState = ActionState::SELECTING_TARGET;
					pendingAction = "coup";
					setActionPrompt("CLICK ON A PLAYER TO COUP");
				}
				
				// Role-specific buttons
				if (current_game->get_current_player()) {
					Role currentRole = current_game->get_current_player()->getRole();
					
					if (currentRole == Role::BARON) {
						if (isButtonClicked(baronInvestButton, mp)) {
							std::cout << "Baron INVEST action clicked!" << std::endl;
							try {
								Player* currentPlayer = current_game->get_current_player();
								if (currentPlayer) {
									dynamic_cast<Baron*>(currentPlayer)->invest();
									std::cout << "Invest action performed successfully!" << std::endl;
									
								}
							} catch (const std::exception& e) {
								std::cout << "Invest failed: " << e.what() << std::endl;
								setErrorMessage(errorMessageText, "ERROR: " + std::string(e.what()));
							}
						}
					}
				    // Spy special actions  
					else if (currentRole == Role::SPY) {
						if (isButtonClicked(spyViewCoinsButton, mp)) {
							std::cout << "Spy VIEW COINS action clicked!" << std::endl;
							setErrorMessage(errorMessageText, "");  // Clear previous error
							
							// Switch to target selection mode for view coins
							actionState = ActionState::SELECTING_TARGET;
							pendingAction = "view_coins";
							setActionPrompt("CLICK ON A PLAYER TO VIEW COINS");
						}
						else if (isButtonClicked(spyBlockArrestButton, mp)) {
							std::cout << "Spy BLOCK ARREST action clicked!" << std::endl;
							setErrorMessage(errorMessageText, "");  // Clear previous error
							
							// Switch to target selection mode for block arrest
							actionState = ActionState::SELECTING_TARGET;
							pendingAction = "block_arrest";
							setActionPrompt("CLICK ON A PLAYER TO BLOCK ARREST");
						}
					}
					// Governor special actions
					else if (currentRole == Role::GOVERNOR) {
						 if (isButtonClicked(governorBlockTaxButton, mp)) {
							std::cout << "Governor BLOCK TAX action clicked!" << std::endl;
							setErrorMessage(errorMessageText, "");  // Clear previous error
							
							// Switch to target selection mode for block tax
							actionState = ActionState::SELECTING_TARGET;
							pendingAction = "block_tax";
							setActionPrompt("CLICK ON A PLAYER TO BLOCK TAX");
						}
					}
				}

			// Handle player selection when in target selection mode
			if (actionState == ActionState::SELECTING_TARGET) {
				// Check if user clicked on any player in the list
				auto players = current_game->get_players_objects();
				Player* currentPlayer = current_game->get_current_player();
				
				for (size_t i = 0; i < players.size(); ++i) {
					// Calculate the clickable area for each player name
					sf::FloatRect playerArea;
					playerArea.left = 50;
					playerArea.top = 150 + i * 35;
					playerArea.width = 400;  // Wide enough to cover the player text
					playerArea.height = 30;  // Height of one line of text
					
					// Check if mouse click is within this player's area
					if (playerArea.contains(mp)) {
						current_game->set_target_player(*players[i]);
						
						// // Can't target yourself (for most actions)
						// if (targetPlayer == currentPlayer && (pendingAction == "sanction" || pendingAction == "coup")) {
						// 	setErrorMessage(errorMessageText, "ERROR: Cannot target yourself!");
						// 	actionState = ActionState::CHOOSING_ACTION;
						// 	actionPromptText.setString("CHOOSE ACTION");
						// 	pendingAction = "";
						// 	break;
						// }
						
						// Execute the pending action on the selected target
						try {
							if (pendingAction == "sanction") {
								currentPlayer->sanction(current_game->get_target_player());  // Pass target player
								std::cout << "Sanction performed on " << current_game->get_target_player().getName() << std::endl;
								setErrorMessage(errorMessageText, "SANCTIONED " + current_game->get_target_player().getName());
							}
							
							else if (pendingAction == "coup") {
                                if (!currentPlayer) { /* Should not happen if logic is correct */ return; }

                                // Attempt to pay the coup cost immediately
                                try {
                                    currentPlayer->pay_coup_cost(current_game->get_target_player()); // DEDUCTS COINS NOW
                                } catch (const std::exception& e_cost) {
                                    std::cout << "Coup attempt failed (cost): " << e_cost.what() << std::endl;
                                    setErrorMessage(errorMessageText, "ERROR: " + std::string(e_cost.what()));
                                    actionState = ActionState::CHOOSING_ACTION; // Reset state
                                    setActionPrompt("CHOOSE ACTION");
                                    pendingAction = "";
                                    break; // Exit player loop, action failed
                                }

                                auto generals = current_game->get_generals();
                                std::vector<Player*> eligibleGenerals;
                                
                                for (Player* general : generals) {
                                    if (general != currentPlayer && general->coins() >= 5) {
                                        eligibleGenerals.push_back(general);
                                    }
                                }
                                
                                if (!eligibleGenerals.empty()) {
                                    // Generals can intervene. Start UI intervention process.
                                    // Player::coup() is NOT called here.
                                    interventionState = InterventionState::WAITING_GENERAL;
                                    pendingInterventors = eligibleGenerals;
                                    currentInterventorIndex = 0;
                                    pendingAttacker = currentPlayer;
                                    pendingTarget = &current_game->get_target_player(); // Ensure target is correctly set
                                    pendingActionType = "coup";
                                    
                                    interventionPromptText.setString(
                                        pendingInterventors[0]->getName() + " (General), do you want to block " +
                                        pendingAttacker->getName() + "'s coup on " + pendingTarget->getName() + "?"
                                    );
                                    
                                    actionState = ActionState::CHOOSING_ACTION; // Or a specific intervention UI state
                                    setActionPrompt("CHOOSE ACTION"); // Or "Intervention..."
                                    pendingAction = ""; // Handled by intervention
                                    return;  // Exit to let intervention UI run
                                }
                                else {
                                    // No eligible generals, execute coup normally.
                                    // general_intervention flag is false.
                                    try {
                                        currentPlayer->coup(current_game->get_target_player());
                                        std::cout << "Coup performed on " << current_game->get_target_player().getName() << std::endl;
                                        setErrorMessage(errorMessageText, "COUPED " + current_game->get_target_player().getName());
                                    } catch (const std::exception& e) {
                                        std::cout << "Coup failed: " << e.what() << std::endl;
                                        setErrorMessage(errorMessageText, "ERROR: " + std::string(e.what()));
                                    }
                                    // Reset state after action
                                    actionState = ActionState::CHOOSING_ACTION;
                                    setActionPrompt("CHOOSE ACTION");
                                    pendingAction = "";
                                }
                            }
							else if (pendingAction == "arrest") {  
								currentPlayer->arrest(current_game->get_target_player());  // Pass target player
								std::cout << "Arrest performed on " << current_game->get_target_player().getName() << std::endl;
								setErrorMessage(errorMessageText, "ARRESTED " + current_game->get_target_player().getName());
							}
							else if (pendingAction == "view_coins") { 
								dynamic_cast<Spy*>(currentPlayer)->view_coins(current_game->get_target_player());
								std::cout << "View coins performed on " << current_game->get_target_player().getName() << std::endl;
								setErrorMessage(errorMessageText, "VIEWED COINS OF '" + current_game->get_target_player().getName() +"'\
								\nNUMBER OF COINS: " + std::to_string(current_game->get_target_player().coins()));

							}
							else if (pendingAction == "block_arrest") { 
								dynamic_cast<Spy*>(currentPlayer)->disable_arrest(current_game->get_target_player());
								std::cout << "Block arrest performed on " << current_game->get_target_player().getName() << std::endl;
								setErrorMessage(errorMessageText, "BLOCKED ARREST FOR " + current_game->get_target_player().getName());

							}
							else if (pendingAction == "block_tax") {  
								dynamic_cast<Governor*>(currentPlayer)->block_tax(current_game->get_target_player());
								std::cout << "Block tax performed on " << current_game->get_target_player().getName() << std::endl;
								setErrorMessage(errorMessageText, "BLOCKED TAX FOR " + current_game->get_target_player().getName());

							}
							
							// Reset to normal action selection
							actionState = ActionState::CHOOSING_ACTION;
							setActionPrompt("CHOOSE ACTION");
							pendingAction = "";
							
						} catch (const std::exception& e) {
							std::cout << pendingAction << " failed: " << e.what() << std::endl;
							setErrorMessage(errorMessageText, "ERROR: " + std::string(e.what()));
							
							// Reset to normal action selection
							actionState = ActionState::CHOOSING_ACTION;
							setActionPrompt("CHOOSE ACTION");
							pendingAction = "";
						}
						break;  // Exit the loop since we found the clicked player
					}
				}
			}
		}

		// Handle ESC key to cancel target selection
		if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
			if (actionState == ActionState::SELECTING_TARGET) {
				// Cancel target selection
				actionState = ActionState::CHOOSING_ACTION;
				setActionPrompt("CHOOSE ACTION");
				pendingAction = "";
				setErrorMessage(errorMessageText, "Action cancelled");
			}
		}

    }
	else if (screen == Screen::GameOver) {
		if (ev.type == sf::Event::MouseButtonPressed) {
			auto mp = sf::Vector2f(ev.mouseButton.x, ev.mouseButton.y);
			
			if (isButtonClicked(playAgainButton, mp)) {
				// Call the reset method instead of just changing the screen
				resetGame();
			}
			else if (isButtonClicked(exitGameButton, mp)) {
				window.close();
			}
		}
	}
}
	
}



void Window::render() {
    window.clear(sf::Color::Black);
	if(screen==Screen::Menu){
		window.draw(titleText);
	
		// Draw buttons with hover effect
		if (startButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
			startButton.setFillColor(sf::Color(49, 79, 142)); // Darker blue when hovering
		} else {
			startButton.setFillColor(sf::Color(59, 89, 152)); // Normal blue
		}
		
		if (exitButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
			exitButton.setFillColor(sf::Color(200, 33, 49)); // Darker red when hovering
		} else {
			exitButton.setFillColor(sf::Color(220, 53, 69)); // Normal red
		}
		
		window.draw(startButton);
		window.draw(startButtonText);
		window.draw(exitButton);
		window.draw(exitButtonText);
		window.draw(nameText);
		window.draw(emailText);
	}
    else if(screen==Screen::AskCount){
		window.draw(titleText);

        window.draw(promptText);
        window.draw(inputText);
        window.draw(errorText);
    }
	else if (screen==Screen::AskNames) {
		window.draw(titleText);

		// Draw restart button with hover effect
		if (restartButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
			restartButton.setFillColor(sf::Color(49, 79, 142)); // Darker blue when hovering
		} else {
			restartButton.setFillColor(sf::Color(59, 89, 152)); // Normal blue
		}
		window.draw(restartButton);
		window.draw(restartButtonText);

        window.draw(namePrompt);
        window.draw(nameInputText);
        // draw each registered player under the input (moved higher)
		for (size_t i = 0; i < current_game->get_players_objects().size(); ++i) {
			sf::Text info;
			info.setFont(font);
			info.setCharacterSize(18);
			info.setFillColor(sf::Color::Green);
			auto* pl = current_game->get_players_objects()[i];
			std::string line = pl->getName() + " : " + coup::to_string(pl->getRole());
			info.setString(line);
			
			// Center each player entry
			sf::FloatRect infoBounds = info.getLocalBounds();
			info.setOrigin(infoBounds.left + infoBounds.width/2.f, 0);
			info.setPosition(window.getSize().x/2.f, 230 + i * 22);
			
			window.draw(info);
		}
        // draw NEXT button if all names are complete
        if(namesComplete) {
            // Draw shadow first (behind the button)
			window.draw(nextButtonShadow);
			// Then draw the button
			window.draw(nextButton);
			window.draw(nextButtonText);
        }
    }
	else if(screen==Screen::Play) {
		// Draw the title "COUP" at the top (reusing existing titleText)
		window.draw(titleText);
		
		// Get the list of all players from the game
		auto players = current_game->get_players_objects();
		// Get the player whose turn it is right now
		Player* currentPlayer = current_game->get_current_player();
		for (size_t i = 0; i < players.size(); ++i) {
			sf::Text playerInfo;
			playerInfo.setFont(font);
			playerInfo.setCharacterSize(20);
		
			// ALWAYS include coin count
			std::string playerText = players[i]->getName() + " " + to_string(players[i]->getRole()) + " (" + std::to_string(players[i]->coins()) + " coins)";
		
			// Highlight current player or show clickable state
			if (players[i] == currentPlayer) {
				playerInfo.setFillColor(sf::Color(255,215,0)); // gold
			} else if (actionState == ActionState::SELECTING_TARGET && interventionState == InterventionState::NONE) {
				playerInfo.setFillColor(sf::Color::Cyan);
			} else {
				playerInfo.setFillColor(CREAM_TEXT);
			}
		
			playerInfo.setString(playerText);
			playerInfo.setPosition(50, 150 + i * 35);
			window.draw(playerInfo);
		}
	
		// Draw the action prompt box and text
		window.draw(actionPromptBox);
		window.draw(actionPromptText);
		
		// Draw error message if there is one
		window.draw(errorMessageText);
	
		// Only draw action buttons if NOT in intervention state
		if (interventionState == InterventionState::NONE) {
			// Draw all the default action buttons at the bottom
			window.draw(gatherButton);         
			window.draw(gatherButtonText);     
			window.draw(taxButton);            
			window.draw(taxButtonText);        
			window.draw(bribeButton);          
			window.draw(bribeButtonText);      
			window.draw(arrestButton);         
			window.draw(arrestButtonText);     
			window.draw(sanctionButton);       
			window.draw(sanctionButtonText);   
			window.draw(coupButton);           
			window.draw(coupButtonText);       
	
			// Draw role-specific buttons only for current player's role
			if (currentPlayer) {
				Role currentRole = currentPlayer->getRole();
				
				// Only draw the buttons that match the current player's role
				switch(currentRole) {
					case Role::BARON:
						window.draw(baronInvestButton);
						window.draw(baronInvestButtonText);
						break;
					case Role::SPY:
						window.draw(spyViewCoinsButton);
						window.draw(spyViewCoinsButtonText);
						window.draw(spyBlockArrestButton);
						window.draw(spyBlockArrestButtonText);
						break;
					case Role::GOVERNOR:
						window.draw(governorBlockTaxButton);
						window.draw(governorBlockTaxButtonText);
						break;
					// Other roles don't have special buttons
					case Role::GENERAL:
					case Role::JUDGE:
					case Role::MERCHANT:
						break;
				}
			}
		}
	
		// Draw intervention UI with overlay if active
		if (interventionState != InterventionState::NONE) {
			// Draw a semi-transparent overlay over the entire game area
			sf::RectangleShape overlay;
			overlay.setSize(sf::Vector2f(window.getSize()));
			overlay.setFillColor(sf::Color(0, 0, 0, 180)); // Semi-transparent black
			window.draw(overlay);
			
			// Draw the intervention UI elements on top of the overlay
			window.draw(interventionBox);
			window.draw(interventionPromptText);
			window.draw(yesButton);
			window.draw(yesButtonText);
			window.draw(noButton);
			window.draw(noButtonText);
		}
	
		checkGameOver();
	}

	else if (screen == Screen::GameOver) {
		
		// Draw the semi-transparent overlay
		window.draw(gameOverBackground);
		
		
		// Draw the trophy icon
		window.draw(trophySprite);
		
		// Draw text elements
		window.draw(gameOverText);
		window.draw(winnerText);
		
		// Draw buttons with hover effect
		if (playAgainButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
			playAgainButton.setFillColor(sf::Color(49, 79, 142)); // Darker blue when hovering
		} else {
			playAgainButton.setFillColor(sf::Color(59, 89, 152)); // Normal blue
		}
		
		if (exitGameButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
			exitGameButton.setFillColor(sf::Color(200, 33, 49)); // Darker red when hovering
		} else {
			exitGameButton.setFillColor(sf::Color(220, 53, 69)); // Normal red
		}
		
		window.draw(playAgainButton);
		window.draw(playAgainButtonText);
		window.draw(exitGameButton);
		window.draw(exitGameButtonText);
	}

window.display();

}

bool Window::isButtonClicked(const sf::RectangleShape& button, const sf::Vector2f& mousePos) {
    return button.getGlobalBounds().contains(mousePos);
}

void Window::setActionPrompt(const std::string& message) {
    // Only change text if message is different
    if (actionPromptText.getString() != message) {
        // Set the text
        actionPromptText.setString(message);
        
        // Always reset font size when text content changes
        actionPromptText.setCharacterSize(18);
        
        // Get text bounds to check if it's too wide
        sf::FloatRect textBounds = actionPromptText.getLocalBounds();
        float maxWidth = actionPromptBox.getSize().x - 20; // 10px padding on each side
        
        // If text is too wide, reduce the font size
        if (textBounds.width > maxWidth) {
            int newSize = 18;
            while (textBounds.width > maxWidth && newSize > 10) {
                newSize -= 2;
                actionPromptText.setCharacterSize(newSize);
                textBounds = actionPromptText.getLocalBounds();
            }
        }
        
        // Re-center the text
        centerTextInButton(actionPromptText, actionPromptBox);
    }
    // If message is the same, do nothing to preserve current size
}

void Window::checkGameOver() {
    try {
        // First check if there is a winner
        std::string winnerName = current_game->winner(); // This will throw if no winner yet
        
        // If we got here, there is a winner - then set up the game over screen
        winnerText.setString("WINNER: " + winnerName);
        
        // Center the winner text
        sf::FloatRect tb = winnerText.getLocalBounds();
        winnerText.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
        winnerText.setPosition(window.getSize().x/2.f, 220);
        
        // Only change to GameOver screen if we have a winner
        screen = Screen::GameOver;
        std::cout << "Game Over! Winner: " + winnerName << std::endl;
    }
    catch (const std::exception& e) {
        // No winner yet, just continue the game
        // Don't print anything to avoid console spam
    }
}

void Window::resetGame() {
    // Reset game state
    current_game->reset();  // Create a fresh Game object
    
    // Reset player management variables
    num_of_players = 0;
    index_nop = 0;
    namesComplete = false;
    
    // Reset input fields
    countInput.clear();
    nameInput.clear();
    
    // Reset display text objects (this is what was missing)
    inputText.setString("");       // Clear the count input display
    nameInputText.setString("");   // Clear the name input display
    errorText.setString("");       // Clear any error messages
    
    // Reset game state variables
    actionState = ActionState::CHOOSING_ACTION;
    interventionState = InterventionState::NONE;
    pendingAction = "";
    pendingActionType = "";
    
    // Reset all intervention-related variables
    pendingInterventors.clear();
    currentInterventorIndex = 0;
    pendingAttacker = nullptr;
    pendingTarget = nullptr;
    
    // Reset text displays
    setErrorMessage(errorMessageText, "");
    setActionPrompt("CHOOSE ACTION");
    
    // Change the screen
    screen = Screen::Menu;
}

void Window::fitTextInButton(sf::Text& text, const sf::RectangleShape& button) {
    // First center the text
    centerTextInButton(text, button);
    
    // Check if text is too wide for the button
    sf::FloatRect textBounds = text.getLocalBounds();
    float maxWidth = button.getSize().x - 10; // 5px padding on each side
    
    // If text is too wide, reduce the font size
    if (textBounds.width > maxWidth) {
        int newSize = text.getCharacterSize();
        while (textBounds.width > maxWidth && newSize > 8) { // Don't go below 8pt
            newSize -= 1;
            text.setCharacterSize(newSize);
            textBounds = text.getLocalBounds();
        }
        // Re-center with the new size
        centerTextInButton(text, button);
    }
}

void Window::setErrorMessage(sf::Text& errorText, const std::string& message) {
    // Set the text
    errorText.setString(message);
    
    // Reset font size to default
    errorText.setCharacterSize(20);
    
    // Get text bounds to check if it's too wide
    sf::FloatRect textBounds = errorText.getLocalBounds();
    float maxWidth = window.getSize().x - 100; // 50px padding on each side
    
    // If text is too wide, reduce the font size
    if (textBounds.width > maxWidth) {
        int newSize = 20;
        while (textBounds.width > maxWidth && newSize > 10) {
            newSize -= 2;
            errorText.setCharacterSize(newSize);
            textBounds = errorText.getLocalBounds();
        }
        
        // If still too wide at smallest font size, truncate with ellipsis
        if (textBounds.width > maxWidth) {
            std::string truncated = message;
            while (textBounds.width > maxWidth && truncated.length() > 3) {
                truncated = truncated.substr(0, truncated.length() - 4) + "...";
                errorText.setString(truncated);
                textBounds = errorText.getLocalBounds();
            }
        }
    }
    
    // Re-center the text
    textBounds = errorText.getLocalBounds();
    errorText.setOrigin(textBounds.left + textBounds.width/2.f, textBounds.top + textBounds.height/2.f);
    errorText.setPosition(window.getSize().x/2.f, errorText.getPosition().y);
}