#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "PlayerFactory.hpp"
#include "Role.hpp"

using namespace coup;

// Test fixture with 6 players, each with different roles
struct CoupTestFixture {
    Game game;
    Player* gov;
    Player* spy;
    Player* baron;
    Player* general;
    Player* judge;
    Player* merchant;

    CoupTestFixture() {
        // Create and register players using PlayerFactory
        gov = &PlayerFactory::create_player(game, "Gov", Role::GOVERNOR);
        spy = &PlayerFactory::create_player(game, "Spy", Role::SPY);
        baron = &PlayerFactory::create_player(game, "Baron", Role::BARON);
        general = &PlayerFactory::create_player(game, "General", Role::GENERAL);
        judge = &PlayerFactory::create_player(game, "Judge", Role::JUDGE);
        merchant = &PlayerFactory::create_player(game, "Merchant", Role::MERCHANT);
    }
};

// =============================================================================
// TEST 1: GENERAL GAME MECHANICS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 1.1: Turn order and next turn advancement") {
    // Test initial turn order
    CHECK(game.turn() == "Gov");
    
    // Test that each action advances to next turn
    gov->gather();
    CHECK(game.turn() == "Spy");
    
    spy->gather();
    CHECK(game.turn() == "Baron");
    
    baron->gather();
    CHECK(game.turn() == "General");
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 1.2: Action validation - sufficient coins") {
    // Test gather action (always should work)
    int beforeCoins = gov->coins();
    gov->gather();
    CHECK(gov->coins() == beforeCoins + 1);
    
    // Test tax action 
    int beforeTax = spy->coins();
    spy->tax();
    CHECK(spy->coins() == beforeTax + 2);
    
    // Test bribe action - need 4 coins
    // Give baron enough coins for bribe
    baron->addCoins(4);
    int beforeBribe = baron->coins();
    baron->bribe();
    CHECK(baron->coins() == beforeBribe - 4);
    CHECK(game.turn() == "Baron"); // Should still be baron's turn after bribe
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 1.3: Action validation - insufficient coins throws") {
    // Test bribe with insufficient coins
    CHECK_THROWS_AS(general->bribe(), std::exception);
    
    // Test sanction with insufficient coins (needs 3)
    CHECK_THROWS_AS(judge->sanction(*merchant), std::exception);
    
    // Test coup with insufficient coins (needs 7)
    CHECK_THROWS_AS(merchant->coup(*gov), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 1.4: Actions on inactive players throws") {
    // First eliminate a player with coup
    gov->addCoins(7);
    gov->coup(*spy);
    CHECK_FALSE(spy->getActive());
    
    // Try to perform actions on inactive player - should throw
    CHECK_THROWS_AS(baron->arrest(*spy), std::exception);
    CHECK_THROWS_AS(general->sanction(*spy), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 1.5: Blocked actions throw exceptions") {
    // Sanction baron to block his gather/tax
	gov->addCoins(3);
    gov->sanction(*spy);
    
    // Baron should not be able to gather or tax while sanctioned
    CHECK_THROWS_AS(spy->gather(), std::exception);
    CHECK_THROWS_AS(spy->tax(), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 1.6: Player with 10+ coins can only coup") {
    // Give governor 10 coins
    gov->addCoins(10);
    
    // Should not be able to do other actions
    CHECK_THROWS_AS(gov->gather(), std::exception);
    CHECK_THROWS_AS(gov->tax(), std::exception);
    
    // Should be able to coup
    gov->coup(*spy);
    CHECK_FALSE(spy->getActive());
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 1.7: Winner announcement") {
    // Eliminate all players except one
    gov->addCoins(10); // Add coins for coup
    
    // Gov eliminates spy
    gov->coup(*spy);
    
    // Continue eliminating players until only one remains
    baron->addCoins(10); // Add coins for coup
    baron->coup(*general);
    
    judge->addCoins(10); // Add coins for coup
    judge->coup(*merchant);
    
    gov->addCoins(10); // Add coins for coup
    gov->coup(*baron);
    
    judge->addCoins(10); // Add coins for coup
    judge->coup(*gov);
    
    // Now only judge should remain
    CHECK(game.winner() == "Judge");
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 1.8: Winner throws when multiple players active") {
    // With multiple players still active, winner() should throw
    CHECK_THROWS_AS(game.winner(), std::exception);
}

// =============================================================================
// TEST 2: SPY SPECIFIC TESTS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 2.1: Spy view_coins functionality") {
    // Give gov some coins
    gov->addCoins(42); // Add coins for view_coins test
    
    // Spy should be able to view baron's coins
    // Cast to Spy to access spy-specific methods
    Spy* spyPlayer = dynamic_cast<Spy*>(spy);
    REQUIRE(spyPlayer != nullptr);
    game.next_turn(); // Go to spy turn
    CHECK_NOTHROW(spyPlayer->view_coins(*baron));
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 2.2: Spy block arrest functionality") {
	game.next_turn(); // go to spy turn
    // Cast to Spy to access spy-specific methods
    Spy* spyPlayer = dynamic_cast<Spy*>(spy);
    REQUIRE(spyPlayer != nullptr);
    
    // Spy blocks arrest on baron
    spyPlayer->disable_arrest(*baron);
    
    // Now baron should not be able to arrest general
    CHECK_THROWS_AS(baron->arrest(*general), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 2.3: Spy block arrest doesn't advance turn") {
	game.next_turn(); // go to spy turn
    // Cast to Spy to access spy-specific methods
    Spy* spyPlayer = dynamic_cast<Spy*>(spy);
    REQUIRE(spyPlayer != nullptr);
    
    std::string turnBefore = game.turn();
    spyPlayer->disable_arrest(*baron);
    // Turn should remain the same after blocking arrest
    CHECK(game.turn() == turnBefore);
}

// =============================================================================
// TEST 3: BARON SPECIFIC TESTS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 3.1: Baron invest functionality") {
	game.next_turn(); // go to spy turn
	game.next_turn(); // go to baron turn

    // Cast to Baron to access baron-specific methods
    Baron* baronPlayer = dynamic_cast<Baron*>(baron);
    REQUIRE(baronPlayer != nullptr);
    
    // Give baron enough coins to invest (needs 3)
	baron->addCoins(3);
    int beforeInvest = baron->coins();
    
    baronPlayer->invest();
    // Should spend 3 and gain 6, net gain of 3
    CHECK(baron->coins() == beforeInvest + 3);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 3.2: Baron compensation after sanction") {
    // When baron's turn comes, he should get compensation
    int beforeCompensation = baron->coins();

    // Sanction baron
    gov->addCoins(6);
    gov->sanction(*baron);

	game.next_turn(); // go to baron turn
    CHECK(baron->coins() == beforeCompensation + 1); // +1 compensation
}

// =============================================================================
// TEST 4: GOVERNOR SPECIFIC TESTS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 4.1: Governor enhanced tax") {
    // Governor should get 3 coins from tax instead of 2
    int beforeTax = gov->coins();
    gov->tax();
    CHECK(gov->coins() == beforeTax + 3);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 4.2: Governor blocks tax functionality") {
    int spyCoinsBeforeTax = spy->coins();

    // Cast to Governor to access governor-specific methods
    Governor* govPlayer = dynamic_cast<Governor*>(gov);
    REQUIRE(govPlayer != nullptr);
    
    govPlayer->block_tax(*spy);

    // Spy should not be able to tax next turn
    CHECK_THROWS_AS(spy->tax(), std::exception);

	// Spy should not gain any coins from tax, cause they are blocked
	CHECK(spy->coins() == spyCoinsBeforeTax);

}

