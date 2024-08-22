#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include "engine.hpp"
#include "order.hpp"

void printHelp() {
    std::cout << "Commands:\n";
    std::cout << "  place <symbol> <quantity> <side> <type> [price] [stopPrice] [conditionType]\n";
    std::cout << "    Example (Limit): place APPLE 200 ASK LIMIT 10.10\n";
    std::cout << "    Example (Market): place APPLE 200 ASK MARKET\n";
    std::cout << "    Example (Stop-Limit): place APPLE 200 ASK STOP-LIMIT 10.20 9.80\n";
    std::cout << "  print <symbol>\n";
    std::cout << "    Example: print APPLE\n";
    std::cout << "  exit\n";
}

OrderCondition parseCondition(const std::string& conditionStr) {
    if (conditionStr == "AON") return AON;
    if (conditionStr == "IOC") return IOC;
    if (conditionStr == "FOK") return FOK;
    return NONE; // Default to NONE if no valid condition is provided
}

std::vector<std::string> tokenize(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    ID id_gen = 0;
    Engine engine;

    std::string command;
    std::cout << "Welcome to the Trading Engine CLI. Type 'help' for a list of commands.\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        } else if (command == "help") {
            printHelp();
        } else if (command.rfind("place", 0) == 0) {
            std::vector<std::string> tokens = tokenize(command);
            
            if (tokens.size() < 5) {
                std::cout << "Invalid command format. Type 'help' for instructions.\n";
                continue;
            }

            // Parse input parameters
            std::string symbol = tokens[1];
            Quantity quantity = std::stoi(tokens[2]);
            std::string side = tokens[3];
            std::string type = tokens[4];
            Price price = 0.0;
            Price stopPrice = NO_STOP_PRICE;
            std::string conditionStr = "NONE";

            // Check for LIMIT or STOP-LIMIT orders which require a price
            if (type == "LIMIT" || type == "STOP-LIMIT") {
                if (tokens.size() < 6) {
                    std::cout << "Missing price for LIMIT or STOP-LIMIT order.\n";
                    continue;
                }
                price = std::stod(tokens[5]);
            }

            // Check for STOP and STOP-LIMIT orders which require a stop price
            if (type == "STOP" || type == "STOP-LIMIT") {
                if (tokens.size() < 7) {
                    std::cout << "Missing stop price for STOP or STOP-LIMIT order.\n";
                    continue;
                }
                stopPrice = std::stod(tokens[6]);
            }

            // If a condition is specified, extract it, otherwise default to NONE
            if (tokens.size() >= 8) {
                conditionStr = tokens[7];
            }

            // Convert side and type strings to enums (assuming these enums are defined in your code)
            Side orderSide = (side == "ASK") ? ASK : BID;
            OrderType orderType;

            // Map strings to enums for order type
            if (type == "LIMIT") {
                orderType = LIMIT;
            } else if (type == "MARKET") {
                orderType = MARKET;
                // Set price and stop price to market defaults
                price = (side == "ASK") ? MARKET_ORDER_ASK_PRICE : MARKET_ORDER_BID_PRICE;
                stopPrice = NO_STOP_PRICE;  // No stop price for market orders
            } else if (type == "STOP") {
                orderType = STOP;
            } else if (type == "STOP-LIMIT") {
                orderType = STOP_LIMIT;
            } else {
                std::cout << "Unknown order type. Type 'help' for a list of commands.\n";
                continue;
            }

            // Parse the condition string into the enum
            OrderCondition condition = parseCondition(conditionStr);

            // Place the order
            engine.placeOrder(Order(symbol, id_gen++, price, stopPrice, quantity, orderSide, orderType, condition));
            std::cout << "Order placed: " << symbol << " " << quantity << " " << side << " " << type;
            if (type != "MARKET") {
                std::cout << " " << price;
            }
            if (type == "STOP" || type == "STOP-LIMIT") {
                std::cout << " Stop Price: " << stopPrice;
            }
            std::cout << " " << conditionStr << "\n";

        } else if (command.rfind("print", 0) == 0) {
            // Parsing the 'print' command
            std::vector<std::string> tokens = tokenize(command);

            if (tokens.size() < 2) {
                std::cout << "Invalid command format. Type 'help' for instructions.\n";
                continue;
            }

            std::string symbol = tokens[1];

            // Print stock information
            engine.printStock(symbol);

        } else {
            std::cout << "Unknown command. Type 'help' for a list of commands.\n";
        }
    }

    std::cout << "Exiting...\n";
    return 0;
}