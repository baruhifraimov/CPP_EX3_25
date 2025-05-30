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
        
        // Now start the game with registered players
        game.start_game();
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
// =============================================================================
// ADDITIONAL COMPREHENSIVE TESTS - COVERING ALL MISSING FUNCTIONS
// =============================================================================

// =============================================================================
// TEST 9: PLAYER VALIDATION AND EDGE CASES
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 9.1: Player validate_active functionality") {
    // Eliminate spy to make inactive
    gov->addCoins(7);
    gov->coup(*spy);
    
    // Validate active should throw for inactive player
    CHECK_THROWS_AS(spy->validate_active(), std::exception);
    
    // Should work for active players
    CHECK_NOTHROW(baron->validate_active());
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 9.2: Player addCoins with negative overflow") {
    // Try to remove more coins than player has
    int currentCoins = gov->coins();
    CHECK_THROWS_AS(gov->addCoins(-(currentCoins + 1)), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 9.3: IsOver10Coins boundary testing") {
    // Test exactly 10 coins
    gov->addCoins(10);
    CHECK(gov->IsOver10Coins());
    
    // Test 9 coins (should be false)
    spy->addCoins(9);
    CHECK_FALSE(spy->IsOver10Coins());
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 9.4: pay_coup_cost functionality") {
    // Test insufficient coins for coup cost
    CHECK_THROWS_AS(gov->pay_coup_cost(*spy), std::exception);
    
    // Test self-coup attempt
    gov->addCoins(7);
    CHECK_THROWS_AS(gov->pay_coup_cost(*gov), std::exception);
    
    // Test valid coup cost payment
    int beforeCoins = gov->coins();
    CHECK_NOTHROW(gov->pay_coup_cost(*spy));
    CHECK(gov->coins() == beforeCoins - 7);
}

// =============================================================================
// TEST 10: OPERATION BLOCKING SYSTEM
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 10.1: Block operation timers") {
    // Sanction spy to block operations (gov's turn)
    gov->addCoins(3);
    gov->sanction(*spy);
    
    // Move to spy's turn - spy should be blocked from gather and tax
    CHECK(game.turn() == "Spy");
    CHECK_THROWS_AS(spy->gather(), std::exception);
    CHECK_THROWS_AS(spy->tax(), std::exception);
    
    // After advancing turns, blocks should expire
    game.next_turn(); // Baron
    game.next_turn(); // General
    game.next_turn(); // Judge
    game.next_turn(); // Merchant
    game.next_turn(); // Gov
    game.next_turn(); // Spy
    
    // Now spy should be able to act again
    CHECK_NOTHROW(spy->gather());
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 10.2: EXTRA_TURN operation blocking") {
    // Move to spy's turn and give enough for bribe
    game.next_turn(); // Spy's turn
    spy->addCoins(4);
    
    // Bribe should give extra turn
    spy->bribe();
    CHECK(game.turn() == "Spy"); // Should still be spy's turn
    
    // After another action, should advance
    spy->gather();
    CHECK(game.turn() == "Spy");
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 10.3: Double arrest protection") {
	baron->addCoins(5); // add enough money for baron to be capable for arrest
    // Gov arrests spy (gov's turn)
    spy->addCoins(1);
    gov->arrest(*baron);
    
    // Move to baron's turn - baron tries to arrest spy again - should throw
    CHECK(game.turn() == "Spy");
    CHECK_THROWS_AS(spy->arrest(*baron), std::exception);
}

