#include <map>
#include <unordered_map>
#include <queue>

struct Order {
    std::string ticker;
    std::string id;
    double price;
    int quantity;
};

class OrderQueue {
    std::queue<Order> queue;
    int orders;
public:
    OrderQueue(): queue(), orders(0) {};
    int getOrderNumber() { return orders;}
    void push(Order order) {
        queue.push(order);
        orders += order.quantity;
    }
    void processOrder(Order *order) {
        Order match =  queue.front();
        int used = std::min(match.quantity, order->quantity);
        match.quantity -= used;
        order->quantity -= used;
        orders -= used;
        if (match.quantity <= 0) {
            queue.pop();
        }
    }
};

class Book {
    std::map<double, OrderQueue> bid;
    std::map<double, OrderQueue> ask;
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

    std::vector<std::pair<double, std::pair<int, int>>> exportData() {
        std::vector<std::pair<double, std::pair<int, int>>> res;
        for (auto it = ask.rbegin(); it != ask.rend(); it++) {
           res.push_back({it->first, {0, it->second.getOrderNumber()}}); 
        }
        for (auto it = bid.begin(); it != bid.end(); it++) {
            res.push_back({it->first, {it->second.getOrderNumber(), 0}});
        } 
        return res;
    }
};
