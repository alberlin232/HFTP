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

    Order* front() {
        //TODO: add error handling here
        return &(head->next->order);
    }

    void pop() {
        Node* node = head->next;
        head->next  = head->next->next;
        dic.erase(node->order.id);
        quantity -= node->order.quantity;
    }

    bool remove(ID id) {
        if (!dic.contains(id)) return false;
        Node* node = dic.at(id);
        node->prev->next = node->next;
        node->next->prev = node->prev;
        dic.erase(id);
        return true;
    }

    void decQty(Quantity qty) {
        quantity -= qty;
    }
};

class Book {
    std::map<Price, OrderQueue, std::greater<Price>> bids;
    std::map<Price, OrderQueue> asks;
    std::map<Price, OrderQueue, std::greater<Price>> stopBids;
    std::map<Price, OrderQueue> stopAsks;
    Price marketPrice;

public:
    Book(): 
        bids(), 
        asks(),
        stopBids(),
        stopAsks(),
        marketPrice(0)
        {};

    void processOrder(Order order) {
        if (order.orderType == MARKET || order.orderType == LIMIT) {
            if (order.side == BID) insertBid(order);
            else if (order.side == ASK) insertAsk(order);
        } else if (order.orderType == STOP || order.orderType == STOP_LIMIT) {
            if (order.side == BID) insertStopBid(order);
            else if (order.side == ASK) insertStopAsk(order);
        }
    }

    bool removeOrder(Order order) {
        bool removed = false;
        if (order.orderType == STOP || order.orderType == STOP_LIMIT) {
            removed = order.side ? stopAsks.at(order.stop).remove(order.id) : stopBids.at(order.stop).remove(order.id);
        } 
        if (!removed) {
            removed = order.side ? asks.at(order.price).remove(order.id) : bids.at(order.price).remove(order.id);
        }
        return removed;
    }

    void insertStopBid(Order order) {
        if (!stopBids.contains(order.stop)) {
            stopBids.insert({order.stop, OrderQueue()});
        }
        stopBids.at(order.stop).push(order);
    }

    void insertStopAsk(Order order) {
        if (!stopAsks.contains(order.stop)) {
            stopAsks.insert({order.stop, OrderQueue()});
        }
        stopAsks.at(order.stop).push(order);
    }

    void updatePrice(Price lastPrice) {
        Price oldPrice = marketPrice;
        marketPrice = lastPrice;

        //trigger stops here
        if (oldPrice < marketPrice){
            //ask stops
            activateStopAsks();
        } else if (oldPrice > marketPrice) {
            //bid stops
            activateStopBids();
        }
    }

    void activateStopAsks() {
        for (auto it = stopAsks.begin(); it != stopAsks.end(); it++) {
            if (marketPrice > it->first) break;
            while (it->second.getOrderNumber() > 0) {
                insertAsk(*(it->second.front()));
                it->second.pop();
            }
        }
    }

    void activateStopBids() {
        for (auto it = stopBids.begin(); it != stopBids.end(); it++) {
            if (marketPrice < it->first) break;
            while (it->second.getOrderNumber() > 0) {
                insertAsk(*(it->second.front()));
                it->second.pop();
            }
        }
    }

    Order matchBid(Order order) {
        Price lastPrice;
        for (auto it = asks.begin(); it != asks.end(); it++) {
            if (order.quantity <= 0 || ((order.orderType == LIMIT || order.orderType == STOP_LIMIT)
                 && it->first > order.price)) break;
            if (it->second.getOrderNumber() <= 0) continue;
            while (it->second.getOrderNumber() > 0 && order.quantity > 0) {
                Order* match = it->second.front();
                int fill_qty = match->getMaxFillQty(&order);
                match->fillQty(fill_qty);
                order.fillQty(fill_qty);
                it->second.decQty(fill_qty);
                if (match->quantity <= 0) {
                    it->second.pop();
                }
            }
            lastPrice = it->first;
        }
        updatePrice(lastPrice);
        return order;
    }


    Order matchAsk(Order order) {
        Price lastPrice;
        for (auto it = bids.begin(); it != bids.end(); it++) {
            if (order.quantity <= 0 || ((order.orderType == LIMIT || order.orderType == STOP_LIMIT)
                 && it->first < order.price)) break;
            if (it->second.getOrderNumber() <= 0) continue;
            while (it->second.getOrderNumber() > 0 && order.quantity > 0) {
                Order* match = it->second.front();
                int fill_qty = match->getMaxFillQty(&order);
                match->fillQty(fill_qty);
                order.fillQty(fill_qty);
                it->second.decQty(fill_qty);
                if (match->quantity <= 0) {
                    it->second.pop();
                }
            }
            lastPrice = it->first;
        }
        updatePrice(lastPrice);
        return order;
    }

    void insertBid(Order order) {
        order = matchBid(order);
        if (order.quantity <= 0) {
            return;
        }
        if (!bids.contains(order.price)) {
            bids[order.price] = OrderQueue();
        }
        bids[order.price].push(order);
    }


    void insertAsk(Order order) {
        order = matchAsk(order);
        if (order.quantity <= 0) {
            return;
        }
        if (!asks.contains(order.price)) {
            asks[order.price] = OrderQueue();
        }
        asks[order.price].push(order);
    }

    std::vector<std::pair<Price, std::pair<Quantity, Quantity>>> exportData() {
        std::vector<std::pair<Price, std::pair<Quantity, Quantity>>> res;
        for (auto it = asks.rbegin(); it != asks.rend(); it++) {
            if (it->first == MARKET_ORDER_ASK_PRICE) continue;
            res.push_back({it->first, {0, it->second.getOrderNumber()}}); 
        }
        for (auto it = bids.begin(); it != bids.end(); it++) {
            if (it->first == MARKET_ORDER_BID_PRICE) continue;
            res.push_back({it->first, {it->second.getOrderNumber(), 0}});
        } 
        return res;
    }
};
