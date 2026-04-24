#pragma once

#include "sequence.hpp"
#include "dyn_arr.hpp"
#include "Mutability.hpp"

template<Mutability M, typename T>
class array_seq : public sequence<T> {
private:
    dyn_arr<T>* arr;

public:
    array_seq();
    explicit array_seq(unsigned initial_size);
    array_seq(const T* items, unsigned count);
    array_seq(const array_seq& other);
    array_seq(const dyn_arr<T>& other);
    ~array_seq();

    array_seq& operator=(const array_seq& other);

    T get_first() const override;
    T get_last() const override;
    unsigned size() const override;

    sequence<T>* append(const T& item) override;
    sequence<T>* prepend(const T& item) override;
    sequence<T>* insert(const T& item, unsigned index) override;
    sequence<T>* concat(sequence<T>* other) override;
    sequence<T>* get_subsequence(unsigned start, unsigned end) const override;
    unsigned find(const T& value) const override;

    T& operator[](unsigned index) override;
    const T& operator[](unsigned index) const override;

    auto begin() { return arr->begin(); }
    auto end()   { return arr->end(); }
    auto begin() const { return arr->begin(); }
    auto end()   const { return arr->end(); }

    template <typename Func>
    array_seq<M,T>* map(Func f);

    template <typename Func>
    array_seq<M,T>* where(Func f);

    template <typename Func, typename U>
    U reduce(Func f, U initial) const;

    using iterator = typename dyn_arr<T>::iterator;
    using const_iterator = typename dyn_arr<T>::const_iterator;
};

#include "array_seq.tpp"