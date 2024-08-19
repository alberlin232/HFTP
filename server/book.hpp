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
    std::map<int, OrderQueue> bid;
    std::map<int, OrderQueue> ask;
public:
    Book(): bid(), ask() {};

    Order matchBid(Order order) {
        while (ask.contains(order.price) && order.quantity > 0) {
            ask[order.price].processOrder(&order);
            if (ask[order.price].getOrderNumber() <= 0) {
                ask.erase(order.price);
            }
            
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
        while (bid.contains(order.price) && order.quantity > 0) {
            bid[order.price].processOrder(&order);
            if (bid[order.price].getOrderNumber() <= 0) {
                bid.erase(order.price);
            }
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
};
