#pragma once

#include "Mutability.hpp"

template<typename T>
class dyn_arr {
private:
    T* data;
    unsigned length;
public:
    class iterator {
        T* current;
    public:
        iterator(T* ptr);
        iterator& operator++();
        iterator operator++(int);
        T& operator*();
        const T& operator*() const;
        int operator!=(iterator& other) const;
        int operator==(iterator& other) const;
    };
    
    class const_iterator {
        const T* current;
    public:
        const_iterator(const T* ptr);
        const_iterator& operator++();
        const_iterator operator++(int);
        const T& operator*() const;
        int operator!=(const const_iterator& other) const;
        int operator==(const const_iterator& other) const;
    };

    dyn_arr();
    explicit dyn_arr(unsigned initial_size);
    dyn_arr(const T* items, unsigned initial_size);
    dyn_arr(const dyn_arr& other);
    ~dyn_arr();

    T& operator[](unsigned index);
    const T& operator[](unsigned index) const;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    dyn_arr& operator=(const dyn_arr&) = delete;
    dyn_arr& operator=(dyn_arr other);

    unsigned size() const; 
    void resize(unsigned new_size);

};

#include "dyn_arr.tpp"