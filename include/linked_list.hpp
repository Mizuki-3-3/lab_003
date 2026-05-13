#pragma once

template <typename T>
class forward_list {
private:
    struct node {
    T value;
    node* next;
    node(const T& val);
    };
    
    node* head;
    node* tail;
    unsigned length;
public:
    class iterator {//*, ->, ++, ++(int), ==, !=, 
        node* curr;
    public:
        explicit iterator(node* ptr) noexcept: curr(ptr) {}
        T& operator*() const; //возвращаем неконстанты
        T* operator->() const;
        iterator& operator++();
        iterator operator++(int);
        bool operator!=(const iterator& other) const;
        bool operator==(const iterator& other) const;
    };

    class const_iterator {//*, ->, ++, ++(int), ==, !=, 
        node* curr;
    public:
        explicit const_iterator(node* ptr) noexcept: curr(ptr) {}
        const T& operator*() const; //возвращаем константы
        const T* operator->() const;
        const_iterator& operator++();
        const_iterator operator++(int);
        bool operator!=(const const_iterator& other) const;
        bool operator==(const const_iterator& other) const;
    };

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

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    forward_list<T> slice(unsigned start, unsigned end);

};

#include "linked_list.tpp"