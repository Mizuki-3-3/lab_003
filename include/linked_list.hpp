#pragma once

#include "Mutability.hpp"

template <typename T>
struct node {
    T value;
    node<T>* next;
    node(const T& value);
};

template <typename T>
class s_list {
private:
    node<T>* head;
    node<T>* tail;
    unsigned length;

public:
    class iterator {
        node<T>* curr;
    public:
        iterator(node<T>* ptr);
        iterator& operator++();
        T& operator*();
        const T& operator*() const;
        int operator!=(const iterator& other) const;
        int operator==(const iterator& other) const;
    };

    class const_iterator {
        const node<T>* curr;
    public:
        const_iterator(const node<T>* ptr);
        const_iterator& operator++();
        const T& operator*() const;
        int operator!=(const const_iterator& other) const;
        int operator==(const const_iterator& other) const;
    };

    s_list();
    explicit s_list(unsigned initial_size);
    s_list(const T* data, unsigned initial_size);
    s_list(const s_list& other);
    ~s_list();

    T& operator[](unsigned index);
    const T& operator[](unsigned index) const;
    s_list& operator=(s_list other);
    s_list operator+(const s_list& right);

    unsigned size() const;
    T get_first() const;
    T get_last() const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    s_list<T> slice(unsigned start, unsigned end);

};

#include "linked_list.tpp"