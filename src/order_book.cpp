#include "../include/order_book.h"
#include <mutex>
#include <shared_mutex>

void OrderBook::add_order(const Order& order) {
    std::unique_lock<std::shared_mutex> lock(book_mutex_);
    orders_[order.order_id] = order;
    if (order.is_bid) {
        bids_[order.price] += order.quantity;
    } else {
        asks_[order.price] += order.quantity;
    }
}

void OrderBook::cancel_order(const std::string& order_id) {
    std::unique_lock<std::shared_mutex> lock(book_mutex_);
    auto it = orders_.find(order_id);
    if (it == orders_.end()) {
        throw std::runtime_error("Order not found: " + order_id);
    }
    const Order& order = it->second;
    if (order.is_bid) {
        bids_[order.price] -= order.quantity;
        if (bids_[order.price] <= 0) {
            bids_.erase(order.price);
        }
    } else {
        asks_[order.price] -= order.quantity;
        if (asks_[order.price] <= 0) {
            asks_.erase(order.price);
        }
    }
    orders_.erase(it);
}

double OrderBook::best_bid() const {
    std::shared_lock<std::shared_mutex> lock(book_mutex_);
    if (bids_.empty()) {
        throw std::runtime_error("No bids in book");
    }
    return bids_.begin()->first;
}

double OrderBook::best_ask() const {
    std::shared_lock<std::shared_mutex> lock(book_mutex_);
    if (asks_.empty()) {
        throw std::runtime_error("No asks in book");
    }
    return asks_.begin()->first;
}

double OrderBook::spread() const {
    std::shared_lock<std::shared_mutex> lock(book_mutex_);
    if (bids_.empty() || asks_.empty()) {
        throw std::runtime_error("Cannot compute spread on empty book");
    }
    return asks_.begin()->first - bids_.begin()->first;
}

int OrderBook::best_bid_quantity() const {
    std::shared_lock<std::shared_mutex> lock(book_mutex_);
    if (bids_.empty()) return 0;
    return bids_.begin()->second;
}

int OrderBook::best_ask_quantity() const {
    std::shared_lock<std::shared_mutex> lock(book_mutex_);
    if (asks_.empty()) return 0;
    return asks_.begin()->second;
}

void OrderBook::cancel_order_quantity(double price, int quantity, bool is_bid) {
    std::unique_lock<std::shared_mutex> lock(book_mutex_);
    if (is_bid) {
        bids_[price] -= quantity;
        if (bids_[price] <= 0) bids_.erase(price);
    } else {
        asks_[price] -= quantity;
        if (asks_[price] <= 0) asks_.erase(price);
    }
}
