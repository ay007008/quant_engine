#include "../include/matching_engine.h"

std::vector<Trade> MatchingEngine::submit_order(const Order& order) {
    std::vector<Trade> trades = match(order);
    trade_log_.insert(trade_log_.end(), trades.begin(), trades.end());
    return trades;
}

std::vector<Trade> MatchingEngine::match(const Order& order) {
    std::vector<Trade> trades;
    Order remaining = order;

    if (order.is_bid) {
        // Incoming buy — match against asks
        while (remaining.quantity > 0) {
            try {
                double best_ask = book_.best_ask();
                // Buy order crosses if its price >= best ask
                if (remaining.price < best_ask) break;

                int ask_qty = book_.best_ask_quantity();
                int fill_qty = std::min(remaining.quantity, ask_qty);

                // Find the resting ask order ID at best ask
                // For simplicity create a synthetic ID
                std::string ask_id = "ask_at_" + std::to_string(best_ask);

                trades.push_back({
                    remaining.order_id,
                    ask_id,
                    best_ask,
                    fill_qty
                });

                // Reduce quantities
                remaining.quantity -= fill_qty;

                // Remove filled quantity from book
                Order filled_ask{"filled", best_ask, fill_qty, false};
                book_.cancel_order_quantity(best_ask, fill_qty, false);

            } catch (const std::runtime_error&) {
                break; // No more asks
            }
        }
    } else {
        // Incoming sell — match against bids
        while (remaining.quantity > 0) {
            try {
                double best_bid = book_.best_bid();
                // Sell order crosses if its price <= best bid
                if (remaining.price > best_bid) break;

                int bid_qty = book_.best_bid_quantity();
                int fill_qty = std::min(remaining.quantity, bid_qty);

                std::string bid_id = "bid_at_" + std::to_string(best_bid);

                trades.push_back({
                    bid_id,
                    remaining.order_id,
                    best_bid,
                    fill_qty
                });

                remaining.quantity -= fill_qty;
                book_.cancel_order_quantity(best_bid, fill_qty, true);

            } catch (const std::runtime_error&) {
                break; // No more bids
            }
        }
    }

    // If order not fully filled, rest remainder in book
    if (remaining.quantity > 0) {
        book_.add_order(remaining);
    }

    return trades;
}