// =============================================================================
// TEST 11: ROLE-SPECIFIC ADVANCED TESTS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 11.1: Governor block_tax edge cases") {
    Governor* govPlayer = dynamic_cast<Governor*>(gov);
    REQUIRE(govPlayer != nullptr);
    
    // Governor tries to block own tax - should throw
    CHECK_THROWS_AS(govPlayer->block_tax(*gov), std::exception);
    
    // Move to spy's turn, spy does tax
    game.next_turn();
    spy->tax();
    
    // Move through turns back to gov
    CHECK(game.turn() == "Baron");
    baron->gather();
    general->gather(); 
    judge->gather(); 
    merchant->gather();
    
    // Gov blocks spy's tax
    govPlayer->block_tax(*spy);
    
    // Move to spy's turn - spy should not be able to tax
    game.next_turn();
    CHECK_THROWS_AS(spy->tax(), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 11.2: Spy disable_arrest edge cases") {
    // Move to spy's turn
    game.next_turn();
    
    Spy* spyPlayer = dynamic_cast<Spy*>(spy);
    REQUIRE(spyPlayer != nullptr);
    
    // Spy tries to disable arrest on self - should throw
    CHECK_THROWS_AS(spyPlayer->disable_arrest(*spy), std::exception);
    
    // Test with 10+ coins - should throw
    spy->addCoins(10);
    CHECK_THROWS_AS(spyPlayer->disable_arrest(*baron), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 11.3: Baron invest edge cases") {
    // Move to baron's turn
    game.next_turn(); // Spy
    game.next_turn(); // Baron
    
    Baron* baronPlayer = dynamic_cast<Baron*>(baron);
    REQUIRE(baronPlayer != nullptr);
    
    // Baron with insufficient coins for invest
    CHECK_THROWS_AS(baronPlayer->invest(), std::exception);
    
    // Baron with exactly 3 coins
    baron->addCoins(3);
    int beforeInvest = baron->coins();
    baronPlayer->invest();
    CHECK(baron->coins() == beforeInvest + 3); // -3 + 6 = +3 net
    
    // Move to new baron turn with 10+ coins - should throw
    game.next_turn(); // General
    game.next_turn(); // Judge
    game.next_turn(); // Merchant
    game.next_turn(); // Gov
    game.next_turn(); // Spy
    game.next_turn(); // Baron
    
    baron->addCoins(10);
    CHECK_THROWS_AS(baronPlayer->invest(), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 11.4: General undo insufficient coins") {
    // Move to general's turn
    game.next_turn(); // Spy
    game.next_turn(); // Baron
    game.next_turn(); // General
    
    General* generalPlayer = dynamic_cast<General*>(general);
    REQUIRE(generalPlayer != nullptr);
    
    // General without enough coins to block coup
    CHECK_FALSE(generalPlayer->undo(*spy, *baron, true));
    
    // General with enough coins can block
    general->addCoins(5);
    CHECK(generalPlayer->undo(*spy, *baron, true));
    CHECK(game.get_general_intervention());
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 11.5: Judge undo functionality") {
    // Move to judge's turn
    game.next_turn(); // Spy
    game.next_turn(); // Baron
    game.next_turn(); // General
    game.next_turn(); // Judge
    
    Judge* judgePlayer = dynamic_cast<Judge*>(judge);
    REQUIRE(judgePlayer != nullptr);
    
    // Judge blocks bribe
    CHECK(judgePlayer->undo(*spy, true));
    CHECK(game.get_judge_intervention());
    
    // Judge doesn't block
    CHECK_FALSE(judgePlayer->undo(*spy, false));
    CHECK_FALSE(game.get_judge_intervention());
}

// =============================================================================
// TEST 12: GAME MECHANICS COMPREHENSIVE
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 12.1: Turn validation comprehensive") {
    // Only current player should be able to act
    CHECK(game.turn() == "Gov");
    
    // All other players should throw when trying to act
    CHECK_THROWS_AS(spy->gather(), std::exception);
    CHECK_THROWS_AS(baron->gather(), std::exception);
    CHECK_THROWS_AS(general->gather(), std::exception);
    CHECK_THROWS_AS(judge->gather(), std::exception);
    CHECK_THROWS_AS(merchant->gather(), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 12.2: Sanction self-targeting prevention") {
    // Give gov enough coins for sanction
    gov->addCoins(3);
    
    // Should not be able to sanction self
    CHECK_THROWS_AS(gov->sanction(*gov), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 12.3: All actions disabled with 10+ coins except coup") {
    // Give gov 10+ coins
    gov->addCoins(11);
    
    // All actions except coup should throw
    CHECK_THROWS_AS(gov->gather(), std::exception);
    CHECK_THROWS_AS(gov->tax(), std::exception);
    CHECK_THROWS_AS(gov->bribe(), std::exception);
    CHECK_THROWS_AS(gov->arrest(*spy), std::exception);
    CHECK_THROWS_AS(gov->sanction(*spy), std::exception);
    
    // Only coup should work
    CHECK_NOTHROW(gov->coup(*spy));
}

// =============================================================================
// TEST 13: MERCHANT SPECIFIC COMPREHENSIVE TESTS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 13.1: Merchant turn start bonus detailed") {
    // Move to merchant's turn
    game.next_turn(); // Spy
    game.next_turn(); // Baron
    game.next_turn(); // General
    game.next_turn(); // Judge
    game.next_turn(); // Merchant
    
    // Merchant with less than 3 coins should not get bonus
    CHECK(merchant->coins() < 3);
    int beforeTurn = merchant->coins();
    merchant->gather();
    CHECK(merchant->coins() == beforeTurn + 1); // Only gather bonus, no turn bonus
    
    // Give merchant 3+ coins and test turn bonus
    CHECK(game.turn() == "Gov");
    gov->gather();
    spy->gather();
    baron->gather();
    general->gather();
    judge->gather();
    
    // Back to merchant with 3+ coins
    merchant->addCoins(3);
    CHECK(merchant->coins() >= 3);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 13.2: Merchant arrest double payment") {
    // Move to merchant's turn and give coins
    game.next_turn(); // Spy
    game.next_turn(); // Baron
    game.next_turn(); // General
    game.next_turn(); // Judge
    game.next_turn(); // Merchant
    
    merchant->addCoins(3);
    int beforeArrest = merchant->coins();
    
    // Merchant does action to advance turn
    merchant->gather();
    
    // Gov arrests merchant
    int govBefore = gov->coins();
    gov->arrest(*merchant);
    
    CHECK(merchant->coins() == beforeArrest - 1); // Standard arrest penalty for merchant
    CHECK(gov->coins() == govBefore); // Gov should not gain coins from merchant arrest
}

// =============================================================================
// TEST 14: BARON SPECIAL ABILITIES COMPREHENSIVE
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 14.1: Baron sanction compensation detailed") {
    // Gov sanctions baron
    gov->addCoins(3);
    int baronBefore = baron->coins();
    gov->sanction(*baron);
    
    // Baron should get immediate compensation of 1 coin
    CHECK(baron->coins() == baronBefore + 1);
}

// =============================================================================
// TEST 15: GENERAL SPECIAL ABILITIES COMPREHENSIVE
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 15.1: General arrest compensation") {
    // Gov arrests general
    general->addCoins(1);
    int generalBefore = general->coins();
    gov->arrest(*general);
    
    // General should not lose coins due to compensation
    CHECK(general->coins() == generalBefore);
}

