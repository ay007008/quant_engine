# Quant Engine — C++ Trading Systems

A production-grade C++ quantitative trading engine built from first principles.
Currently implements a thread-safe limit order book with a matching engine in progress.
Part of a broader market making system validated against live prediction market data.

## What's built

### Limit Order Book
- Two-sided price level aggregation (bids descending, asks ascending)
- O(log n) order add and cancel
- O(1) best bid and ask lookup
- Reader-writer thread safety via std::shared_mutex
- Quantity aggregation at price levels
- Full Google Test suite — 5/5 passing

## Architecture decisions

**Why std::shared_mutex over std::mutex:**
The order book is read-heavy — the pricing thread checks best bid/ask on every tick
while the execution thread writes orders occasionally. A shared mutex allows multiple
simultaneous readers with exclusive writer access, eliminating unnecessary blocking
on the hot read path.

**Why std::map over unordered_map for price levels:**
Price levels must be sorted — best bid is always the maximum, best ask always the
minimum. std::map (red-black tree) maintains sorted order automatically with O(log n)
operations. An unordered_map would require O(n) scan to find best prices.

**Why separate orders_ unordered_map:**
Cancel requests arrive by order ID, not by price. Without a separate ID-to-order index,
cancellation requires scanning every price level — O(n). The unordered_map provides
O(1) order lookup by ID at the cost of storing each order twice.

## Building

```bash
mkdir build && cd build
cmake ..
make
./test_order_book
```

## Testing
[==========] 5 tests from 1 test suite ran.
[  PASSED  ] 5 tests passed.
Tests cover: best bid/ask correctness, order cancellation, empty book error handling,
quantity aggregation at same price level, and nonexistent order cancellation.

## Roadmap

- [ ] Matching engine — price-time priority execution
- [ ] Gamma Capture volatility model — Poisson barrier crossing pricer
- [ ] Avellaneda-Stoikov optimal market maker
- [ ] Three-threaded C++ market maker (feed / pricing / execution)
- [ ] AWS EC2 deployment with CI/CD pipeline
- [ ] Heston stochastic vol calibration

## Related work

- Live binary options market maker running on Bayse Markets (Python)
- C++ European options pricer with original Maclaurin series GBM derivation
- Gamma Capture model validation research (in progress, co-authored with Louis Pellathy)

Substack: open.substack.com/pub/ayomideakinola007
