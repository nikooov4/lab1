#pragma once

#include "UniquePtr.hpp" 
#include <iostream>

namespace SmartPointer {

    template <typename T>
    class LinkedListUnique {
    private:
        struct Node {
            T data;
            UniquePtr<Node> next;
            Node(T value) : data(value), next(nullptr) {}
        };

        UniquePtr<Node> head;

    public:
        LinkedListUnique() : head(nullptr) {}

        void push_front(T value) {
            UniquePtr<Node> newNode(new Node(value));
            newNode->next = std::move(head);
            head = std::move(newNode);
        }

        void print() const {
            const Node* current = head.get();
            while (current != nullptr) {
                std::cout << current->data << " -> ";
                current = current->next.get();
            }
            std::cout << "nullptr" << std::endl;
        }

        void pop_front() {
            if (head) {
                head = std::move(head->next);
            }
        }

        bool find(T value) const {
            const Node* current = head.get();
            while (current != nullptr) {
                if (current->data == value) {
                    return true;
                }
                current = current->next.get();
            }
            return false;
        }
    };
}
