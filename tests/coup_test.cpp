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

using namespace coup;

struct CoupTestFixture {
    Game game;
    Governor gov;
    Spy spy;
    Baron bar;
    General gen;
    Judge jud;
    Merchant mer;

    CoupTestFixture()
        : game(), gov(game, "Gov"), spy(game, "Spy"), bar(game, "Bar"),
          gen(game, "Gen"), jud(game, "Jud"), mer(game, "Mer") {}
};

// A. General Game Mechanics
TEST_CASE("Initialize game & turn order") {
    Game g;
    Player A(g, "A");
    Player B(g, "B");
    Player C(g, "C");

    CHECK(g.turn() == "A");
    // advance turn by performing a gather
    A.gather();
    CHECK(g.turn() == "B");
}

TEST_CASE("Active players & winner at end") {
    Game g;
    Player A(g, "A");
    Player B(g, "B");
    // force coup
    // give A enough coins
    g.add_coins(7);
    for(int i=0; i<7; ++i) { if(g.turn()=="A") A.gather(); else g.turn(); }
    A.coup(B);
    auto active = g.players();
    CHECK(active.size() == 1);
    CHECK(active[0] == "A");
    CHECK(g.winner() == "A");
}

TEST_CASE("Exception when calling winner() too early") {
    Game g;
    Player A(g, "A");
    Player B(g, "B");
    Player C(g, "C");
    CHECK_THROWS_AS(g.winner(), std::exception);
}

// B. Basic Actions (role-independent)
TEST_CASE_FIXTURE(CoupTestFixture, "Gather resource") {
    // initial bank = 50 by default
    CHECK(game.turn() == "Gov");
    int before = gov.coins();
    gov.gather();
    CHECK(gov.coins() == before + 1);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Standard tax") {
    // use spy as generic actor
    // advance to Spy
    gov.gather(); spy.gather();
    int beforeSp = spy.coins();
    spy.tax();
    CHECK(spy.coins() == beforeSp + 2);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Legal bribe") {
    // advance to Baron and give bar enough coins
    while(game.turn() != "Bar") game.next_turn();
    game.add_coins(4);
    bar.gather(); bar.gather(); bar.gather(); bar.gather();
    int before = bar.coins();
    bar.bribe();
    CHECK(bar.coins() == before - 4);
    CHECK(game.turn() == "Bar");
}

TEST_CASE_FIXTURE(CoupTestFixture, "Illegal bribe (insufficient coins)") {
    while(game.turn() != "Bar") game.next_turn();
    // bar has <4
    CHECK_THROWS_AS(bar.bribe(), std::exception);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Legal arrest") {
    // let Governor arrest spy
    while(game.turn() != "Gov") game.next_turn();
    int beforeA = gov.coins();
    int beforeB = spy.coins();
    gov.arrest(spy);
    CHECK(gov.coins() == beforeA + 1);
    CHECK(spy.getActive() == false);
}

TEST_CASE_FIXTURE(CoupTestFixture, "Consecutive arrest on same target") {
    while(game.turn() != "Gov") game.next_turn();
    gov.arrest(spy);
    // skip to Gov again
    while(game.turn() != "Gov") game.next_turn();
    CHECK_THROWS_AS(gov.arrest(spy), std::exception);
}

// TEST_CASE_FIXTURE(CoupTestFixture, "Legal sanction") {
//     // General uses sanction on Baron
//     while(game.turn() != "Gen") game.next_turn();
//     game.add_coins(3);
//     gen.sanction(bar);
//     CHECK(bar.is_blocked());
//     // bar cannot gather next turn
//     while(game.turn() != "Bar") game.next_turn();
//     CHECK_THROWS_AS(bar.gather(), std::exception);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Illegal sanction (insufficient coins)") {
//     while(game.turn() != "Gen") game.next_turn();
//     // gen has <3
//     CHECK_THROWS_AS(gen.sanction(bar), std::exception);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Legal coup") {
//     // give Spy 7 coins
//     while(game.turn() != "Spy") game.next_turn();
//     game.add_coins(7);
//     for(int i=0; i<7; ++i) spy.gather();
//     int before = spy.coins();
//     spy.coup(bar);
//     CHECK(spy.coins() == before - 7);
//     CHECK_FALSE(bar.is_active());
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Illegal coup (insufficient coins)") {
//     while(game.turn() != "Spy") game.next_turn();
//     // spy has <7
//     CHECK_THROWS_AS(spy.coup(bar), std::exception);
// }

