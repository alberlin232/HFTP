#include <unordered_map>
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

    }
};