// =============================================================================
// TEST 16: JUDGE SPECIAL ABILITIES COMPREHENSIVE
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 16.1: Judge sanction extra penalty") {
    // Give gov enough coins to sanction judge
    gov->addCoins(4);
    int govBefore = gov->coins();
    
    gov->sanction(*judge);
    
    // Gov should pay 4 coins total (3 for sanction + 1 penalty to judge)
    CHECK(gov->coins() == govBefore - 4);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 16.2: Judge bribe intervention") {
	Judge* judgePlayer = dynamic_cast<Judge*>(judge);
    REQUIRE(judgePlayer != nullptr);
    
    // Move to spy's turn and give coins for bribe
    game.next_turn();

    // Judge intervenes
	judgePlayer->undo(*spy, true);

    // Verify the flag was set
    CHECK(game.get_judge_intervention() == true);
	// After judge intervenes
	spy->addCoins(4); // Add enough for another bribe
	spy->bribe();
    
    // Next player should be Baron
    CHECK(game.turn() == "Baron");
}

// =============================================================================
// TEST 17: COMPLEX INTERACTION SCENARIOS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 17.1: Multiple interventions in sequence") {
    // Test judge intervention followed by general intervention
    Judge* judgePlayer = dynamic_cast<Judge*>(judge);
    General* generalPlayer = dynamic_cast<General*>(general);
    
    // Move to spy's turn for bribe
    game.next_turn();
    spy->addCoins(4);
    
    // Judge blocks spy's bribe
    judgePlayer->undo(*spy, true);
    spy->bribe();
    
    // Later, move to general's turn for coup intervention
    game.next_turn(); // Baron
    game.next_turn(); // General
    
    general->addCoins(5);
    CHECK(generalPlayer->undo(*gov, *general, true));
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 17.2: Game state consistency after eliminations") {
    // Eliminate spy
    gov->addCoins(7);
    gov->coup(*spy);
    CHECK_FALSE(spy->getActive());
    
    // Check remaining players list
    auto activePlayers = game.players();
    CHECK(activePlayers.size() == 5);
    CHECK(std::find(activePlayers.begin(), activePlayers.end(), "Spy") == activePlayers.end());
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 17.3: Boundary testing with exactly required coins") {
    // Test bribe with exactly 4 coins (gov's turn)
    gov->addCoins(4);
    CHECK(gov->coins() == 4);
    CHECK_NOTHROW(gov->bribe());
    
    // Test sanction with exactly 3 coins (still gov's turn after bribe)
    gov->addCoins(3);
    CHECK(gov->coins() == 3);
    CHECK_NOTHROW(gov->sanction(*spy));
    
    // Test coup with exactly 7 coins (move to baron's turn)
    CHECK(game.turn() == gov->getName());
    baron->addCoins(7);
    CHECK(baron->coins() == 7);
	game.next_turn();
	game.next_turn();
    CHECK_NOTHROW(baron->pay_coup_cost(*general));
}

