#pragma once

#include <unordered_map>
#include <iomanip>
#include "book.hpp"

class Engine {
    std::unordered_map<Ticker, Book> stocks;
public:
    Engine(): stocks() {};

    void placeOrder(Order order) {
        if (!stocks.contains(order.ticker)) stocks.insert({order.ticker, Book()});
        stocks.at(order.ticker).processOrder(order);
    }

    void printStock(Ticker ticker) {
        if (!stocks.contains(ticker)) {
            std::cout << "No Stock: " << ticker << std::endl;
            return;
        }
        std::vector<std::pair<Price, std::pair<Quantity, Quantity>>> data = stocks.at(ticker).exportData();
        std::cout << std::setw(10) << "BID" << std::setw(15) << "Price" << std::setw(15) << "ASK" << std::endl;
        std::cout << std::string(40, '-') << std::endl; 
        for (const auto& entry : data) {
            Price price = entry.first;
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