#pragma once

#include <initializer_list>

template<typename T>
class dyn_arr {
private:
    T* data;
    size_t length;
public:
    class const_iterator;
    class iterator{ //*, ->, ++, --, ==,!=, +=,+,-=, -,[], -,><
        T* current;
    public:
       iterator(T* ptr);
        T& operator*() const;
        T* operator->() const;

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
       size_t operator- (const iterator& other) const{return other.current-current;}

        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
        bool operator< (const iterator& other) const;
        bool operator> (const iterator& other) const;
        bool operator<=(const iterator& other) const;
        bool operator>=(const iterator& other) const;
        bool operator==(const const_iterator& other) const { return current == other.current; }
        bool operator!=(const const_iterator& other) const { return current != other.current; }
        friend class const_iterator;
    };

    class const_iterator {
        const T* current;
    public:
        const_iterator(const T* ptr);
        const_iterator(const iterator& other) : current(other.current) {}
        const T& operator*() const;
        const T* operator->();

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
        size_t operator- (const const_iterator& other) const{return other.current-current;}

        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;
        bool operator< (const const_iterator& other) const;
        bool operator> (const const_iterator& other) const;
        bool operator<=(const const_iterator& other) const;
        bool operator>=(const const_iterator& other) const;
        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return current != other.current; }
        friend class iterator;
    };

    explicit dyn_arr();
    explicit dyn_arr(size_t initial_size);
    dyn_arr(const T* items, size_t initial_size);
    dyn_arr(const dyn_arr& other);
    dyn_arr(const dyn_arr&& move);
    dyn_arr(const std::initializer_list<T> initial_l);

    ~dyn_arr();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    dyn_arr& operator=(const dyn_arr&) = delete;
    dyn_arr& operator=(dyn_arr other);

    size_t size() const; 
    void resize(size_t new_size);
};
#include "dyn_arr.tpp"