// =============================================================================
// TEST 18: ERROR MESSAGE VALIDATION
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 18.1: Specific error messages") {
    // Test that specific error conditions produce expected messages
    
    // Wrong turn error
    try {
        spy->gather(); // Gov's turn, spy tries to act
        FAIL("Should have thrown exception");
    } catch (const std::exception& e) {
        std::string error = e.what();
        CHECK(error.find("Not player turn") != std::string::npos);
    }
    
    // Insufficient coins error
    try {
        gov->bribe(); // Gov has 0 coins, needs 4
        FAIL("Should have thrown exception");
    } catch (const std::exception& e) {
        std::string error = e.what();
        CHECK(error.find("Not enough coins") != std::string::npos);
    }
}

// =============================================================================
// TEST 19: GOVERNOR TAX ENHANCEMENT VALIDATION
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 19.1: Governor vs regular player tax amounts") {
    // Governor should get 3, others should get 2
    int govBefore = gov->coins();
    gov->tax();
    CHECK(gov->coins() == govBefore + 3);
    
    // Regular player gets 2
    int spyBefore = spy->coins();
    spy->tax();
    CHECK(spy->coins() == spyBefore + 2);
}

// =============================================================================
// TEST 20: COMPREHENSIVE GAME END CONDITIONS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 20.1: Game end with single survivor") {
    // Systematically eliminate all but one player
    std::vector<Player*> targets = {spy, baron, general, judge, merchant};
    
    for (Player* target : targets) {
        gov->addCoins(7);
        gov->coup(*target);
        CHECK_FALSE(target->getActive());
        // Advance to gov's turn if not already there
        while (game.turn() != "Gov") {
            game.next_turn();
        }
    }
    
    // Only gov should remain
    CHECK(game.winner() == "Gov");
    CHECK(game.players().size() == 1);
}

// =============================================================================
// TEST 21: GAME VALIDATION - PLAYER COUNT RESTRICTIONS
// =============================================================================

TEST_CASE("Test 21.1: Game with 0 players cannot start") {
    Game emptyGame;
    // Should throw when trying to start with no players
    CHECK_THROWS_AS(emptyGame.start_game(), std::exception);
}

TEST_CASE("Test 21.2: Game with 1 player cannot start") {
    Game singleGame;
    PlayerFactory::create_player(singleGame, "Solo", Role::GOVERNOR);
    // Should throw when trying to start with only 1 player (minimum is 2)
    CHECK_THROWS_AS(singleGame.start_game(), std::exception);
}

TEST_CASE("Test 21.3: Game with more than 6 players cannot start") {
    Game overloadGame;
    
    // Add 6 players (maximum allowed)
    PlayerFactory::create_player(overloadGame, "P1", Role::GOVERNOR);
    PlayerFactory::create_player(overloadGame, "P2", Role::SPY);
    PlayerFactory::create_player(overloadGame, "P3", Role::BARON);
    PlayerFactory::create_player(overloadGame, "P4", Role::GENERAL);
    PlayerFactory::create_player(overloadGame, "P5", Role::JUDGE);
    PlayerFactory::create_player(overloadGame, "P6", Role::MERCHANT);
    
    // Try to add 7th player - should throw
    CHECK_THROWS_AS(PlayerFactory::create_player(overloadGame, "P7", Role::GOVERNOR), std::exception);
}

TEST_CASE("Test 21.4: Game with exactly 2 players can start") {
    Game minGame;
    PlayerFactory::create_player(minGame, "P1", Role::GOVERNOR);
    PlayerFactory::create_player(minGame, "P2", Role::SPY);
    
    // Should work with exactly 2 players
    CHECK_NOTHROW(minGame.start_game());
    CHECK(minGame.turn() == "P1"); // First registered player goes first
}

