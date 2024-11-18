#pragma once

#ifndef LINKED_LIST_SHARED_PTR_H
#define LINKED_LIST_SHARED_PTR_H

#include "SharedPtr.hpp" 
#include <iostream>

namespace SmartPointer {

    template <typename T>
    class LinkedListShared {
    private:
        struct Node {
            T data;
            SharedPtr<Node> next;
            Node(T value) : data(value), next(nullptr) {}
        };

        SharedPtr<Node> head;

    public:
        LinkedListShared() : head(nullptr) {}

        void pushFront(T value) {
            SharedPtr<Node> newNode(new Node(value));
            newNode->next = head;
            head = newNode;
        }

        void print() const {
            SharedPtr<Node> current = head;
            while (current) {
                std::cout << current->data << " -> ";
                current = current->next;
            }
            std::cout << "nullptr" << std::endl;
        }

        void popFront() {
            if (head) {
                head = head->next;
            }
        }

        bool find(T value) const {
            SharedPtr<Node> current = head;
            while (current) {
                if (current->data == value) {
                    return true;
                }
                current = current->next;
            }
            return false;
        }
    };
}

#endif