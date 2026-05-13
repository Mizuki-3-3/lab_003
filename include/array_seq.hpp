#pragma once
#include "sequence.hpp"
#include "dyn_arr.hpp"

template<typename T>
class array_seq : public sequence<T> {
private:
    dyn_arr<T>* arr;
public:
    array_seq();
    explicit array_seq(size_t initial_size);
    array_seq(const T* items, size_t count);
    array_seq(const array_seq& other);
    array_seq(const dyn_arr<T>& other);
    ~array_seq();
    array_seq& operator=(const array_seq& other);

    T front() const override;
    T back() const override;
    size_t size() const override;

    sequence<T>* push_back(const T& item) override;
    sequence<T>* push_front(const T& item) override;
    sequence<T>* insert(const T& item, size_t index) override;
    sequence<T>* concat(sequence<T>* other) override;
    sequence<T>* get_subsequence(size_t start, size_t end) const override;
    size_t find(const T& value) const override;

    T& operator[](size_t index) override;
    const T& operator[](size_t index) const override;

    auto begin() { return arr->begin(); }
    auto end()   { return arr->end(); }
    auto begin() const { return arr->begin(); }
    auto end()   const { return arr->end(); }

    template <typename Func>
    sequence<T>* map(Func f);
    template <typename Func>
    sequence<T>* where(Func f);
    template <typename Func, typename U>
    U reduce(Func f, U initial) const;

    using value_type = T;
    using iterator = dyn_arr<T>::iterator;
    using const_iterator = dyn_arr<T>::const_iterator;
};

#include "array_seq.tpp"