// =============================================================================
// TEST 5: MERCHANT SPECIFIC TESTS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 5.1: Merchant automatic coin bonus") {
	// Go to merchant turn
	game.next_turn();
	game.next_turn();
	game.next_turn();
	game.next_turn();
	game.next_turn();
    // Give merchant 3 coins
	merchant->addCoins(3);
    int beforeBonus = merchant->coins();
    
    // Complete a full round to get back to merchant
	game.next_turn();
	game.next_turn();
	game.next_turn();
	game.next_turn();
	game.next_turn();
	game.next_turn();
    
    // When it's merchant's turn again, should automatically get +1 coin
    int afterBonus = merchant->coins();
    CHECK(afterBonus == beforeBonus + 1);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 5.2: Merchant pays double when arrested") {
		// Go to merchant turn
		game.next_turn();
		game.next_turn();
		game.next_turn();
		game.next_turn();
		game.next_turn();
    // Give merchant some coins
	merchant->tax();
    int beforeArrest = merchant->coins();
    
    // Arrest merchant
    gov->arrest(*merchant);
    
    // Merchant should lose 2 coins instead of 1
    CHECK(merchant->coins() == beforeArrest - 2);
}

// =============================================================================
// TEST 6: JUDGE SPECIFIC TESTS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 6.1: Judge blocks bribe intervention") {
	//Advance to spy
	game.next_turn();

    // Cast to Judge to access judge-specific methods
    Judge* judgePlayer = dynamic_cast<Judge*>(judge);
    REQUIRE(judgePlayer != nullptr);
    
    // Give spy enough coins for bribe
    spy->addCoins(4);
    int beforeBribe = spy->coins();

	// Judge should be able to intervene and block
	judgePlayer->undo(*spy, true); // Judge blocks the bribe

	spy->bribe();
    
    // Spy should lose the coins spent on bribe
    CHECK(spy->coins() == beforeBribe - 4);

	// going next turn to see if bribe didn't initiated
	game.next_turn();
	CHECK(game.turn() != spy->getName());

}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 6.2: Judge sanction penalty") {
    // Sanction judge - attacker should pay extra coin
    gov->addCoins(4); // 3 for sanction + 1 penalty
    gov->sanction(*judge);
    
    // Spy should have paid 4 coins total (3 for sanction + 1 penalty)
    CHECK(gov->coins() == 0);
}

