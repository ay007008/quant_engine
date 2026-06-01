#include <gtest/gtest.h>
#include "../include/order_book.h"

TEST(OrderBookTest, BestBidAsk) {
    OrderBook book;
    book.add_order({"bid1", 100.50, 10, true});
    book.add_order({"bid2", 100.75, 5,  true});
    book.add_order({"bid3", 100.25, 8,  true});
    book.add_order({"ask1", 101.00, 10, false});
    book.add_order({"ask2", 100.90, 5,  false});
    book.add_order({"ask3", 101.25, 8,  false});
    EXPECT_DOUBLE_EQ(book.best_bid(), 100.75);
    EXPECT_DOUBLE_EQ(book.best_ask(), 100.90);
    EXPECT_NEAR(book.spread(), 0.15, 0.0001);
}

TEST(OrderBookTest, CancelOrder) {
    OrderBook book;
    book.add_order({"bid1", 100.75, 5, true});
    book.add_order({"bid2", 100.50, 10, true});
    EXPECT_DOUBLE_EQ(book.best_bid(), 100.75);
    book.cancel_order("bid1");
    EXPECT_DOUBLE_EQ(book.best_bid(), 100.50);
}

TEST(OrderBookTest, CancelNonexistent) {
    OrderBook book;
    EXPECT_THROW(book.cancel_order("fake_id"), std::runtime_error);
}

TEST(OrderBookTest, EmptyBookThrows) {
    OrderBook book;
    EXPECT_THROW(book.best_bid(), std::runtime_error);
    EXPECT_THROW(book.best_ask(), std::runtime_error);
}

TEST(OrderBookTest, QuantityAggregation) {
    OrderBook book;
    book.add_order({"bid1", 100.50, 10, true});
    book.add_order({"bid2", 100.50, 5,  true});
    EXPECT_EQ(book.best_bid_quantity(), 15);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
