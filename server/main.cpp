#include <iostream>
#include "engine.hpp"

int main() {
    Engine engine;
    engine.insertAsk({"APPLE", "ABE", 10.10, 2});
    engine.insertAsk({"APPLE", "ABE", 10.20, 2});
    engine.insertAsk({"APPLE", "ABE", 10.30, 10});
    engine.insertAsk({"APPLE", "ABE", 10.40, 5});
    engine.insertBid({"APPLE", "ABE", 10.20, 3});
    engine.insertBid({"APPLE", "ABE", 9.90, 10});
    engine.insertBid({"APPLE", "ABE", 9.80, 10});
    engine.printStock("APPLE");
}