// =============================================================================
// TEST 7: GENERAL SPECIFIC TESTS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 7.1: General compensation after arrest") {

	general->addCoins(1);
	int beforeArrestGeneral = general->coins();
    // Arrest general
    gov->arrest(*general);
    
    // General's turn, should get compensation
    CHECK(general->coins() == beforeArrestGeneral); // Should not change due to compensation
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 7.2: General blocks coup") {
    // Give general enough coins to block (needs 5)
    general->addCoins(5);

    // Cast to General to access general-specific methods
    General* generalPlayer = dynamic_cast<General*>(general);
    REQUIRE(generalPlayer != nullptr);
    
    // Give spy enough coins for coup
    gov->addCoins(7);
	gov->pay_coup_cost(*spy);

    generalPlayer->undo(*gov,*spy,true);

    // Spy tries to coup general
    gov->coup(*spy);
    
    // General should still be active if he blocked
    // Note: This depends on the undo implementation
    CHECK(spy->getActive()); // Should still be active if block was successful

	// Check if coins paid, 7-7 = 0
	CHECK(gov->coins() == 0);
}

// =============================================================================
// TEST 8: EXTREME CASES AND EDGE CONDITIONS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 8.1: Arrest same person twice throws") {
	// Added 1 coin for the arrest action
	spy->addCoins(1);
    gov->arrest(*spy);
    
    // Complete a round to get back to gov
	game.next_turn();
	game.next_turn();
	game.next_turn();
	game.next_turn();
	game.next_turn();
	game.next_turn();
    
    // Trying to arrest same person again should throw
    CHECK_THROWS_AS(gov->arrest(*spy), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 8.2: Actions on inactive players throw") {
    // Eliminate spy
    gov->addCoins(7);
    gov->coup(*spy);
    
	Spy* spyPlayer = dynamic_cast<Spy*>(spy);

    // All actions on inactive player should throw
	CHECK_THROWS_AS(spyPlayer->gather(), std::exception);
	CHECK_THROWS_AS(spyPlayer->tax(), std::exception);
	CHECK_THROWS_AS(spyPlayer->bribe(), std::exception);
    CHECK_THROWS_AS(spyPlayer->arrest(*spy), std::exception);
    CHECK_THROWS_AS(spyPlayer->sanction(*spy), std::exception);
	CHECK_THROWS_AS(spyPlayer->coup(*spy), std::exception);
	CHECK_THROWS_AS(spyPlayer->view_coins(*spy), std::exception);
	CHECK_THROWS_AS(spyPlayer->disable_arrest(*spy), std::exception);



}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 8.3: Undo without sufficient coins throws") {
    // Cast to access specific methods
    General* generalPlayer = dynamic_cast<General*>(general);
    Judge* judgePlayer = dynamic_cast<Judge*>(judge);
    REQUIRE(generalPlayer != nullptr);
    REQUIRE(judgePlayer != nullptr);
    
    // General tries to undo without enough coins (needs 5)

	// Adding 7 coins for coup
    gov->addCoins(7);
    // General doesn't have enough coins to block, should return false
    CHECK_FALSE(generalPlayer->undo(*gov, *baron, true));
    gov->coup(*baron);
	// Check if coup succeeded, baron active = false
	CHECK_FALSE(baron->getActive());

	// Adding 4 coins for bribe
	spy->addCoins(4);
    // Judge tries to undo without being in right context
	judgePlayer->undo(*spy, false);
	spy->bribe();
	// check if bribe succeeded 
	CHECK(game.turn() == spy->getName());
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 8.4: Wrong player acts out of turn") {
    // It's Gov's turn initially
    CHECK(game.turn() == "Gov");
    
    // Other players trying to act should throw
    CHECK_THROWS_AS(spy->gather(), std::exception);
    CHECK_THROWS_AS(baron->tax(), std::exception);
    CHECK_THROWS_AS(general->gather(), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 8.5: Multiple roles interaction") {
    // Test that two players with same role don't interfere
    Game g2;
    Player& g1 = PlayerFactory::create_player(g2, "Gov1", Role::GOVERNOR);
    Player& g2_player = PlayerFactory::create_player(g2, "Gov2", Role::GOVERNOR);
    g2.start_game();
    
    // Both should be able to use enhanced tax independently
    g1.tax();
    CHECK(g1.coins() == 3);
    
    g2_player.tax();
    CHECK(g2_player.coins() == 3);
}
