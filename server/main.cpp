#include <iostream>

#include "order.hpp"
#include "engine.hpp"

int main() {
    ID id_gen = 0;
    Engine engine;
    engine.placeOrder(Order("APPLE", id_gen++, 10.10, 2, ASK, LIMIT));
    engine.placeOrder(Order("APPLE", id_gen++, 3, BID));
    engine.printStock("APPLE");
}

