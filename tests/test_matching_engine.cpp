#include <gtest/gtest.h>
#include "../include/matching_engine.h"

// Test 1 — buy order crosses spread and executes immediately
TEST(MatchingEngineTest, BuyOrderExecutes) {
    MatchingEngine engine;

    // Add resting ask at 101.00
    engine.submit_order({"ask1", 101.00, 10, false});

    // Submit buy at 101.00 — should execute immediately
    auto trades = engine.submit_order({"bid1", 101.00, 5, true});

    EXPECT_EQ(trades.size(), 1);
    EXPECT_DOUBLE_EQ(trades[0].price, 101.00);
    EXPECT_EQ(trades[0].quantity, 5);
}

// Test 2 — sell order crosses spread and executes
TEST(MatchingEngineTest, SellOrderExecutes) {
    MatchingEngine engine;

    // Add resting bid at 100.75
    engine.submit_order({"bid1", 100.75, 10, true});

    // Submit sell at 100.60 — crosses spread, should execute at 100.75
    auto trades = engine.submit_order({"ask1", 100.60, 5, false});

    EXPECT_EQ(trades.size(), 1);
    EXPECT_DOUBLE_EQ(trades[0].price, 100.75);
    EXPECT_EQ(trades[0].quantity, 5);
}

// Test 3 — order that doesn't cross rests in book
TEST(MatchingEngineTest, OrderRestsInBook) {
    MatchingEngine engine;

    // Add resting ask at 101.00
    engine.submit_order({"ask1", 101.00, 10, false});

    // Submit buy at 100.50 — doesn't cross, should rest
    auto trades = engine.submit_order({"bid1", 100.50, 5, true});

    EXPECT_EQ(trades.size(), 0);
    EXPECT_DOUBLE_EQ(engine.book().best_bid(), 100.50);
}

// Test 4 — partial fill leaves remainder in book
TEST(MatchingEngineTest, PartialFill) {
    MatchingEngine engine;

    // Resting ask with 10 units
    engine.submit_order({"ask1", 101.00, 10, false});

    // Buy 3 units — partial fill, 7 should remain at ask
    auto trades = engine.submit_order({"bid1", 101.00, 3, true});

    EXPECT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].quantity, 3);
    EXPECT_EQ(engine.book().best_ask_quantity(), 7);
}

// Test 5 — trade log accumulates correctly
TEST(MatchingEngineTest, TradeLogAccumulates) {
    MatchingEngine engine;

    engine.submit_order({"ask1", 101.00, 10, false});
    engine.submit_order({"bid1", 101.00, 3, true});
    engine.submit_order({"bid2", 101.00, 4, true});

    EXPECT_EQ(engine.trade_log().size(), 2);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
