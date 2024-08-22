#pragma once

#include <string>
#include <cstdint>
#include <limits>

typedef double Price;
typedef uint64_t Quantity;
typedef uint32_t OrderID;
typedef uint32_t UserID;
typedef uint64_t ID;
typedef std::string Ticker;

const Price MARKET_ORDER_BID_PRICE = std::numeric_limits<Price>::max();
const Price MARKET_ORDER_ASK_PRICE = 0;
const Price NO_STOP_PRICE = std::numeric_limits<Price>::min();

enum Side {
    BID,
    ASK
};

enum OrderType {
    MARKET,
    LIMIT,
    STOP,
    STOP_LIMIT
};

enum OrderCondition {
    NONE = 0,
    AON = 1,
    IOC = AON << 1,
    FOK = AON | IOC
};

struct Order {
    Ticker ticker;
    ID id;
    Price price;
    Price stop = NO_STOP_PRICE;
    Quantity quantity;
    Side side;
    OrderType orderType = MARKET;
    OrderCondition condtion = NONE;

    Order(): ticker(""), id(0), quantity(0), side(BID) {};
    Order(Ticker tk, ID id, Quantity q, Side s):
        ticker(tk),
        id(id),
        price(s ? MARKET_ORDER_ASK_PRICE : MARKET_ORDER_BID_PRICE),
        quantity(q),
        side(s)
        {};

    Order(Ticker tk, ID id, Price p, Price st, Quantity q, Side s, OrderType t, OrderCondition c):
        ticker(tk),
        id(id),
        price(p),
        stop(st),
        quantity(q),
        side(s),
        orderType(t),
        condtion(c)
        {};
    
    void fillQty(Quantity fill_qty) {
        quantity -= fill_qty;
    }

    Quantity getMaxFillQty(Order *order) {
        return std::min(quantity, order->quantity);
    }

    ID encode(OrderID orderID, UserID userID) {
        return static_cast<ID>(userID) << 32 | orderID;
    }

    OrderID getOrderID() {
        return static_cast<OrderID>(id & 0xFFFFFFFF);
    }

    UserID getUserID() {
        return static_cast<UserID>(id >> 32);
    }
    
};
