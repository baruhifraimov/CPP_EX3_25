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

using namespace coup;

Window::Window(Game& current_game) : window(sf::VideoMode(800, 600), "COUP Game"), 								current_game(current_game) {
	
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
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(350, 50);
	{
		sf::FloatRect tb = titleText.getLocalBounds();
		titleText.setOrigin(tb.left + tb.width/2.f,
							tb.top  + tb.height/2.f);
		// place it at half the window width, y=50
		titleText.setPosition(window.getSize().x/2.f, 50.f);
	  }
    
    // START button
    startButton.setSize(sf::Vector2f(200, 60));
    startButton.setFillColor(sf::Color::Green);
    startButton.setPosition(300, 200);
    
    startButtonText.setFont(font);
    startButtonText.setString("START");
    startButtonText.setCharacterSize(24);
    startButtonText.setFillColor(sf::Color::Black);
    startButtonText.setPosition(365, 220);
	// centering
	{
		// 1) get the text’s local bounding rect
		sf::FloatRect tb = startButtonText.getLocalBounds();
	  
		// 2) set its origin to the center of that rect
		startButtonText.setOrigin(tb.left + tb.width/2.f,
								  tb.top  + tb.height/2.f);
	  
		// 3) move it to the center of the button
		sf::Vector2f bp = startButton.getPosition();
		sf::Vector2f bs = startButton.getSize();
		startButtonText.setPosition(bp.x + bs.x/2.f,
									bp.y + bs.y/2.f);
	  }
    
    // EXIT button
    exitButton.setSize(sf::Vector2f(200, 60));
    exitButton.setFillColor(sf::Color::Red);
    exitButton.setPosition(300, 300);
    
    exitButtonText.setFont(font);
    exitButtonText.setString("EXIT");
    exitButtonText.setCharacterSize(24);
    exitButtonText.setFillColor(sf::Color::Black);
    exitButtonText.setPosition(375, 320);
	{
		sf::FloatRect tb = exitButtonText.getLocalBounds();
		exitButtonText.setOrigin(tb.left + tb.width/2.f,
								 tb.top  + tb.height/2.f);
		sf::Vector2f bp = exitButton.getPosition();
		sf::Vector2f bs = exitButton.getSize();
		exitButtonText.setPosition(bp.x + bs.x/2.f,
								   bp.y + bs.y/2.f);
	  }
    
    // Name and email at bottom
    nameText.setFont(font);
    nameText.setString("Baruh Ifraimov");
    nameText.setCharacterSize(20);
    nameText.setFillColor(sf::Color::White);
    nameText.setPosition(50, 520);
    
    emailText.setFont(font);
    emailText.setString("baruh.ifraimov@gmail.com");
    emailText.setCharacterSize(20);
    emailText.setFillColor(sf::Color::White);
    emailText.setPosition(50, 550);

	// — AskCount setup —
    promptText.setFont(font);
    promptText.setString("How many players? (2-6)");
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(200,120);
	

    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::Yellow);
    inputText.setPosition(200,160);

    errorText.setFont(font);
    errorText.setCharacterSize(20);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(200,160);

	// --- AskNames setup ---
    namePrompt.setFont(font);
    namePrompt.setCharacterSize(24);
    namePrompt.setFillColor(sf::Color::White);
    namePrompt.setPosition(200, 120);  // moved from 200 to 120

    nameInputText.setFont(font);
    nameInputText.setCharacterSize(24);
    nameInputText.setFillColor(sf::Color::Yellow);
    nameInputText.setPosition(200, 160);  // moved from 240 to 160
    
    // NEXT button (hidden until last name entered)
    nextButton.setSize({120, 50});
    nextButton.setFillColor(sf::Color(50,200,50));
    nextButton.setPosition(340, 400);
    // label
    nextButtonText.setFont(font);
    nextButtonText.setString("NEXT");
    nextButtonText.setCharacterSize(24);
    nextButtonText.setFillColor(sf::Color::Black);
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
    currentPlayerRoleText.setFillColor(sf::Color::White);
    currentPlayerRoleText.setPosition(550, 150);

    // Action prompt box (center)
    actionPromptBox.setSize({400, 80});
    actionPromptBox.setFillColor(sf::Color::Black);
    actionPromptBox.setOutlineThickness(3);
    actionPromptBox.setOutlineColor(sf::Color::Red);
	actionPromptBox.setPosition(200, 485);

    actionPromptText.setFont(font);
    actionPromptText.setCharacterSize(18);
    actionPromptText.setFillColor(sf::Color::White);
    actionPromptText.setPosition(220, 510);
    actionPromptText.setString("CHOOSE ACTION");



	// Action buttons - flat retro style, centered horizontally
	sf::Vector2f buttonSize(120, 50);  // Made smaller to fit 6 buttons
	sf::Color buttonColor(64, 64, 64);  // dark gray

	float startX = (800 - (6 * 120 + 5 * 15)) / 2.0f;  // Center horizontally
	float buttonY = 370;  // Position in lower half of screen
	gatherButton.setSize(buttonSize);
	gatherButton.setFillColor(buttonColor);
	gatherButton.setPosition(startX, buttonY);  // First button
	
	taxButton.setSize(buttonSize);
	taxButton.setFillColor(buttonColor);
	taxButton.setPosition(startX + 135, buttonY);  // Second button (120 + 15 gap)
	
	bribeButton.setSize(buttonSize);
	bribeButton.setFillColor(buttonColor);
	bribeButton.setPosition(startX + 270, buttonY);  // Third button
	
	arrestButton.setSize(buttonSize);
	arrestButton.setFillColor(buttonColor);
	arrestButton.setPosition(startX + 405, buttonY);  // Fourth button
	
	sanctionButton.setSize(buttonSize);
	sanctionButton.setFillColor(buttonColor);
	sanctionButton.setPosition(startX + 540, buttonY);  // Fifth button
	
	coupButton.setSize(buttonSize);
	coupButton.setFillColor(buttonColor);
	coupButton.setPosition(startX + 675, buttonY);  // Sixth button
	
	// Button texts
	gatherButtonText.setFont(font);
	gatherButtonText.setString("GATHER");
	gatherButtonText.setCharacterSize(14);
	gatherButtonText.setFillColor(sf::Color::White);
	centerTextInButton(gatherButtonText, gatherButton);
	
	taxButtonText.setFont(font);
	taxButtonText.setString("TAX");
	taxButtonText.setCharacterSize(14);
	taxButtonText.setFillColor(sf::Color::White);
	centerTextInButton(taxButtonText, taxButton);
	
	bribeButtonText.setFont(font);
	bribeButtonText.setString("BRIBE");
	bribeButtonText.setCharacterSize(14);
	bribeButtonText.setFillColor(sf::Color::White);
	centerTextInButton(bribeButtonText, bribeButton);
	
	arrestButtonText.setFont(font);
	arrestButtonText.setString("ARREST");
	arrestButtonText.setCharacterSize(14);
	arrestButtonText.setFillColor(sf::Color::White);
	centerTextInButton(arrestButtonText, arrestButton);
	
	sanctionButtonText.setFont(font);
	sanctionButtonText.setString("SANCTION");
	sanctionButtonText.setCharacterSize(12);
	sanctionButtonText.setFillColor(sf::Color::White);
	centerTextInButton(sanctionButtonText, sanctionButton);
	
	coupButtonText.setFont(font);
	coupButtonText.setString("COUP");
	coupButtonText.setCharacterSize(14);
	coupButtonText.setFillColor(sf::Color::White);
	centerTextInButton(coupButtonText, coupButton);

	// Error message display
	errorMessageText.setFont(font);
	errorMessageText.setCharacterSize(16);
	errorMessageText.setFillColor(sf::Color::Red);
	errorMessageText.setPosition(50, 500);  // Below player list
	errorMessageText.setString("");  // Start empty

	// Role-specific buttons - positioned below the default buttons  
	sf::Vector2f roleButtonSize(120, 40);
	sf::Color roleButtonColor(80, 120, 160);  // blue-ish

	float roleStartX = (800 - (3 * 120 + 2 * 15)) / 2.0f;  // Center for max 3 buttons
	float roleButtonY = 430;  // Below default buttons

	// Baron button - INVEST (needs target selection)
	baronInvestButton.setSize(roleButtonSize);
	baronInvestButton.setFillColor(roleButtonColor);
	baronInvestButton.setPosition(roleStartX, roleButtonY);

	baronInvestButtonText.setFont(font);
	baronInvestButtonText.setString("INVEST");
	baronInvestButtonText.setCharacterSize(12);
	baronInvestButtonText.setFillColor(sf::Color::White);
	centerTextInButton(baronInvestButtonText, baronInvestButton);

	// Spy buttons
	spyViewCoinsButton.setSize(roleButtonSize);
	spyViewCoinsButton.setFillColor(roleButtonColor);  
	spyViewCoinsButton.setPosition(roleStartX, roleButtonY);

	spyViewCoinsButtonText.setFont(font);
	spyViewCoinsButtonText.setString("VIEW COINS");
	spyViewCoinsButtonText.setCharacterSize(10);
	spyViewCoinsButtonText.setFillColor(sf::Color::White);
	centerTextInButton(spyViewCoinsButtonText, spyViewCoinsButton);

	spyBlockArrestButton.setSize(roleButtonSize);
	spyBlockArrestButton.setFillColor(roleButtonColor);
	spyBlockArrestButton.setPosition(roleStartX + 135, roleButtonY);

	spyBlockArrestButtonText.setFont(font);
	spyBlockArrestButtonText.setString("BLOCK ARREST");
	spyBlockArrestButtonText.setCharacterSize(9);
	spyBlockArrestButtonText.setFillColor(sf::Color::White);
	centerTextInButton(spyBlockArrestButtonText, spyBlockArrestButton);

	// Governor button
	governorBlockTaxButton.setSize(roleButtonSize);
	governorBlockTaxButton.setFillColor(roleButtonColor);
	governorBlockTaxButton.setPosition(roleStartX, roleButtonY);

	governorBlockTaxButtonText.setFont(font);
	governorBlockTaxButtonText.setString("BLOCK TAX");
	governorBlockTaxButtonText.setCharacterSize(11);
	governorBlockTaxButtonText.setFillColor(sf::Color::White);
	centerTextInButton(governorBlockTaxButtonText, governorBlockTaxButton);


	// Intervention UI setup
	interventionBox.setSize(sf::Vector2f(600, 200));
	interventionBox.setFillColor(sf::Color(40, 40, 40, 230));  // Semi-transparent dark gray
	interventionBox.setPosition(100, 250);  // Center of screen

	interventionPromptText.setFont(font);
	interventionPromptText.setCharacterSize(16);
	interventionPromptText.setFillColor(sf::Color::White);
	interventionPromptText.setPosition(120, 280);

	// Yes/No buttons for intervention
	yesButton.setSize(sf::Vector2f(100, 40));
	yesButton.setFillColor(sf::Color(0, 150, 0));  // Green
	yesButton.setPosition(200, 380);

	noButton.setSize(sf::Vector2f(100, 40));
	noButton.setFillColor(sf::Color(150, 0, 0));   // Red
	noButton.setPosition(400, 380);

	yesButtonText.setFont(font);
	yesButtonText.setString("YES");
	yesButtonText.setCharacterSize(16);
	yesButtonText.setFillColor(sf::Color::White);
	centerTextInButton(yesButtonText, yesButton);

	noButtonText.setFont(font);
	noButtonText.setString("NO");
	noButtonText.setCharacterSize(16);
	noButtonText.setFillColor(sf::Color::White);
	centerTextInButton(noButtonText, noButton);

	// --- GameOver screen setup ---
	gameOverText.setFont(font);
	gameOverText.setString("GAME OVER");
	gameOverText.setCharacterSize(48);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setPosition(400, 150);
	{
		sf::FloatRect tb = gameOverText.getLocalBounds();
		gameOverText.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
		gameOverText.setPosition(400, 150);
	}

	winnerText.setFont(font);
	winnerText.setCharacterSize(32);
	winnerText.setFillColor(sf::Color::Yellow);
	winnerText.setPosition(400, 220);

	// Play Again button
	playAgainButton.setSize(sf::Vector2f(200, 60));
	playAgainButton.setFillColor(sf::Color::Green);
	playAgainButton.setPosition(200, 350);

	playAgainButtonText.setFont(font);
	playAgainButtonText.setString("PLAY AGAIN");
	playAgainButtonText.setCharacterSize(20);
	playAgainButtonText.setFillColor(sf::Color::Black);
	centerTextInButton(playAgainButtonText, playAgainButton);

	// Exit Game button
	exitGameButton.setSize(sf::Vector2f(200, 60));
	exitGameButton.setFillColor(sf::Color::Red);
	exitGameButton.setPosition(400, 350);

	exitGameButtonText.setFont(font);
	exitGameButtonText.setString("EXIT GAME");
	exitGameButtonText.setCharacterSize(20);
	exitGameButtonText.setFillColor(sf::Color::Black);
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
				if(ev.type==sf::Event::TextEntered){
				  char c = static_cast<char>(ev.text.unicode);
				  if(std::isdigit(c) && countInput.size()<2){
					countInput.push_back(c);
				  }
				  else if(c=='\b' && !countInput.empty()){
					countInput.pop_back();
				  }
				  inputText.setString(countInput);
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
						nameInputText.setString("");
						screen = Screen::AskNames;
					} else {
						errorText.setString("Enter a number between 2 and 6");
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
				if (isButtonClicked(nextButton,mp)) {
				  current_game.start_game();
				  screen = Screen::Play;
				}
			  }
			  continue; // _EVERY_ AskNames event is eaten here
			}
		  
			// 1) TEXT ENTRY for name
			if (ev.type==sf::Event::TextEntered) {
			  char c = static_cast<char>(ev.text.unicode);
			  if (std::isprint(c) && nameInput.size()<16) nameInput.push_back(c);
			  else if (c=='\b' && !nameInput.empty())       nameInput.pop_back();
			  nameInputText.setString(nameInput);
			  continue;
			}
		  
			// 2) ENTER to register one name
			if (ev.type==sf::Event::KeyPressed && ev.key.code==sf::Keyboard::Enter) {
				if (!nameInput.empty()) {
					static std::random_device rd;
					static std::mt19937 gen(rd());
					static std::uniform_int_distribution<int> dis(0, 1);  // 0 to 5 inclusive
					Role role = static_cast<Role>(dis(gen));
					std::cout<< to_string(role);
				  try {
					PF.create_player(current_game, nameInput, role);
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
					
					if (isButtonClicked(yesButton, mp)) {
                        // Current intervenor wants to block
                        Player* intervenor = pendingInterventors[currentInterventorIndex];
                        
                        try {
                            if (interventionState == InterventionState::WAITING_GENERAL) {
                                // General blocks coup - pass true for shouldBlock
                                if (dynamic_cast<General*>(intervenor)->undo(*pendingAttacker, *pendingTarget, true)) {
                                    errorMessageText.setString(intervenor->getName() + " (General) blocked the coup!");
                                    interventionState = InterventionState::NONE;
                                    return;  // Action blocked, done
                                }
                            } else if (interventionState == InterventionState::WAITING_JUDGE) {
                                // Judge blocks bribe - pass true for shouldBlock
                                if (dynamic_cast<Judge*>(intervenor)->undo(*pendingAttacker, true)) {
                                    errorMessageText.setString(intervenor->getName() + " (Judge) blocked the bribe!");
                                    interventionState = InterventionState::NONE;
                                    return;  // Action blocked, done
                                }
                            }
                        } catch (const std::exception& e) {
                            errorMessageText.setString("Intervention failed: " + std::string(e.what()));
                        }
                    }
					else if (isButtonClicked(noButton, mp)) {
						// Current intervenor doesn't want to block - pass false for shouldBlock
						Player* intervenor = pendingInterventors[currentInterventorIndex];
						
						if (interventionState == InterventionState::WAITING_GENERAL) {
							dynamic_cast<General*>(intervenor)->undo(*pendingAttacker, *pendingTarget, false);
						} else if (interventionState == InterventionState::WAITING_JUDGE) {
							dynamic_cast<Judge*>(intervenor)->undo(*pendingAttacker, false);
						}
					}
					
					// Move to next intervenor or execute action
					currentInterventorIndex++;
					if (currentInterventorIndex >= pendingInterventors.size()) {
						// No more intervenors, execute the original action
						try {
							if (pendingActionType == "coup") {
								pendingAttacker->coup(*pendingTarget);
								errorMessageText.setString("COUPED " + pendingTarget->getName());
							} else if (pendingActionType == "bribe") {
								pendingAttacker->bribe();
								errorMessageText.setString("BRIBE SUCCESSFUL");
							}
						} catch (const std::exception& e) {
							errorMessageText.setString("ERROR: " + std::string(e.what()));
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
				// RETURN HERE - don't process any other events during intervention
				return;
			}

			// NORMAL GAME PROCESSING - only when not in intervention
			if (ev.type == sf::Event::MouseButtonPressed) {
				auto mp = sf::Vector2f(ev.mouseButton.x, ev.mouseButton.y);
				
				// Check for default action buttons
				if (isButtonClicked(gatherButton, mp)) {
					std::cout << "GATHER action clicked!" << std::endl;
					errorMessageText.setString("");  // Clear previous error
					try {
						Player* currentPlayer = current_game.get_current_player();
						if (currentPlayer) {
							currentPlayer->gather();
							std::cout << "Gather action performed successfully!" << std::endl;
						}
					} catch (const std::exception& e) {
						std::cout << "Gather failed: " << e.what() << std::endl;
						errorMessageText.setString("ERROR: " + std::string(e.what()));
					}
				}
				else if (isButtonClicked(taxButton, mp)) {
					std::cout << "TAX action clicked!" << std::endl;
					errorMessageText.setString("");  // Clear previous error
					try {
						Player* currentPlayer = current_game.get_current_player();
						if (currentPlayer) {
							currentPlayer->tax();
							std::cout << "Tax action performed successfully!" << std::endl;
						}
					} catch (const std::exception& e) {
						std::cout << "Tax failed: " << e.what() << std::endl;
						errorMessageText.setString("ERROR: " + std::string(e.what()));
					}
				}
				else if (isButtonClicked(bribeButton, mp)) {
					std::cout << "BRIBE action clicked!" << std::endl;
					errorMessageText.setString("");  // Clear previous error
					Player* attacker = current_game.get_current_player();
					if (attacker) {
						// gather all other Judges
						auto judges = current_game.get_judges();
						std::vector<Player*> eligibleJudges;
						for (auto* j : judges) {
							if (j != attacker) eligibleJudges.push_back(j);
						}

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
							return;  // <-- bail out now so we stay in intervention mode
						} else {
							// no Judges → execute immediately
							try {
								attacker->bribe();
								std::cout << "Bribe successful" << std::endl;
								errorMessageText.setString("BRIBE SUCCESSFUL");
							} catch (const std::exception& e) {
								std::cout << "Bribe failed: " << e.what() << std::endl;
								errorMessageText.setString("ERROR: " + std::string(e.what()));
							}
						}
					}
				}
				else if (isButtonClicked(arrestButton, mp)) {
					std::cout << "ARREST action clicked!" << std::endl;
					errorMessageText.setString("");  // Clear previous error
					
					// Switch to target selection mode (like sanction and coup)
					actionState = ActionState::SELECTING_TARGET;
					pendingAction = "arrest";
					actionPromptText.setString("CLICK ON A PLAYER TO ARREST");
				}
				else if (isButtonClicked(sanctionButton, mp)) {
					std::cout << "SANCTION action clicked!" << std::endl;
					errorMessageText.setString("");  // Clear previous error
					
					// Switch to target selection mode
					actionState = ActionState::SELECTING_TARGET;
					pendingAction = "sanction";
					actionPromptText.setString("CLICK ON A PLAYER TO SANCTION");
				}
				else if (isButtonClicked(coupButton, mp)) {
					std::cout << "COUP action clicked!" << std::endl;
					errorMessageText.setString("");  // Clear previous error
					
					// Switch to target selection mode
					actionState = ActionState::SELECTING_TARGET;
					pendingAction = "coup";
					actionPromptText.setString("CLICK ON A PLAYER TO COUP");
				}
				
				// Role-specific buttons
				if (current_game.get_current_player()) {
					Role currentRole = current_game.get_current_player()->getRole();
					
					if (currentRole == Role::BARON) {
						if (isButtonClicked(baronInvestButton, mp)) {
							std::cout << "Baron INVEST action clicked!" << std::endl;
							try {
								Player* currentPlayer = current_game.get_current_player();
								if (currentPlayer) {
									dynamic_cast<Baron*>(currentPlayer)->invest();
									std::cout << "Invest action performed successfully!" << std::endl;
									
								}
							} catch (const std::exception& e) {
								std::cout << "Invest failed: " << e.what() << std::endl;
								errorMessageText.setString("ERROR: " + std::string(e.what()));
							}
						}
					}
				    // Spy special actions  
					else if (currentRole == Role::SPY) {
						if (isButtonClicked(spyViewCoinsButton, mp)) {
							std::cout << "Spy VIEW COINS action clicked!" << std::endl;
							errorMessageText.setString("");  // Clear previous error
							
							// Switch to target selection mode for view coins
							actionState = ActionState::SELECTING_TARGET;
							pendingAction = "view_coins";
							actionPromptText.setString("CLICK ON A PLAYER TO VIEW COINS");
						}
						else if (isButtonClicked(spyBlockArrestButton, mp)) {
							std::cout << "Spy BLOCK ARREST action clicked!" << std::endl;
							errorMessageText.setString("");  // Clear previous error
							
							// Switch to target selection mode for block arrest
							actionState = ActionState::SELECTING_TARGET;
							pendingAction = "block_arrest";
							actionPromptText.setString("CLICK ON A PLAYER TO BLOCK ARREST");
						}
					}
					// Governor special actions
					else if (currentRole == Role::GOVERNOR) {
						 if (isButtonClicked(governorBlockTaxButton, mp)) {
							std::cout << "Governor BLOCK TAX action clicked!" << std::endl;
							errorMessageText.setString("");  // Clear previous error
							
							// Switch to target selection mode for block tax
							actionState = ActionState::SELECTING_TARGET;
							pendingAction = "block_tax";
							actionPromptText.setString("CLICK ON A PLAYER TO BLOCK TAX");
						}
					}
				}

			// Handle player selection when in target selection mode
			if (actionState == ActionState::SELECTING_TARGET) {
				// Check if user clicked on any player in the list
				auto players = current_game.get_players_objects();
				Player* currentPlayer = current_game.get_current_player();
				
				for (size_t i = 0; i < players.size(); ++i) {
					// Calculate the clickable area for each player name
					sf::FloatRect playerArea;
					playerArea.left = 50;
					playerArea.top = 150 + i * 35;
					playerArea.width = 400;  // Wide enough to cover the player text
					playerArea.height = 30;  // Height of one line of text
					
					// Check if mouse click is within this player's area
					if (playerArea.contains(mp)) {
						current_game.set_target_player(*players[i]);
						
						// // Can't target yourself (for most actions)
						// if (targetPlayer == currentPlayer && (pendingAction == "sanction" || pendingAction == "coup")) {
						// 	errorMessageText.setString("ERROR: Cannot target yourself!");
						// 	actionState = ActionState::CHOOSING_ACTION;
						// 	actionPromptText.setString("CHOOSE ACTION");
						// 	pendingAction = "";
						// 	break;
						// }
						
						// Execute the pending action on the selected target
						try {
							if (pendingAction == "sanction") {
								currentPlayer->sanction(current_game.get_target_player());  // Pass target player
								std::cout << "Sanction performed on " << current_game.get_target_player().getName() << std::endl;
								errorMessageText.setString("SANCTIONED " + current_game.get_target_player().getName());
							}
							else if (pendingAction == "coup") {
								// Check for General intervention before executing coup
								auto generals = current_game.get_generals();
								std::vector<Player*> eligibleGenerals;
								
								// Find generals who can intervene (not the attacker, have 5+ coins)
								for (Player* general : generals) {
									if (general != currentPlayer && general->coins() >= 5) {
										eligibleGenerals.push_back(general);
									}
								}
								
								if (!eligibleGenerals.empty()) {
									// Start intervention process
									interventionState = InterventionState::WAITING_GENERAL;
									pendingInterventors = eligibleGenerals;
									currentInterventorIndex = 0;
									pendingAttacker = currentPlayer;
									pendingTarget = &current_game.get_target_player();
									pendingActionType = "coup";
									
									// Set up intervention prompt
									interventionPromptText.setString(
										pendingInterventors[0]->getName() + " (General), do you want to block " +
										pendingAttacker->getName() + "'s coup on " + pendingTarget->getName() + "?"
									);
									
									// Don't execute coup yet - wait for intervention decision
									actionState = ActionState::CHOOSING_ACTION;
									actionPromptText.setString("CHOOSE ACTION");
									pendingAction = "";
									return;  // Exit without executing coup
								}
								
								// No eligible generals, execute coup normally
								currentPlayer->coup(current_game.get_target_player());
								std::cout << "Coup performed on " << current_game.get_target_player().getName() << std::endl;
								errorMessageText.setString("COUPED " + current_game.get_target_player().getName());
							}
							// else if (pendingAction == "bribe") {
							// 	// Check for Judge intervention before executing bribe
							// 	auto judges = current_game.get_judges();
							// 	std::vector<Player*> eligibleJudges;
								
							// 	// Find judges who can intervene (not the attacker)
							// 	for (Player* judge : judges) {
							// 		if (judge != currentPlayer) {
							// 			eligibleJudges.push_back(judge);
							// 		}
							// 	}
								
							// 	if (!eligibleJudges.empty()) {
							// 		// Start intervention process
							// 		interventionState = InterventionState::WAITING_JUDGE;
							// 		pendingInterventors = eligibleJudges;
							// 		currentInterventorIndex = 0;
							// 		pendingAttacker = currentPlayer;
							// 		pendingActionType = "bribe";
									
							// 		// Set up intervention prompt
							// 		interventionPromptText.setString(
							// 			pendingInterventors[0]->getName() + " (Judge), do you want to block " +
							// 			pendingAttacker->getName() + "'s bribe?"
							// 		);
									
							// 		// Don't execute bribe yet - wait for intervention decision
							// 		actionState = ActionState::CHOOSING_ACTION;
							// 		actionPromptText.setString("CHOOSE ACTION");
							// 		pendingAction = "";
							// 		return;  // Exit without executing bribe
							// 	}
								
							// 	// No eligible judges, execute bribe normally
							// 	currentPlayer->bribe();
							// 	std::cout << "Bribe performed successfully!" << std::endl;
							// 	errorMessageText.setString("BRIBE SUCCESSFUL");
							// }
							else if (pendingAction == "arrest") {  
								currentPlayer->arrest(current_game.get_target_player());  // Pass target player
								std::cout << "Arrest performed on " << current_game.get_target_player().getName() << std::endl;
								errorMessageText.setString("ARRESTED " + current_game.get_target_player().getName());
							}
							else if (pendingAction == "view_coins") { 
								dynamic_cast<Spy*>(currentPlayer)->view_coins(current_game.get_target_player());
								std::cout << "View coins performed on " << current_game.get_target_player().getName() << std::endl;
								errorMessageText.setString("VIEWED COINS OF '" + current_game.get_target_player().getName() +"'\
								\nNUMBER OF COINS: " + std::to_string(current_game.get_target_player().coins()));

							}
							else if (pendingAction == "block_arrest") { 
								dynamic_cast<Spy*>(currentPlayer)->disable_arrest(current_game.get_target_player());
								std::cout << "Block arrest performed on " << current_game.get_target_player().getName() << std::endl;
								errorMessageText.setString("BLOCKED ARREST FOR " + current_game.get_target_player().getName());

							}
							else if (pendingAction == "block_tax") {  
								dynamic_cast<Governor*>(currentPlayer)->block_tax(current_game.get_target_player());
								std::cout << "Block tax performed on " << current_game.get_target_player().getName() << std::endl;
								errorMessageText.setString("BLOCKED TAX FOR " + current_game.get_target_player().getName());

							}
							
							// Reset to normal action selection
							actionState = ActionState::CHOOSING_ACTION;
							actionPromptText.setString("CHOOSE ACTION");
							pendingAction = "";
							
						} catch (const std::exception& e) {
							std::cout << pendingAction << " failed: " << e.what() << std::endl;
							errorMessageText.setString("ERROR: " + std::string(e.what()));
							
							// Reset to normal action selection
							actionState = ActionState::CHOOSING_ACTION;
							actionPromptText.setString("CHOOSE ACTION");
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
				actionPromptText.setString("CHOOSE ACTION");
				pendingAction = "";
				errorMessageText.setString("Action cancelled");
			}
		}

			// Handle intervention decisions
	if (interventionState != InterventionState::NONE) {
		if (ev.type == sf::Event::MouseButtonPressed) {
			auto mp = sf::Vector2f(ev.mouseButton.x, ev.mouseButton.y);
			
			if (isButtonClicked(yesButton, mp)) {
				// Current intervenor wants to block
				Player* intervenor = pendingInterventors[currentInterventorIndex];
				
				try {
					if (interventionState == InterventionState::WAITING_GENERAL) {
						// General blocks coup - pass true for shouldBlock
						if (dynamic_cast<General*>(intervenor)->undo(*pendingAttacker, *pendingTarget, true)) {
							errorMessageText.setString(intervenor->getName() + " (General) blocked the coup!");
							interventionState = InterventionState::NONE;
							return;  // Action blocked, done
						}
					} else if (interventionState == InterventionState::WAITING_JUDGE) {
						// Judge blocks bribe - pass true for shouldBlock
						if (dynamic_cast<Judge*>(intervenor)->undo(*pendingAttacker, true)) {
							errorMessageText.setString(intervenor->getName() + " (Judge) blocked the bribe!");
							interventionState = InterventionState::NONE;
							return;  // Action blocked, done
						}
					}
				} catch (const std::exception& e) {
					errorMessageText.setString("Intervention failed: " + std::string(e.what()));
				}
			}
			else if (isButtonClicked(noButton, mp)) {
				// Current intervenor doesn't want to block - pass false for shouldBlock
				Player* intervenor = pendingInterventors[currentInterventorIndex];
				
				if (interventionState == InterventionState::WAITING_GENERAL) {
					dynamic_cast<General*>(intervenor)->undo(*pendingAttacker, *pendingTarget, false);
				} else if (interventionState == InterventionState::WAITING_JUDGE) {
					dynamic_cast<Judge*>(intervenor)->undo(*pendingAttacker, false);
				}
			}
			
			// Move to next intervenor or execute action
			currentInterventorIndex++;
			if (currentInterventorIndex >= pendingInterventors.size()) {
				// No more intervenors, execute the original action
				try {
					if (pendingActionType == "coup") {
						pendingAttacker->coup(*pendingTarget);
						errorMessageText.setString("COUPED " + pendingTarget->getName());
					} else if (pendingActionType == "bribe") {
						pendingAttacker->bribe();
						errorMessageText.setString("BRIBE SUCCESSFUL");
					}
				} catch (const std::exception& e) {
					errorMessageText.setString("ERROR: " + std::string(e.what()));
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
		return;  // Don't process other events during intervention
		}

    }
	else if (screen == Screen::GameOver) {
		if (ev.type == sf::Event::MouseButtonPressed) {
			auto mp = sf::Vector2f(ev.mouseButton.x, ev.mouseButton.y);
			
			if (isButtonClicked(playAgainButton, mp)) {
				// Reset game and go back to start
				// this->current_game = Game{};  // Reset the game
				screen = Screen::Menu;
				winnerName = "";
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
        window.draw(startButton);
        window.draw(startButtonText);
        window.draw(exitButton);
        window.draw(exitButtonText);
        window.draw(nameText);
        window.draw(emailText);
    }
    else if(screen==Screen::AskCount){
        window.draw(promptText);
        window.draw(inputText);
        window.draw(errorText);
    }
	else if (screen==Screen::AskNames) {
        window.draw(namePrompt);
        window.draw(nameInputText);
        // draw each registered player under the input (moved higher)
        for (size_t i = 0; i < current_game.get_players_objects().size(); ++i) {
            sf::Text info;
            info.setFont(font);
            info.setCharacterSize(18);
            info.setFillColor(sf::Color::Green);
            auto* pl = current_game.get_players_objects()[i];
            std::string line = pl->getName() + " : " + coup::to_string(pl->getRole());
            info.setString(line);
            info.setPosition(200, 230 + i * 22);  // moved from 280 to 230, smaller spacing
            window.draw(info);
        }
        // draw NEXT button if all names are complete
        if(namesComplete) {
            window.draw(nextButton);
            window.draw(nextButtonText);
        }
    }
	else if(screen==Screen::Play) {
		// Draw the title "COUP" at the top (reusing existing titleText)
		window.draw(titleText);
		
		// Get the list of all players from the game
		auto players = current_game.get_players_objects();
		// Get the player whose turn it is right now
		Player* currentPlayer = current_game.get_current_player();
		for (size_t i = 0; i < players.size(); ++i) {
			sf::Text playerInfo;
			playerInfo.setFont(font);
			playerInfo.setCharacterSize(20);
		
			// ALWAYS include coin count
			std::string playerText = players[i]->getName() + " " + to_string(players[i]->getRole()) + " (" + std::to_string(players[i]->coins()) + " coins)";
		
			// Highlight current player or show clickable state
			if (players[i] == currentPlayer) {
				playerInfo.setFillColor(sf::Color(255,215,0)); // gold
			} else if (actionState == ActionState::SELECTING_TARGET) {
				playerInfo.setFillColor(sf::Color::Cyan);
			} else {
				playerInfo.setFillColor(sf::Color::White);
			}
		
			playerInfo.setString(playerText);
			playerInfo.setPosition(50, 150 + i * 35);
			window.draw(playerInfo);
		}
		// // Loop through each player to display them
		// for (size_t i = 0; i < players.size(); ++i) {
		// 	// Create a new text object for this player's info
		// 	sf::Text playerInfo;
		// 	// Use the same font as everything else
		// 	playerInfo.setFont(font);
		// 	// Set the text size
		// 	playerInfo.setCharacterSize(20);
			
		// 	// This will hold the text we want to display for this player
		// 	std::string playerText;
			
		// 	// Check if this player is the one whose turn it is
		// 	if (players[i] == currentPlayer) {
		// 		// This is the current player - highlight them in gold
		// 		playerInfo.setFillColor(sf::Color(255, 215, 0)); // gold color
		// 		// Show their name AND coin count: "Alice (3 coins)"
		// 		playerText = players[i]->getName() + " " + to_string(players[i]->getRole()) + " (" + std::to_string(players[i]->coins()) + " coins)";
		// 	} else {
		// 		// This is NOT the current player - show in white
		// 		if (actionState == ActionState::SELECTING_TARGET) {
		// 			// If we're selecting a target, make other players cyan to show they're clickable
		// 			playerInfo.setFillColor(sf::Color::Cyan);
		// 		} else {
		// 			playerInfo.setFillColor(sf::Color::White);
		// 		}
		// 		// Only show their name, hide their coins: "Bob"
		// 		playerText = players[i]->getName();
		// 	}
			
		// 	// Set the text content
		// 	playerInfo.setString(playerText);
		// 	// Position this player's text on the left side of screen
		// 	// Each player is 35 pixels below the previous one
		// 	playerInfo.setPosition(50, 150 + i * 35);
		// 	// Actually draw this player's text to the screen
		// 	window.draw(playerInfo);
		// }

		// Draw the action prompt box and text
		window.draw(actionPromptBox);
		window.draw(actionPromptText);
		
		// Draw error message if there is one
		window.draw(errorMessageText);

		// Draw all the default action buttons at the bottom
		window.draw(gatherButton);         // Draw the gray button background
		window.draw(gatherButtonText);     // Draw the white "GATHER" text on top
		window.draw(taxButton);            // Draw the gray button background
		window.draw(taxButtonText);        // Draw the white "TAX" text on top
		window.draw(bribeButton);          // Draw the gray button background
		window.draw(bribeButtonText);      // Draw the white "BRIBE" text on top
		window.draw(arrestButton);         // Draw the gray button background
		window.draw(arrestButtonText);     // Draw the white "ARREST" text on top
		window.draw(sanctionButton);       // Draw the gray button background
		window.draw(sanctionButtonText);   // Draw the white "SANCTION" text on top
		window.draw(coupButton);           // Draw the gray button background
		window.draw(coupButtonText);       // Draw the white "COUP" text on top


		// Draw role-specific buttons only for current player's role
		if (currentPlayer) {
			Role currentRole = currentPlayer->getRole();
			
			// Only draw the buttons that match the current player's role
			switch(currentRole) {
				case Role::BARON:
					// Baron gets 1 button: INVEST
					window.draw(baronInvestButton);
					window.draw(baronInvestButtonText);
					break;
				case Role::SPY:
					// Spy gets 2 buttons: VIEW COINS and BLOCK ARREST
					window.draw(spyViewCoinsButton);
					window.draw(spyViewCoinsButtonText);
					window.draw(spyBlockArrestButton);
					window.draw(spyBlockArrestButtonText);
					break;
				case Role::GOVERNOR:
					// Governor gets 1 button: BLOCK TAX
					window.draw(governorBlockTaxButton);
					window.draw(governorBlockTaxButtonText);
					break;
				// Other roles don't have special buttons
				case Role::GENERAL:
				case Role::JUDGE:
				case Role::MERCHANT:
					// No special buttons for these roles
					break;
			}
		}

		// Draw intervention UI if active
		if (interventionState != InterventionState::NONE) {
			window.draw(interventionBox);
			window.draw(interventionPromptText);
			window.draw(yesButton);
			window.draw(yesButtonText);
			window.draw(noButton);
			window.draw(noButtonText);
		}


	}

	else if (screen == Screen::GameOver) {
		window.draw(gameOverText);
		window.draw(winnerText);
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

// void Window::checkGameOver() {
//     try {
//         // Use the game's winner() function to check if there's a winner
//         std::string winner = current_game.winner();
        
//         // If winner() returns successfully, we have a winner!
//         winnerName = winner;
//         winnerText.setString("WINNER: " + winnerName + "!");
        
//         // Center the winner text
//         sf::FloatRect tb = winnerText.getLocalBounds();
//         winnerText.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
//         winnerText.setPosition(400, 220);
        
//         screen = Screen::GameOver;
//         std::cout << "Game Over! Winner: " << winnerName << std::endl;
//         return;
        
//     } catch (const std::exception& e) {
//         // If winner() throws an exception, it means game isn't over yet
//         std::cout << "Game continues: " << e.what() << std::endl;
        
//         // Move to next turn since game continues
//         try {
//             current_game.next_turn();
//             std::cout << "Turn advanced to: " << current_game.turn() << std::endl;
//         } catch (const std::exception& turnError) {
//             std::cout << "Turn error: " << turnError.what() << std::endl;
//             errorMessageText.setString("Turn error: " + std::string(turnError.what()));
//         }
//     }
// }