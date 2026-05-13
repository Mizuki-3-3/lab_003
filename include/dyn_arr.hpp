#pragma once

#include <utility>

template<typename T>
class dyn_arr {
private:
    T* data;
    unsigned length;
public:
    class iterator{ //*, ->, ++, --, ==,!=, +=,+,-=, -,[], -,><
        T* current;
    public:
       iterator(T* ptr);
        T& operator*();
        T* operator->();

       iterator& operator++();
       iterator operator++(int);
       iterator& operator--();
       iterator operator--(int);

        bool operator!=(iterator& other) const;
        bool operator==(iterator& other) const;

       iterator& operator+=(std::ptrdiff_t n);
       iterator operator+(std::ptrdiff_t n);
       iterator& operator-=(std::ptrdiff_t n);
       iterator operator-(std::ptrdiff_t n);

        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
        bool operator< (const iterator& other) const;
        bool operator> (const iterator& other) const;
        bool operator<=(const iterator& other) const;
        bool operator>=(const iterator& other) const;

    };
    
    class const_iterator {
        const T* current;
    public:
        const_iterator(T* ptr);
        T& operator*();
        T* operator->();

        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);

        bool operator!=(const_iterator& other) const;
        bool operator==(const_iterator& other) const;

        const_iterator& operator+=(std::ptrdiff_t n);
        const_iterator operator+(std::ptrdiff_t n);
        const_iterator& operator-=(std::ptrdiff_t n);
        const_iterator operator-(std::ptrdiff_t n);

        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;
        bool operator< (const const_iterator& other) const;
        bool operator> (const const_iterator& other) const;
        bool operator<=(const const_iterator& other) const;
        bool operator>=(const const_iterator& other) const;

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