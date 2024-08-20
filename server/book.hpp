#include <map>
#include <unordered_map>
#include <queue>
#include "order.hpp"


class OrderQueue {
    std::queue<Order> queue;
    Quantity quantity;
public:
    OrderQueue(): queue(), quantity(0) {};
    Quantity getOrderNumber() { return quantity;}
    void push(Order order) {
        queue.push(order);
        quantity += order.quantity;
    }
    void processOrder(Order *order) {
        Order match =  queue.front();
        int used = std::min(match.quantity, order->quantity);
        match.quantity -= used;
        order->quantity -= used;
        quantity -= used;
        if (match.quantity <= 0) {
            queue.pop();
        }
    }
};

class Book {
    std::map<Price, OrderQueue> bid;
    std::map<Price, OrderQueue> ask;
public:
    Book(): bid(), ask() {};

    Order matchBid(Order order) {
        for (auto it = ask.begin(); it != ask.end(); it++) {
            if (it->first > order.price) break;
            if (it->second.getOrderNumber() <= 0) continue;
            it->second.processOrder(&order); 
        }
        return order;
    }
    void insertBid(Order order) {
        order = matchBid(order);
        if (order.quantity <= 0) {
            return;
        }
        if (!bid.contains(order.price)) {
            bid[order.price] = OrderQueue();
        }
        bid[order.price].push(order);
    }

    Order matchAsk(Order order) {
        for (auto it = bid.rbegin(); it != bid.rend(); it++) {
            if (it->first > order.price) break;
            if (it->second.getOrderNumber() <= 0) continue;
            it->second.processOrder(&order); 
        }
        return order;
    }
    void insertAsk(Order order) {
        order = matchAsk(order);
        if (order.quantity <= 0) {
            return;
        }
        if (!ask.contains(order.price)) {
            ask[order.price] = OrderQueue();
        }
        ask[order.price].push(order);
    }

    std::vector<std::pair<Price, std::pair<Quantity, Quantity>>> exportData() {
        std::vector<std::pair<Price, std::pair<Quantity, Quantity>>> res;
        for (auto it = ask.rbegin(); it != ask.rend(); it++) {
           res.push_back({it->first, {0, it->second.getOrderNumber()}}); 
        }
        for (auto it = bid.begin(); it != bid.end(); it++) {
            res.push_back({it->first, {it->second.getOrderNumber(), 0}});
        } 
        return res;
    }
};
