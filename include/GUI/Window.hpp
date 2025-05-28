#pragma once

#include <SFML/Graphics.hpp>
#include "../Game.hpp"
#include "../PlayerFactory.hpp"

#include <string>

using namespace coup;
class Window {
private:
    enum class Screen { Menu, 
                        AskCount, 
                        AskNames,
                        Play,
                        GameOver
                        };

    Screen screen = Screen::Menu;

    sf::RenderWindow window;
    sf::Font font;
    
    // -- GameOver --
    sf::Text winnerText;
    sf::Text gameOverText;
    sf::RectangleShape playAgainButton, exitGameButton;
    sf::Text playAgainButtonText, exitGameButtonText;
    std::string winnerName = "";

    // --- Menu ---
    sf::Text titleText, startButtonText, exitButtonText, nameText, emailText;
    sf::RectangleShape startButton, exitButton;

    // --- ask count ---
    sf::Text promptText, inputText, errorText;
    std::string countInput;

    // --- ask names ---
    sf::Text namePrompt, nameInputText;
    std::string nameInput;
    bool namesComplete = false;
    sf::RectangleShape nextButton;
    sf::Text            nextButtonText;
    
    // --- play screen ---
    sf::Text currentPlayerRoleText;
    sf::RectangleShape actionPromptBox;
    sf::Text actionPromptText;
    sf::Text errorMessageText;

    // Default buttons (available to all players)
    sf::RectangleShape gatherButton, taxButton, bribeButton, arrestButton, sanctionButton, coupButton;
    sf::Text gatherButtonText, taxButtonText, bribeButtonText, arrestButtonText, sanctionButtonText, coupButtonText;

    // Role-specific buttons
    // Baron buttons
    sf::RectangleShape baronInvestButton;
    sf::Text baronInvestButtonText;

    // Spy buttons
    sf::RectangleShape spyViewCoinsButton, spyBlockArrestButton;
    sf::Text spyViewCoinsButtonText, spyBlockArrestButtonText;

    // Governor buttons
    sf::RectangleShape governorBlockTaxButton;
    sf::Text governorBlockTaxButtonText;

    std::string commandInput;
    sf::Text commandText;
    

    // Intervention system
    enum class InterventionState {
        NONE,              // No intervention needed
        WAITING_GENERAL,   // Waiting for General to decide on coup intervention
        WAITING_JUDGE      // Waiting for Judge to decide on bribe intervention
    };
    InterventionState interventionState = InterventionState::NONE;
    std::vector<Player*> pendingInterventors;  // List of generals/judges who can intervene
    size_t currentInterventorIndex = 0;        // Which general/judge we're asking
    Player* pendingAttacker = nullptr;         // Who performed the action
    Player* pendingTarget = nullptr;           // Target of the action (for coup)
    std::string pendingActionType = "";        // "coup" or "bribe"

    // Intervention UI elements
    sf::RectangleShape interventionBox;
    sf::Text interventionPromptText;
    sf::RectangleShape yesButton, noButton;
    sf::Text yesButtonText, noButtonText;
    

    // Player selection for targeted actions
    enum class ActionState { 
        CHOOSING_ACTION,     // Normal state - choosing what action to do
        SELECTING_TARGET     // Waiting for player to click on a target player
    };

    ActionState actionState = ActionState::CHOOSING_ACTION;
    std::string pendingAction = "";  // Store which action we're waiting to execute ("sanction", "coup", etc.)


    /**
     * @brief adjusts the position of an sf::Text object so that it is 
     * centered within the bounds of a given sf::RectangleShape button. 
     * It takes a reference to the text and the button as parameters, 
     * modifying the text's position directly.
     */
    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);

    // factory + Game
    int num_of_players; // Number 
    int index_nop; // index of num of player
    Game current_game; // Game instance for the game
    PlayerFactory PF;

    /**
     * @brief responsible for setting up or initializing 
     * the components associated with the window. 
     * This function likely prepares the internal state or 
     * UI elements required for the window to function properly.
     * 
     */
    void initializeComponents();
    
    /**
     * @brief handles events related to the window, 
     * such as user input or system messages. 
     * It does not return a value and is likely 
     * intended to be called as part of the window's event loop.
     * 
     */
    void handleEvents();

    /**
     * @brief 
     * responsible for rendering the contents of the window. It likely handles drawing operations or 
     * updating the visual representation of the window on the screen.
     * 
     */
    void render();
    
    /**
     * @brief  determines whether a button, represented by an sf::RectangleShape, 
     * has been clicked based on the provided mouse position (sf::Vector2f). 
     * It returns a boolean value indicating 
     * whether the mouse position intersects with the button's boundaries.
     * 
     * @param button 
     * @param mousePos 
     * @return true 
     * @return false 
     */
    bool isButtonClicked(const sf::RectangleShape& button, const sf::Vector2f& mousePos);

    /**
     * @brief responsible for determining whether a game has ended. 
     * It does not take any parameters and does not return a value, 
     * suggesting it performs its checks internally, possibly updating 
     * the state of the Window or related components.
     * 
     */
    void checkGameOver();

    /**
     * @brief sets the Action Prompt object by accepting a constant reference to a std::string containing the desired message. 
     * This function allows updating or defining the prompt message for user interaction or display purposes.
     * 
     * @param message 
     */
    void setActionPrompt(const std::string& message);

public:
    Window(Game& current_game);
    ~Window();
    void run();
};

