#pragma once

#include "Mutability.hpp"

template <typename T>
class forward_list {
private:
    class node {
    T value;
    node<T>* next;
    node(const T& value);
    node<T>* head;
    node<T>* tail;
    unsigned length;
    };
    class forward_iterator {//*, ->, ++, ++(int), ==, !=, 
        node<T>* curr;
    public:
        explicit forward_iterator(node<T>* ptr) noexcept: curr(ptr) {}
        T& operator*() const; //возвращаем неконстанты
        T* operator->() const;
        forward_iterator& operator++();
        forward_iterator operator++(int);
        bool operator!=(const forward_iterator& other) const;
        bool operator==(const forward_iterator& other) const;
    };

    class const_forward_iterator {//*, ->, ++, ++(int), ==, !=, 
        node<T>* curr;
    public:
        explicit const_forward_iterator(node<T>* ptr) noexcept: curr(ptr) {}
        const T& operator*() const; //возвращаем константы
        const T* operator->() const;
        const_forward_iterator& operator++();
        const_forward_iterator operator++(int);
        bool operator!=(const const_forward_iterator& other) const;
        bool operator==(const const_forward_iterator& other) const;
    };
public:
    

    

    forward_list();
    explicit forward_list(unsigned initial_size);
    forward_list(const T* data, unsigned initial_size);
    forward_list(const forward_list& other);
    ~forward_list();

    T& operator[](unsigned index);
    const T& operator[](unsigned index) const;
    forward_list& operator=(forward_list other);
    forward_list operator+(const forward_list& right);

    unsigned size() const;
    T get_first() const;
    T get_last() const;

    forward_iterator begin();
    forward_iterator end();
    const_forward_iterator begin() const;
    const_forward_iterator end() const;

    forward_list<T> slice(unsigned start, unsigned end);

};

#include "linked_list.tpp"