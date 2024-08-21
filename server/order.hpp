#pragma once

#include <string>
#include <cstdint>

typedef double Price;
typedef uint64_t Quantity;
typedef uint32_t OrderID;
typedef uint32_t UserID;
typedef uint64_t ID;
typedef std::string Ticker;

enum Side {
    BID,
    ASK
};

enum OrderType {
    ORDER,
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
    Price price = -1;
    Price stop = -1;
    Quantity quantity;
    Side side;
    OrderType orderType = ORDER;
    OrderCondition condtion = NONE;

    Order(): ticker(""), id(0), quantity(0), side(BID) {};
    Order(Ticker tk, ID id, Quantity q, Side s):
        ticker(tk),
        id(id),
        quantity(q),
        side(s)
        {};

    Order(Ticker tk, ID id, Price p, Quantity q, Side s, OrderType t):
        ticker(tk),
        id(id),
        price(p),
        quantity(q),
        side(s),
        orderType(t)
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