// // C. Role-Specific Abilities
// TEST_CASE_FIXTURE(CoupTestFixture, "Governor’s enhanced tax") {
//     while(game.turn() != "Gov") game.next_turn();
//     int b4 = gov.coins();
//     gov.tax();
//     CHECK(gov.coins() == b4 + 3);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Governor blocks another’s tax") {
//     // spy taxes
//     while(game.turn() != "Spy") game.next_turn();
//     spy.gather(); spy.gather();
//     spy.tax();
//     // gov blocks
//     while(game.turn() != "Gov") game.next_turn();
//     int before = spy.coins();
//     gov.block_tax(spy);
//     CHECK(spy.coins() == before - 2);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Spy views another’s coins") {
//     while(game.turn() != "Spy") game.next_turn();
//     int c = bar.coins();
//     CHECK(spy.view_coins(bar) == c);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Spy blocks next arrest") {
//     // bar arrests gen
//     while(game.turn() != "Bar") game.next_turn();
//     bar.gather(); bar.gather();
//     bar.arrest(gen);
//     // spy blocks
//     while(game.turn() != "Spy") game.next_turn();
//     spy.block_arrest(bar);
//     // bar turn next
//     while(game.turn() != "Bar") game.next_turn();
//     CHECK_THROWS_AS(bar.arrest(gen), std::exception);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Baron invests legally") {
//     while(game.turn() != "Bar") game.next_turn();
//     game.add_coins(3);
//     int b4 = bar.coins();
//     bar.invest();
//     CHECK(bar.coins() == b4 + 3);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Baron compensation after sanction") {
//     // gen sanctions bar
//     while(game.turn() != "Gen") game.next_turn();
//     game.add_coins(3);
//     gen.sanction(bar);
//     // bar turn
//     while(game.turn() != "Bar") game.next_turn();
//     int b4 = bar.coins();
//     bar.gather();
//     CHECK(bar.coins() == b4 + 2);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "General prevents a coup") {
//     while(game.turn() != "Gen") game.next_turn();
//     game.add_coins(5);
//     gen.gather(); gen.gather(); gen.gather(); gen.gather(); gen.gather();
//     gen.undo();
//     // bar tries to coup gen
//     while(game.turn() != "Bar") game.next_turn();
//     game.add_coins(7);
//     for(int i=0;i<7;++i) bar.gather();
//     bar.coup(gen);
//     CHECK(gen.is_active());
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "General compensation after arrest") {
//     // bar arrests gen
//     while(game.turn() != "Bar") game.next_turn();
//     bar.gather();
//     bar.arrest(gen);
//     // gen turn start
//     while(game.turn() != "Gen") game.next_turn();
//     CHECK(gen.coins() == 0);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Judge blocks a bribe") {
//     while(game.turn() != "Bar") game.next_turn();
//     game.add_coins(4);
//     bar.gather();bar.gather();bar.gather();bar.gather();
//     bar.bribe();
//     while(game.turn() != "Jud") game.next_turn();
//     jud.undo(bar);
//     CHECK(bar.coins() == 0);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Judge penalty on sanction") {
//     // gen sanctions jud
//     while(game.turn() != "Gen") game.next_turn();
//     game.add_coins(3);
//     gen.sanction(jud);
//     while(game.turn() != "Jud") game.next_turn();
//     int bankBefore = game.bank_coins();
//     // judge turn start penalty
//     CHECK(game.bank_coins() == bankBefore + 1);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Merchant bonus at turn start") {
//     while(game.turn() != "Mer") game.next_turn();
//     mer.gather(); mer.gather(); mer.gather();
//     int b4 = mer.coins();
//     // start next mer turn
//     while(game.turn() != "Mer") game.next_turn();
//     CHECK(mer.coins() == b4 + 1);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Merchant pays double after arrest") {
//     while(game.turn() != "Mer") game.next_turn();
//     mer.gather(); mer.gather();
//     // bar arrests Mer
//     while(game.turn() != "Bar") game.next_turn();
//     int bankBefore = game.bank_coins();
//     bar.arrest(mer);
//     CHECK(game.bank_coins() == bankBefore + 2);
// }

// // D. Exceptions & Edge Cases
// TEST_CASE_FIXTURE(CoupTestFixture, "Forced coup at 10+ coins") {
//     while(game.turn() != "Gov") game.next_turn();
//     game.add_coins(10);
//     for(int i=0;i<10;++i) gov.gather();
//     CHECK_THROWS_AS(gov.gather(), std::exception);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Wrong player acts out of turn") {
//     // it's Gov's turn
//     CHECK_THROWS_AS(spy.gather(), std::exception);
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Real-time block before next turn") {
//     // bar bribes
//     while(game.turn() != "Bar") game.next_turn();
//     game.add_coins(4);
//     bar.gather();bar.gather();bar.gather();bar.gather();
//     bar.bribe();
//     // judge blocks immediately
//     while(game.turn() != "Jud") game.next_turn();
//     jud.undo(bar);
//     CHECK(bar.coins() == 0);
//     CHECK(game.turn() == "Jud");
// }

// TEST_CASE_FIXTURE(CoupTestFixture, "Two players share same role—no collision") {
//     Game g;
//     Governor g1(g, "X");
//     Governor g2(g, "Y");
//     while(g.turn() != "X") g.next_turn();
//     g1.tax();
//     while(g.turn() != "Y") g.next_turn();
//     g2.tax();
//     CHECK(g1.coins() == 3);
//     CHECK(g2.coins() == 3);
// }