TEST_CASE("Test 21.5: Game with exactly 6 players can start") {
    Game maxGame;
    PlayerFactory::create_player(maxGame, "P1", Role::GOVERNOR);
    PlayerFactory::create_player(maxGame, "P2", Role::SPY);
    PlayerFactory::create_player(maxGame, "P3", Role::BARON);
    PlayerFactory::create_player(maxGame, "P4", Role::GENERAL);
    PlayerFactory::create_player(maxGame, "P5", Role::JUDGE);
    PlayerFactory::create_player(maxGame, "P6", Role::MERCHANT);
    
    // Should work with exactly 6 players
    CHECK_NOTHROW(maxGame.start_game());
    CHECK(maxGame.turn() == "P1"); // First registered player goes first
}

// =============================================================================
// TEST 22: CORRECTED TURN ORDER AND GAME MECHANICS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 22.1: Correct turn order - actions advance turns automatically") {
    // Initial turn should be Gov (first registered player)
    CHECK(game.turn() == "Gov");
    
    // Gov does action - turn advances to Spy (second registered)
    gov->gather();
    CHECK(game.turn() == "Spy");
    
    // Spy does action - turn advances to Baron (third registered)
    spy->gather();
    CHECK(game.turn() == "Baron");
    
    // Baron does action - turn advances to General (fourth registered)
    baron->gather();
    CHECK(game.turn() == "General");
    
    // General does action - turn advances to Judge (fifth registered)
    general->gather();
    CHECK(game.turn() == "Judge");
    
    // Judge does action - turn advances to Merchant (sixth registered)
    judge->gather();
    CHECK(game.turn() == "Merchant");
    
    // Merchant does action - turn cycles back to Gov (first player)
    merchant->gather();
    CHECK(game.turn() == "Gov");
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 22.2: Bribe gives extra turn - no turn advancement") {
    // Gov's turn - give enough coins for bribe
    gov->addCoins(4);
    CHECK(game.turn() == "Gov");
    
    // Gov bribes - should NOT advance turn (extra turn)
    gov->bribe();
    CHECK(game.turn() == "Gov"); // Still Gov's turn
    
    // Gov does another action - NOW turn advances
    gov->gather();
    CHECK(game.turn() == "Gov");
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 22.3: Turn advancement with elimination") {
    // Gov's turn - eliminate Spy
    gov->addCoins(7);
    CHECK(game.turn() == "Gov");
    gov->coup(*spy);
    CHECK_FALSE(spy->getActive());
    
    // Turn should advance to Baron (skipping eliminated Spy)
    CHECK(game.turn() == "Baron");
    
    // Continue turn order: Baron -> General -> Judge -> Merchant -> Gov
    baron->gather();
    CHECK(game.turn() == "General");
    
    general->gather();
    CHECK(game.turn() == "Judge");
    
    judge->gather();
    CHECK(game.turn() == "Merchant");
    
    merchant->gather();
    CHECK(game.turn() == "Gov"); // Cycles back, skipping eliminated Spy
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 22.4: Blocking actions don't advance turn except block tax") {
    // Move to spy's turn to test spy blocking
    gov->gather(); // Gov -> Spy
    CHECK(game.turn() == "Spy");
    
    Spy* spyPlayer = dynamic_cast<Spy*>(spy);
    REQUIRE(spyPlayer != nullptr);
    
    // Spy blocks arrest - should NOT advance turn
    spyPlayer->disable_arrest(*baron);
    CHECK(game.turn() == "Spy"); // Still Spy's turn
    
    // Spy does regular action to advance
    spy->gather(); // Spy -> Baron
    
    // Test Governor blocking doesn't advance turn
    baron->gather(); // Baron -> General  
    general->gather(); // General -> Judge
    judge->gather(); // Judge -> Merchant
    merchant->gather(); // Merchant -> Gov
    
    Governor* govPlayer = dynamic_cast<Governor*>(gov);
    REQUIRE(govPlayer != nullptr);
    
    govPlayer->block_tax(*spy);
    CHECK(game.turn() == spy->getName()); // spy's turn after blocking
}

