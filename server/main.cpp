#include <iostream>

#include "order.hpp"
#include "engine.hpp"

int main() {
    ID id_gen = 0;
    Engine engine;
    engine.placeOrder(Order("APPLE", id_gen++, 10.10, 200, ASK, LIMIT));
    engine.placeOrder(Order("APPLE", id_gen++, 10.20, 150, ASK, LIMIT));
    engine.placeOrder(Order("APPLE", id_gen++, 10.30, 100, ASK, LIMIT));
    engine.placeOrder(Order("APPLE", id_gen++, 10.40, 50, ASK, LIMIT));
    engine.placeOrder(Order("APPLE", id_gen++, 9.90, 100, BID, LIMIT));
    engine.placeOrder(Order("APPLE", id_gen++, 9.80, 50, BID, LIMIT));
    engine.printStock("APPLE");
}

