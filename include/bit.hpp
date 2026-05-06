#pragma once
#include <concepts>
#include <cstddef>
#include "Mutability.hpp"
#include "errors.hpp"


template <std::integral T>
class bit_proxy {
    T& ref;
    unsigned idx;
public:
    bit_proxy(T& value, size_t bit);
    bit_proxy& operator=(bool val);
    operator bool() const;
};

template <std::integral T>
class bit {
    T value;
    static constexpr unsigned len {sizeof(T)*8};
    using proxy = bit_proxy<T>;
public:
    bit();
    bit(T val);
    bit(const bit& other);

    bit& operator=(const bit& other);
    unsigned size() const;
    bool operator[](size_t idx) const;
    bit_proxy<T> operator[](size_t idx);
    bit operator&(const bit& other) const;
    bit operator|(const bit& other) const;
    bit operator^(const bit& other) const;
    template<typename U>
    bit<T> operator&(U other) const;
    template<typename U>
    bit<T> operator|(U other) const;
    template<typename U>
    bit<T> operator^(U other) const;

    bit operator~() const;

    T get_value() const;

    operator bool() const;
};

#include "bit.tpp"