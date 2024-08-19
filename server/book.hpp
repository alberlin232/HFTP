#include <map>
#include <unordered_map>
#include <queue>

struct Order {
    std::string ticker;
    std::string id;
    double price;
    int quantity;
};


class Book {
    std::map<int, std::queue<Order> > bid;
    std::map<int, std::queue<Order> > ask;
public:
    Book(): bid(), ask() {};

    Order matchBid(Order order) {
        while (ask.contains(order.price) && order.quantity > 0) {
            Order &match = ask[order.price].front();
            int temp_quant = match.quantity;
            match.quantity -= order.quantity;
            order.quantity -= temp_quant;
            if (match.quantity <= 0) {
                ask[order.price].pop();
                if (ask.size() <= 0) {
                    ask.erase(order.price);
                }
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
            bid[order.price] = std::queue<Order>();
        }
        bid[order.price].push(order);
    }

    Order matchAsk(Order order) {
        while (bid.contains(order.price) && order.quantity > 0) {
            Order &match = bid[order.price].front();
            int temp_quant = match.quantity;
            match.quantity -= order.quantity;
            order.quantity -= temp_quant;
            if (match.quantity <= 0) {
                bid[order.price].pop();
                if (bid.size() <= 0) {
                    bid.erase(order.price);
                }
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
            ask[order.price] = std::queue<Order>();
        }
        ask[order.price].push(order);
    }

    void printAsk() {
        for (const auto& [key, value] : ask) {
            std::queue<Order> copy = value;
            while (!copy.empty()) {
                std::cout << copy.front().quantity << std::endl;
                copy.pop();
            }
        }
    }
};
