#pragma once

#include <utility>

template<typename T>
class dyn_arr {
private:
    T* data;
    unsigned length;

    class contiguous_iterator{ //*, ->, ++, --, ==,!=, +=,+,-=, -,[], -,><
        T* current;
    public:
        contiguous_iterator(T* ptr);
        T& operator*();
        T* operator->();

        contiguous_iterator& operator++();
        contiguous_iterator operator++(int);
        contiguous_iterator& operator--();
        contiguous_iterator operator--(int);

        bool operator!=(contiguous_iterator& other) const;
        bool operator==(contiguous_iterator& other) const;

        contiguous_iterator& operator+=(std::ptrdiff_t n);
        contiguous_iterator operator+(std::ptrdiff_t n);
        contiguous_iterator& operator-=(std::ptrdiff_t n);
        contiguous_iterator operator-(std::ptrdiff_t n);

        bool operator==(const contiguous_iterator& other) const;
        bool operator!=(const contiguous_iterator& other) const;
        bool operator< (const contiguous_iterator& other) const;
        bool operator> (const contiguous_iterator& other) const;
        bool operator<=(const contiguous_iterator& other) const;
        bool operator>=(const contiguous_iterator& other) const;

    };
    
    class const_contiguous_iterator {
        const T* current;
    public:
        const_contiguous_iterator(T* ptr);
        T& operator*();
        T* operator->();

        const_contiguous_iterator& operator++();
        const_contiguous_iterator operator++(int);
        const_contiguous_iterator& operator--();
        const_contiguous_iterator operator--(int);

        bool operator!=(const_contiguous_iterator& other) const;
        bool operator==(const_contiguous_iterator& other) const;

        const_contiguous_iterator& operator+=(std::ptrdiff_t n);
        const_contiguous_iterator operator+(std::ptrdiff_t n);
        const_contiguous_iterator& operator-=(std::ptrdiff_t n);
        const_contiguous_iterator operator-(std::ptrdiff_t n);

        bool operator==(const const_contiguous_iterator& other) const;
        bool operator!=(const const_contiguous_iterator& other) const;
        bool operator< (const const_contiguous_iterator& other) const;
        bool operator> (const const_contiguous_iterator& other) const;
        bool operator<=(const const_contiguous_iterator& other) const;
        bool operator>=(const const_contiguous_iterator& other) const;

    };
public:
    dyn_arr();
    explicit dyn_arr(unsigned initial_size);
    dyn_arr(const T* items, unsigned initial_size);
    dyn_arr(const dyn_arr& other);
    ~dyn_arr();

    T& operator[](unsigned index);
    const T& operator[](unsigned index) const;

    contiguous_iterator begin();
    const_contiguous_iterator begin() const;
    contiguous_iterator end();
    const_contiguous_iterator end() const;

    dyn_arr& operator=(const dyn_arr&) = delete;
    dyn_arr& operator=(dyn_arr other);

    unsigned size() const; 
    void resize(unsigned new_size);

};

#include "dyn_arr.tpp"