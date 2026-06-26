#pragma once

#include <initializer_list>

template <typename T>
class forward_list {
private:
    struct node {
    T value;
    node* next;
    node(const T& val);
    };
    
    node* M_head;
    node* tail;
    size_t length;//real length one more bigger
public:
    class const_iterator;
    class iterator {//*, ->, ++, ++(int), ==, !=, 
        node* curr;
    public:
        explicit iterator(node* ptr) noexcept: curr(ptr) {}
        iterator(const const_iterator& other) : curr(other.curr) {}
        T& operator*() const; //возвращаем неконстанты
        T* operator->() const;
        iterator& operator++();
        iterator operator++(int);
        iterator operator+(int right);
        bool operator!=(const iterator& other) const;
        bool operator==(const iterator& other) const;
        bool operator==(const const_iterator& other) const { return curr == other.curr; }
        bool operator!=(const const_iterator& other) const { return curr != other.curr; }
        friend class const_iterator;
    };

    class const_iterator {//*, ->, ++, ++(int), ==, !=, 
        node* curr;
    public:
        explicit const_iterator(node* ptr) noexcept: curr(ptr) {}
        const_iterator(const iterator& other) : curr(other.curr) {}
        const T& operator*() const; //возвращаем константы
        const T* operator->() const;
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator operator+(int right);
        bool operator!=(const const_iterator& other) const;
        bool operator==(const const_iterator& other) const;
        friend class iterator;
    };

    explicit forward_list();
    explicit forward_list(size_t initial_size);
    forward_list(const T* data, size_t initial_size);
    forward_list(const forward_list& other);
    forward_list(forward_list&& move);
    forward_list(const std::initializer_list<T> initial_l);
    ~forward_list();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    forward_list& operator=(forward_list other);
    forward_list operator+(const forward_list& right);

    size_t size() const;
    T front() const;
    T back() const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    iterator before_begin();
    iterator before_end();

    forward_list<T> slice(size_t start, size_t end);
    forward_list<T>* insert_after(const_iterator place, const T& value);
};
#include "linked_list.tpp"