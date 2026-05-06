#include "bit_seq.hpp"
#include <utility>
#include <cstring>

template<Mutability M, std::integral T>
bit_seq<M,T>::bit_seq() : bit_s(std::make_unique<dyn_arr<bit<T>>>()) {}

template<Mutability M, std::integral T>
bit_seq<M,T>::bit_seq(bit<T>* items, unsigned count) {
    auto arr = std::make_unique<dyn_arr<bit<T>>>(items, count);
    if (!arr) throw null_ptr();
    bit_s = std::move(arr);
}

template<Mutability M, std::integral T>
bit_seq<M,T>::bit_seq(const bit_seq& other)
    : bit_s(std::make_unique<dyn_arr<bit<T>>>(*other.bit_s)) {}

template<Mutability M, std::integral T>
bit_seq<M,T>::bit_seq(unsigned initial_size)
    : bit_s(std::make_unique<dyn_arr<bit<T>>>(initial_size)) {}

template<Mutability M, std::integral T>
bit<T>& bit_seq<M,T>::operator[](unsigned index) {
    if (index >= bit_s->size()) throw index_out_of_range();
    return (*bit_s)[index];
}

template<Mutability M, std::integral T>
const bit<T>& bit_seq<M,T>::operator[](unsigned index) const {
    if (index >= bit_s->size()) throw index_out_of_range();
    return (*bit_s)[index];
}

template<Mutability M, std::integral T>
bit<T> bit_seq<M,T>::get_first() const {
    if (size() == 0) throw index_out_of_range();
    return (*bit_s)[0];
}

template<Mutability M, std::integral T>
bit<T> bit_seq<M,T>::get_last() const {
    if (size() == 0) throw index_out_of_range();
    return (*bit_s)[size() - 1];
}

template<Mutability M, std::integral T>
unsigned bit_seq<M,T>::size() const {
    return bit_s->size();
}


template<Mutability M, std::integral T>
bit_seq<M,T>& bit_seq<M,T>::operator=(const bit_seq& other) {
    if (this != &other) {
        bit_s = std::make_unique<dyn_arr<bit<T>>>(*other.bit_s);
        if (!bit_s) throw null_ptr();
    }
    return *this;
}

template<Mutability M, std::integral T>
sequence<bit<T>>* bit_seq<M,T>::append(const bit<T>& val) {
    if constexpr (M == Mutability::Mutable) {
        bit_s->resize(bit_s->size() + 1);
        (*bit_s)[bit_s->size() - 1] = val;
        return this;
    } else {
        auto new_arr = std::make_unique<dyn_arr<bit<T>>>(bit_s->size() + 1);
        if (!new_arr) throw null_ptr();
        for (unsigned i = 0; i < bit_s->size(); ++i)
            (*new_arr)[i] = (*bit_s)[i];
        (*new_arr)[bit_s->size()] = val;
        return new bit_seq<Mutability::Immutable, T>(std::move(new_arr));
    }
}

template<Mutability M, std::integral T>
sequence<bit<T>>* bit_seq<M,T>::prepend(const bit<T>& item) {
    if constexpr (M == Mutability::Mutable) {
        bit_s->resize(bit_s->size() + 1);
        for (unsigned i = bit_s->size() - 1; i > 0; --i)
            (*bit_s)[i] = (*bit_s)[i - 1];
        (*bit_s)[0] = item;
        return this;
    } else {
        auto new_arr = std::make_unique<dyn_arr<bit<T>>>(bit_s->size() + 1);
        if (!new_arr) throw null_ptr();
        (*new_arr)[0] = item;
        for (unsigned i = 0; i < bit_s->size(); ++i)
            (*new_arr)[i + 1] = (*bit_s)[i];
        return new bit_seq<Mutability::Immutable, T>(std::move(new_arr));
    }
}

template<Mutability M, std::integral T>
sequence<bit<T>>* bit_seq<M,T>::insert(const bit<T>& item, unsigned index) {
    if (index > bit_s->size()) throw index_out_of_range();
    if (index == 0) return prepend(item);
    if (index == bit_s->size()) return append(item);

    if constexpr (M == Mutability::Mutable) {
        bit_s->resize(bit_s->size() + 1);
        for (unsigned i = bit_s->size() - 1; i > index; --i)
            (*bit_s)[i] = (*bit_s)[i - 1];
        (*bit_s)[index] = item;
        return this;
    } else {
        auto new_arr = std::make_unique<dyn_arr<bit<T>>>(bit_s->size() + 1);
        if (!new_arr) throw null_ptr();
        for (unsigned i = 0; i < index; ++i)
            (*new_arr)[i] = (*bit_s)[i];
        (*new_arr)[index] = item;
        for (unsigned i = index; i < bit_s->size(); ++i)
            (*new_arr)[i + 1] = (*bit_s)[i];
        return new bit_seq<Mutability::Immutable, T>(std::move(new_arr));
    }
}

