#pragma once
#include <iostream>
#include <memory>

class LinkedList {
private:
    struct Node {
        int value;
        std::unique_ptr<Node> next;
        Node(int value) : value(value), next(nullptr) {}
    };
    std::unique_ptr<Node> head_;
    Node* tail_;

public:
    LinkedList() : head_(nullptr), tail_(nullptr) {}
    ~LinkedList() = default;
    
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    void push_back(int value) {
        auto newNode = std::make_unique<Node>(value);
        Node* rawNode = newNode.get(); // Get raw pointer for the tail
        
        if (!head_) {
            head_ = std::move(newNode);
            tail_ = rawNode;
        } else {
            tail_->next = std::move(newNode);
            tail_ = rawNode;
        }
    }

    void print(std::ostream& os) const {
        Node* current = head_.get();
        while (current) {
            os << current->value;
            if (current->next) os << " -> ";
            current = current->next.get();
        }
        os << "\n";
    }
};