// =============================================================================
// TEST 23: ROLE-SPECIFIC ABILITIES WITH CORRECT TURN MANAGEMENT
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 23.1: Spy abilities on correct turn") {
    // Move to Spy's turn
    gov->gather(); // Gov -> Spy
    CHECK(game.turn() == "Spy");
    
    Spy* spyPlayer = dynamic_cast<Spy*>(spy);
    REQUIRE(spyPlayer != nullptr);
    
    // Give baron some coins to view
    baron->addCoins(5);
    
    // Spy views coins - this doesn't advance turn
    CHECK_NOTHROW(spyPlayer->view_coins(*baron));
    CHECK(game.turn() == "Spy"); // Still Spy's turn
    
    // Spy does regular action - now turn advances
    spy->gather();
    CHECK(game.turn() == "Baron");
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 23.2: Baron invest on correct turn") {
    // Move to Baron's turn: Gov -> Spy -> Baron
    gov->gather();
    spy->gather();
    CHECK(game.turn() == "Baron");
    
    Baron* baronPlayer = dynamic_cast<Baron*>(baron);
    REQUIRE(baronPlayer != nullptr);
    
    // Give baron exactly 3 coins for invest
    baron->addCoins(3);
    int beforeInvest = baron->coins();
    
    baronPlayer->invest();
    // Should spend 3 and gain 6, net gain of 3
    CHECK(baron->coins() == beforeInvest + 3);
    
    // Turn should advance after invest
    CHECK(game.turn() == "General");
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 23.3: General abilities on correct turn") {
    // Move to General's turn: Gov -> Spy -> Baron -> General
    gov->gather();
    spy->gather();
    baron->gather();
    CHECK(game.turn() == "General");
    
    General* generalPlayer = dynamic_cast<General*>(general);
    REQUIRE(generalPlayer != nullptr);
    
    // Give general 5 coins for blocking ability
    general->addCoins(5);
    
    // General blocks coup - this is intervention, doesn't advance turn
    bool blocked = generalPlayer->undo(*gov, *spy, true);
    CHECK(blocked);
    CHECK(game.get_general_intervention());
    CHECK(game.turn() == "General"); // Still General's turn
    
    // General does regular action - now turn advances
    general->gather();
    CHECK(game.turn() == "Judge");
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 23.4: Judge abilities on correct turn") {
    // Move to Judge's turn: Gov -> Spy -> Baron -> General -> Judge
    gov->gather();
    spy->gather();
    baron->gather();
    general->gather();
    CHECK(game.turn() == "Judge");
    
    Judge* judgePlayer = dynamic_cast<Judge*>(judge);
    REQUIRE(judgePlayer != nullptr);
    
    // Judge blocks bribe - this is intervention, doesn't advance turn
    bool blocked = judgePlayer->undo(*spy, true);
    CHECK(blocked);
    CHECK(game.get_judge_intervention());
    CHECK(game.turn() == "Judge"); // Still Judge's turn
    
    // Judge does regular action - now turn advances
    judge->gather();
    CHECK(game.turn() == "Merchant");
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 23.5: Merchant abilities on correct turn") {
    // Move to Merchant's turn: Gov -> Spy -> Baron -> General -> Judge -> Merchant
    gov->gather();
    spy->gather();
    baron->gather();
    general->gather();
    judge->gather();
    CHECK(game.turn() == "Merchant");
    
    // Give merchant 3+ coins to test auto-bonus
    merchant->addCoins(3);    
    // Merchant does action - turn advances and cycle completes
    merchant->gather();
    CHECK(game.turn() == "Gov"); // Back to Gov
    
    // Test merchant arrest double payment in a new round
    gov->gather(); // Gov -> Spy
    spy->gather(); // Spy -> Baron
    baron->gather(); // Baron -> General
    general->gather(); // General -> Judge
    judge->gather(); // Judge -> Merchant
    
    // Back to merchant's turn
    CHECK(game.turn() == "Merchant");
    
    // Now test arrest on merchant
    merchant->gather(); // Merchant -> Gov
	int beforeArrest = merchant->coins();
    int govBefore = gov->coins();
    gov->arrest(*merchant);
    
    // Merchant should lose 2 coins instead of 1
    CHECK(merchant->coins() == beforeArrest - 2);
    // Gov should not gain coins from merchant arrest (goes to bank)
    CHECK(gov->coins() == govBefore);
}

