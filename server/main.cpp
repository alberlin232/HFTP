#include <iostream>
#include "engine.hpp"

int main() {
    Engine engine;
    engine.insertAsk({"APPLE", "ABE", 100.0, 10});
    engine.insertAsk({"APPLE", "ABE", 100.0, 9});
    engine.insertBid({"APPLE", "ABE", 100.0, 20});
}

