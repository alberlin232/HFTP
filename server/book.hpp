#pragma once

#include <map>
#include <unordered_map>
#include "order.hpp"

struct Node {
    Order order;
    Node* next;
    Node* prev;
    Node(Order order): order(order), next(nullptr), prev(nullptr) {};
};

class OrderQueue {
    Node* head;
    Node* tail;
    std::unordered_map<ID, Node*> dic;
    Quantity quantity;
public:
    OrderQueue(): dic(), quantity(0) {
        head = new Node(Order());
        tail = new Node(Order());
        head->next = tail;
        tail->prev = head;
    };

    Quantity getOrderNumber() { return quantity;}

    void push(Order order) {
        if (dic.contains(order.id)) {
            std::cout << "BIG PROBLEMS" << std::endl;
            exit(0);
        }
        Node* node = new Node(order); 
        Node* prev = tail->prev;
        prev->next = node;
        node->prev = prev;
        node->next = tail;
        dic[order.id] = node;
        quantity += order.quantity;
    }

    Order front() {
        //TODO: add error handling here
        return head->next->order;
    }

    void pop() {
        Node* node = head->next;
        head->next  = head->next->next;
        dic.erase(node->order.id);
        quantity -= node->order.quantity;
    }
    void processOrder(Order *order) {
        Order match = front();
        int fill_qty = match.getMaxFillQty(order);
        match.fillQty(fill_qty);
        order->fillQty(fill_qty);
        quantity -= fill_qty;
        if (match.quantity <= 0) {
            pop();
        }
    }
};

class Book {
    std::map<Price, OrderQueue, std::greater<Price>> bid;
    std::map<Price, OrderQueue> ask;
    Price market_price;

public:
    Book(): 
        bid(), 
        ask(),
        market_price(0)
        {};


    Order matchBid(Order order) {
        for (auto it = ask.begin(); it != ask.end(); it++) {
            if (order.quantity <= 0) break;
            if (it->second.getOrderNumber() <= 0) continue;
            it->second.processOrder(&order); 
        }
        return order;
    }

    Order matchBidLimit(Order order) {
        for (auto it = ask.begin(); it != ask.end(); it++) {
            if (it->first > order.price || order.quantity <= 0) break;
            if (it->second.getOrderNumber() <= 0) continue;
            it->second.processOrder(&order); 
        }
        return order;
    }

    Order matchAsk(Order order) {
        for (auto it = bid.begin(); it != bid.end(); it++) {
            if (order.quantity <= 0) break;
            if (it->second.getOrderNumber() <= 0) continue;
            it->second.processOrder(&order); 
        }
        return order;
    }

    Order matchAskLimit(Order order) {
        for (auto it = bid.begin(); it != bid.end(); it++) {
            if (it->first < order.price || order.quantity <= 0) break;
            if (it->second.getOrderNumber() <= 0) continue;
            it->second.processOrder(&order); 
        }
        return order;
    }

    void insertBid(Order order) {
        if (order.orderType == ORDER) order = matchBid(order);
        else if (order.orderType == LIMIT) order = matchBidLimit(order);
        if (order.quantity <= 0) {
            return;
        }
        if (!bid.contains(order.price)) {
            bid[order.price] = OrderQueue();
        }
        bid[order.price].push(order);
    }


    void insertAsk(Order order) {
        if (order.orderType == ORDER) order = matchAsk(order);
        else if (order.orderType == LIMIT) order = matchAskLimit(order);
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
        for (auto it = bid.rbegin(); it != bid.rend(); it++) {
            res.push_back({it->first, {it->second.getOrderNumber(), 0}});
        } 
        return res;
    }
};
