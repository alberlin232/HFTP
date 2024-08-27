#pragma once

#include <map>
#include <unordered_map>
#include <iostream>
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
        tail->prev = node;
        dic[order.id] = node;
        quantity += order.quantity;
    }

    void push_front(Order order) {
        if (dic.contains(order.id)) {
            std::cout << "BIG PROBLEMS" << std::endl;
        }
        Node* node = new Node(order);
        Node* next = head->next;
        next->prev = node;
        node->next = next;
        node->prev = head;
        head->next = node;
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