#pragma once
#include <cstddef>

// Lista enlazada para guardar POSICIONES (int) con inserción O(1) al final.
class LinkedList {
    struct Node {
        int   value;
        Node* next;
        explicit Node(int v) : value(v), next(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    std::size_t size_;

public:
    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
    ~LinkedList() { clear(); }

    // Inserción O(1) al final
    void push_back(int v);

    // Utilidades
    bool empty() const { return head_ == nullptr; }
    std::size_t size() const { return size_; }
    void clear();

    // Iterador mínimo para range-for
    struct iterator {
        Node* cur;
        explicit iterator(Node* n) : cur(n) {}
        int& operator*() const { return cur->value; }
        iterator& operator++() { cur = cur->next; return *this; }
        bool operator!=(const iterator& other) const { return cur != other.cur; }
    };
    iterator begin() { return iterator(head_); }
    iterator end()   { return iterator(nullptr); }

    // Prohibimos copia para evitar dobles free; si quieres, puedes implementar move.
    LinkedList(const LinkedList&)            = delete;
    LinkedList& operator=(const LinkedList&) = delete;
};