template<Mutability M, std::integral T>
sequence<bit<T>>* bit_seq<M,T>::concat(sequence<bit<T>>* other) {
    if (other == nullptr) throw null_ptr();
    unsigned new_size = bit_s->size() + other->size();
    auto new_arr = std::make_unique<dyn_arr<bit<T>>>(new_size);
    if (!new_arr) throw null_ptr();
    unsigned pos = 0;
    for (unsigned i = 0; i < bit_s->size(); ++i)
        (*new_arr)[pos++] = (*bit_s)[i];
    for (unsigned i = 0; i < other->size(); ++i)
        (*new_arr)[pos++] = (*other)[i];
    return new bit_seq<Mutability::Immutable, T>(std::move(new_arr));
}

template<Mutability M, std::integral T>
sequence<bit<T>>* bit_seq<M,T>::get_subsequence(unsigned start, unsigned end) const {
    if (start > end || end > size()) throw index_out_of_range();
    unsigned new_size = end - start;
    auto new_arr = std::make_unique<dyn_arr<bit<T>>>(new_size);
    if (!new_arr) throw null_ptr();
    for (unsigned i = start; i < end; ++i)
        (*new_arr)[i - start] = (*bit_s)[i];
    return new bit_seq<Mutability::Immutable, T>(std::move(new_arr));
}

template<Mutability M, std::integral T>
unsigned bit_seq<M,T>::find(const bit<T>& value) const {
    for (unsigned i = 0; i < size(); ++i) {
        if ((*bit_s)[i].get_value() == value.get_value())
            return i;
    }
    return size();
}

template<Mutability M, std::integral T>
bit_seq<M,T> bit_seq<M,T>::operator&(const bit<T>& other) const {
    bit_seq<M,T> result(size());
    for (unsigned i = 0; i < size(); ++i)
        result[i] = (*bit_s)[i] & other;
    return result;
}

template<Mutability M, std::integral T>
bit_seq<M,T> bit_seq<M,T>::operator|(const bit<T>& other) const {
    bit_seq<M,T> result(size());
    for (unsigned i = 0; i < size(); ++i)
        result[i] = (*bit_s)[i] | other;
    return result;
}

template<Mutability M, std::integral T>
bit_seq<M,T> bit_seq<M,T>::operator^(const bit<T>& other) const {
    bit_seq<M,T> result(size());
    for (unsigned i = 0; i < size(); ++i)
        result[i] = (*bit_s)[i] ^ other;
    return result;
}

template<Mutability M, std::integral T>
bit_seq<M,T> bit_seq<M,T>::operator~() const {
    bit_seq<M,T> result(size());
    for (unsigned i = 0; i < size(); ++i)
        result[i] = ~(*bit_s)[i];
    return result;
}

template<Mutability M, std::integral T>
template <typename Func>
bit_seq<M,T> bit_seq<M,T>::map(Func f) const {
    bit_seq<M,T> result(size());
    for (unsigned i = 0; i < size(); ++i)
        result[i] = f((*bit_s)[i]);
    return result;
}

template<Mutability M, std::integral T>
template <typename Func>
bit_seq<M,T> bit_seq<M,T>::where(Func f) const {
    unsigned count = 0;
    for (unsigned i = 0; i < size(); ++i)
        if (f((*bit_s)[i])) ++count;
    bit_seq<M,T> result(count);
    unsigned pos = 0;
    for (unsigned i = 0; i < size(); ++i)
        if (f((*bit_s)[i]))
            result[pos++] = (*bit_s)[i];
    return result;
}

template<Mutability M, std::integral T>
template <typename Func, typename U>
U bit_seq<M,T>::reduce(Func f, U initial) const {
    if (size() == 0) return initial;
    U acc = f(initial, (*bit_s)[0]);
    for (unsigned i = 1; i < size(); ++i)
        acc = f(acc, (*bit_s)[i]);
    return acc;
}