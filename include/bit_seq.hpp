#pragma once
#include <concepts>
#include <cstddef>


template <std::integral T>
class bit_proxy {
    T& ref;
    unsigned idx;
public:
    bit_proxy(T& value, size_t bit) : ref(value), idx(bit) {}
    bit_proxy& operator=(bool val) {
        if (val) { ref |= (static_cast<T>(1) << idx); }
        else { ref &= ~(static_cast<T>(1) << idx); }
        return *this;
    }

    operator bool() const { return (ref >> idx) & 1; }
};

template <std::integral T>
class bit {
    T value;
    static constexpr unsigned len {sizeof(T)};
    using proxy = bit_proxy<T>;
public:
    bit(): value(0) {}
    bit(T val): value(val){}
    bit(const bit& other) value(other.value){}

    bit& operator=(const bit& other){
        if (this == &other){
            return *this;
        }
        value = other.value;
        return *this;
    }
    unsigned size(){
        return len;
    }
    bool operator[](size_t idx) const{
        return (value>>idx)&static_cast<T>(1);
    }
    bit_proxy<T> operator[](size_t idx) {
        return bit_proxy<T>(value, idx);
    }
    bit operator&(const bit& other){
        return bit(value&other.value);
    }

    bit operator|(const bit& other){
        return bit(value|other.value);
    }
    bit operator^(const bit& other){
        return bit(value^other.value);
    }

    bit operator~(const bit& other){
        return bit(~value)
    }

    T get_value() const{return value;}

    operator bool() const {return value !=0;}
};