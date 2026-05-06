#include "bit.hpp"

template <std::integral T>
bit_proxy<T>::bit_proxy(T& value, size_t bit) : ref(value), idx(bit) {}

template <std::integral T>
bit_proxy<T>& bit_proxy<T>::operator=(bool val) {
    if (val) { ref |= (static_cast<T>(1) << idx); }
    else { ref &= ~(static_cast<T>(1) << idx); }
    return *this;
}

template <std::integral T>
bit_proxy<T>::operator bool() const { return (ref >> idx) & 1; }

template <std::integral T>
bit<T>::bit(): value(0) {}

template <std::integral T>
bit<T>::bit(T val): value(val) {}

template <std::integral T>
bit<T>::bit(const bit& other): value(other.value) {}

template <std::integral T>
bit<T>& bit<T>::operator=(const bit& other) {
    if (this == &other) return *this;
    value = other.value;
    return *this;
}

template <std::integral T>
unsigned bit<T>::size() const { return len; }

template <std::integral T>
bool bit<T>::operator[](size_t idx) const {
    if (idx >= len) throw index_out_of_range();
    return (value >> idx) & static_cast<T>(1);
}

template <std::integral T>
bit_proxy<T> bit<T>::operator[](size_t idx) {
    if (idx >= len) throw index_out_of_range();
    return bit_proxy<T>(value, idx);
}

template <std::integral T>
bit<T> bit<T>::operator&(const bit& other) const { return bit(value & other.value); }

template <std::integral T>
bit<T> bit<T>::operator|(const bit& other) const { return bit(value | other.value); }

template <std::integral T>
bit<T> bit<T>::operator^(const bit& other) const { return bit(value ^ other.value); }

template <std::integral T>
bit<T> bit<T>::operator~() const { return bit(~value); }

template <std::integral T>
T bit<T>::get_value() const { return value; }

template <std::integral T>
bit<T>::operator bool() const { return value != 0; }


template <std::integral T>
template <typename U>
bit<T> bit<T>::operator&(U other) const {
    return bit(value & static_cast<T>(other));
}

template <std::integral T>
template <typename U>
bit<T> bit<T>::operator|(U other) const {
    return bit(value | static_cast<T>(other));
}

template <std::integral T>
template <typename U>
bit<T> bit<T>::operator^(U other) const {
    return bit(value ^ static_cast<T>(other));
}