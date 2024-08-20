#include <unordered_map>
#include <iomanip>
#include "book.hpp"

class Engine {
    std::unordered_map<std::string, Book> stocks;
public:
    Engine(): stocks() {};

    void insertBid(Order order) {
        if (!stocks.contains(order.ticker)) stocks.insert({order.ticker, Book()});
        stocks.at(order.ticker).insertBid(order);
    }

    void insertAsk(Order order) {
        if (!stocks.contains(order.ticker)) stocks.insert({order.ticker, Book()});
        stocks.at(order.ticker).insertAsk(order);
    }

    void printStock(std::string ticker) {
        if (!stocks.contains(ticker)) {
            std::cout << "No Stock: " << ticker << std::endl;
        }
        std::vector<std::pair<double, std::pair<int, int>>> data = stocks.at(ticker).exportData();
        std::cout << std::setw(10) << "Buy" << std::setw(15) << "Price" << std::setw(15) << "Sell" << std::endl;
        std::cout << std::string(40, '-') << std::endl; 
        for (const auto& entry : data) {
            double price = entry.first;
            int buyQuantity = entry.second.first;
            int sellQuantity = entry.second.second;

            // Conditional printing with proper formatting
            std::cout << std::setw(10) << (buyQuantity > 0 ? std::to_string(buyQuantity) : " ")
                    << std::fixed << std::setprecision(2)
                    << std::setw(15) << price
                    << std::setw(15) << (sellQuantity > 0 ? std::to_string(sellQuantity) : " ")
                    << std::endl;
        } 
    }
};