#pragma once

#include "order_book.h"
#include <vector>

struct Trade {
    std::string buy_order_id;
    std::string sell_order_id;
    double      price;
    int         quantity;
};

class MatchingEngine {
public:
    // Submit an order — may execute immediately or rest in book
    std::vector<Trade> submit_order(const Order& order);

    // Access the underlying book
    const OrderBook& book() const { return book_; }

    // Get all completed trades
    const std::vector<Trade>& trade_log() const { return trade_log_; }

private:
    OrderBook          book_;
    std::vector<Trade> trade_log_;

    // Try to match an incoming order against resting orders
    std::vector<Trade> match(const Order& order);
};
