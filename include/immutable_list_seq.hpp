#pragma once

#include "sequence.hpp"
#include "linked_list.hpp"

template<typename T>
class immutable_list_seq : public sequence<T> {
private:
    forward_list<T>* list;

public:
    immutable_list_seq();
    explicit immutable_list_seq(const forward_list<T>& other);
    immutable_list_seq(const T* items, size_t count);
    immutable_list_seq(const immutable_list_seq& other);
    ~immutable_list_seq();

    immutable_list_seq& operator=(const immutable_list_seq& other);

    T get_first() const override;
    T get_last() const override;
    size_t size() const override;

    sequence<T>* push_back(const T& item) override;
    sequence<T>* push_front(const T& item) override;
    sequence<T>* insert(const T& item, size_t index) override;
    sequence<T>* concat(sequence<T>* other) override;
    sequence<T>* get_subsequence(size_t start, size_t end) const override;
    size_t find(const T& value) const override;

    T& operator[](size_t index) override;
    const T& operator[](size_t index) const override;

    auto begin() { return list->begin(); }
    auto end()   { return list->end(); }
    auto begin() const { return list->begin(); }
    auto end()   const { return list->end(); }

    template <typename Func>
    sequence<T>* map(Func f);

    template <typename Func>
    sequence<T>* where(Func f);

    template <typename Func, typename U>
    U reduce(Func f, U initial) const;
    using value_type = T;
};

#include "immutable_list_seq.tpp"