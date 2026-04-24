#pragma once
#include <concepts>
#include <cstddef>


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
    static constexpr unsigned len {sizeof(T)};
    using proxy = bit_proxy<T>;
public:
    bit();
    bit(T val): value(val);
    bit(const bit& other);

    bit& operator=(const bit& other);
    unsigned size();
    bool operator[](size_t idx) const;
    bit_proxy<T> operator[](size_t idx);
    bit operator&(const bit& other);
    bit operator|(const bit& other);
    bit operator^(const bit& other);

    bit operator~(const bit& other);

    T get_value() const;

    operator bool() const;
};

#include "data_type/bit.tpp"