// =============================================================================
// TEST 24: 10+ COINS RESTRICTION WITH CORRECT TURNS
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 24.1: Player with 10+ coins can only coup") {
    // Gov's turn - give 10+ coins
    gov->addCoins(10);
    CHECK(game.turn() == "Gov");
    
    // Should not be able to do anything except coup
    CHECK_THROWS_AS(gov->gather(), std::exception);
    CHECK_THROWS_AS(gov->tax(), std::exception);
    CHECK_THROWS_AS(gov->bribe(), std::exception);
    CHECK_THROWS_AS(gov->arrest(*spy), std::exception);
    CHECK_THROWS_AS(gov->sanction(*spy), std::exception);
    
    // Should be able to coup
    CHECK_NOTHROW(gov->coup(*spy));
    CHECK_FALSE(spy->getActive());
    
    // Turn should advance after coup
    CHECK(game.turn() == "Baron"); // Skips eliminated Spy
}

// =============================================================================
// TEST 25: COMPLEX SCENARIOS WITH CORRECT TURN FLOW
// =============================================================================

TEST_CASE_FIXTURE(CoupTestFixture, "Test 25.1: Complete game round with all players") {
    // Round 1: All players gather
    CHECK(game.turn() == "Gov");
    gov->gather(); // Gov: 1 coin, turn -> Spy
    
    CHECK(game.turn() == "Spy");
    spy->gather(); // Spy: 1 coin, turn -> Baron
    
    CHECK(game.turn() == "Baron");
    baron->gather(); // Baron: 1 coin, turn -> General
    
    CHECK(game.turn() == "General");
    general->gather(); // General: 1 coin, turn -> Judge
    
    CHECK(game.turn() == "Judge");
    judge->gather(); // Judge: 1 coin, turn -> Merchant
    
    CHECK(game.turn() == "Merchant");
    merchant->gather(); // Merchant: 1 coin, turn -> Gov (cycle complete)
    
    CHECK(game.turn() == "Gov");
    
    // Verify all players have 1 coin
    CHECK(gov->coins() == 1);
    CHECK(spy->coins() == 1);
    CHECK(baron->coins() == 1);
    CHECK(general->coins() == 1);
    CHECK(judge->coins() == 1);
    CHECK(merchant->coins() == 1);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 25.2: Bribe chain with correct turns") {
    // Give multiple players coins for bribe
    gov->addCoins(4);
    spy->addCoins(4);
    baron->addCoins(4);
    
    // Gov bribes - stays Gov's turn
    CHECK(game.turn() == "Gov");
    gov->bribe();
    CHECK(game.turn() == "Gov");
    // Gov does another action - turn advances
    gov->gather();
	gov->gather();
    CHECK(game.turn() == "Spy");
    
    // Spy bribes - stays Spy's turn
    spy->bribe();
    CHECK(game.turn() == "Spy");
    
    // Spy does another action - turn advances
    spy->gather();
	spy->gather();
    CHECK(game.turn() == "Baron");
    
    // Baron bribes - stays Baron's turn
    baron->bribe();
    CHECK(game.turn() == "Baron");
    
    // Baron does another action - turn advances
    baron->gather();
	baron->gather();

    CHECK(game.turn() == "General");
}

TEST_CASE_FIXTURE(CoupTestFixture, "Test 25.3: Elimination and turn skipping") {
    // Eliminate players in sequence and verify turn order
    
    // Gov eliminates Spy
    gov->addCoins(7);
    gov->coup(*spy);
    CHECK_FALSE(spy->getActive());
    CHECK(game.turn() == "Baron"); // Skips eliminated Spy
    
    // Baron eliminates General
    baron->addCoins(7);
    baron->coup(*general);
    CHECK_FALSE(general->getActive());
    CHECK(game.turn() == "Judge"); // Skips eliminated General
    
    // Judge eliminates Merchant
    judge->addCoins(7);
    judge->coup(*merchant);
    CHECK_FALSE(merchant->getActive());
    CHECK(game.turn() == "Gov"); // Cycles back, skipping eliminated players
    
    // Verify only Gov, Baron, Judge remain
    auto activePlayers = game.players();
    CHECK(activePlayers.size() == 3);
    
    // Continue turn order: Gov -> Baron -> Judge -> Gov
    gov->gather();
    CHECK(game.turn() == "Baron");
    
    baron->gather();
    CHECK(game.turn() == "Judge");
    
    judge->gather();
    CHECK(game.turn() == "Gov");
}
