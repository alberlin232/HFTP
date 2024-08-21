#include <iostream>

#include "order.hpp"
#include "engine.hpp"

int main() {
    ID id_gen = 0;
    Engine engine;
    engine.placeOrder(Order("APPLE", id_gen++, 10.10, 2, ASK, LIMIT));
    engine.placeOrder(Order("APPLE", id_gen++, 10.20, 2, ASK, LIMIT));
    engine.placeOrder(Order("APPLE", id_gen++, 10.30, 10, ASK, LIMIT));
    engine.placeOrder(Order("APPLE", id_gen++, 10.40, 5, ASK, LIMIT));
    engine.placeOrder(Order("APPLE", id_gen++, 10.20, 3, BID, LIMIT));
    engine.placeOrder(Order("APPLE", id_gen++, 9.90, 10, BID, LIMIT));
    engine.placeOrder(Order("APPLE", id_gen++, 9.80, 10, BID, LIMIT));
    engine.printStock("APPLE");
}

