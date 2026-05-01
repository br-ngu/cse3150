#pragma once
#include <iostream>

class LinkedList {
private:
    struct Node {
        int value;
        Node* next;
        Node(int value) : value(value), next(nullptr) {}
    };
    Node* head_;
    Node* tail_;

public:
    LinkedList() : head_(nullptr), tail_(nullptr) {}
    
    ~LinkedList() {
        Node* current = head_;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    void push_back(int value) {
        Node* newNode = new Node(value);
        if (!head_) {
            head_ = newNode;
            tail_ = newNode;
        } else {
            tail_->next = newNode;
            tail_ = newNode;
        }
    }

    void print(std::ostream& os) const {
        Node* current = head_;
        while (current) {
            os << current->value;
            if (current->next) os << " -> ";
            current = current->next;
        }
        os << "\n";
    }
};
