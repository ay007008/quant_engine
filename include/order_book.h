#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <mutex>
#include <shared_mutex>

struct Order {
    std::string order_id;
    double      price;
    int         quantity;
    bool        is_bid;
};

class OrderBook {
public:
    void add_order(const Order& order);
    void cancel_order(const std::string& order_id);
    void cancel_order_quantity(double price, int quantity, bool is_bid);
    double best_bid() const;
    double best_ask() const;
    double spread() const;
    int best_bid_quantity() const;
    int best_ask_quantity() const;

private:
    std::map<double, int, std::greater<double>> bids_;
    std::map<double, int> asks_;
    std::unordered_map<std::string, Order> orders_;
    mutable std::shared_mutex book_mutex_;
};
