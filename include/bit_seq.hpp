#pragma once
#include "sequence.hpp"
#include "bit.hpp"
#include "errors.hpp"
#include "dyn_arr.hpp"
#include <memory>

template <Mutability M, std::integral T = unsigned int>
class bit_seq : public sequence<bit<T>> {
private:
    std::unique_ptr<dyn_arr<bit<T>>> bit_s;
public:
    explicit bit_seq(std::unique_ptr<dyn_arr<bit<T>>> arr) : bit_s(std::move(arr)) {}
    bit_seq();
    bit_seq(bit<T>* items, unsigned count);
    bit_seq(const bit_seq& other);
    bit_seq(unsigned initial_size);
    ~bit_seq() = default;

    bit<T> get_first() const override;
    bit<T> get_last() const override;
    unsigned size() const override;
    bit<T>& operator[](unsigned index) override;
    const bit<T>& operator[](unsigned index) const override;
    bit_seq& operator=(const bit_seq& other);

    sequence<bit<T>>* get_subsequence(unsigned start, unsigned end) const override;
    sequence<bit<T>>* concat(sequence<bit<T>>* other) override;
    sequence<bit<T>>* append(const bit<T>& val) override;
    sequence<bit<T>>* prepend(const bit<T>& val) override;
    sequence<bit<T>>* insert(const bit<T>& val, unsigned index) override;
    unsigned find(const bit<T>& value) const override;

    bit_seq<M, T> operator&(const bit<T>& other) const;
    bit_seq<M, T> operator|(const bit<T>& other) const;
    bit_seq<M, T> operator^(const bit<T>& other) const;
    bit_seq<M, T> operator~() const;

    template <typename Func>
    bit_seq<M, T> map(Func f) const;

    template <typename Func>
    bit_seq<M, T> where(Func f) const;

    template <typename Func, typename U>
    U reduce(Func f, U initial) const;
    using value_type = bit<T>;
};

#include "bit_seq.tpp"