#pragma once

#include "sequence.hpp"
#include "linked_list.hpp"

template<typename T>
class list_seq : public sequence<T> {
private:
    forward_list<T>* list;
public:
    using value_type = T;
    using iterator = typename forward_list<T>::iterator;
    using const_iterator = typename forward_list<T>::const_iterator;

    explicit list_seq();
    explicit list_seq(const forward_list<T>& other);
    list_seq(const T* items, size_t count);
    list_seq(const list_seq& other);
    list_seq(const list_seq&& move);
    list_seq(const std::initializer_list<T> initial_l);
    list_seq(size_t initial_size);
    ~list_seq();

    list_seq& operator=(const list_seq& other);

    T front() const override;
    T back() const override;
    size_t size() const override;

    sequence<T>* push_back(const T& item) override;
    sequence<T>* push_front(const T& item) override;
    sequence<T>* insert(const T& item, size_t index) override;
    sequence<T>* concat(sequence<T>* other) override;
    sequence<T>* get_subsequence(size_t start, size_t end) const override;

    iterator insert(const_iterator place, const T& item);

    T& operator[](size_t index) override;
    const T& operator[](size_t index) const override;

    auto begin() { return list->begin(); }
    auto end()   { return list->end(); }
    auto begin() const { return list->begin(); }
    auto end()   const { return list->end(); }

    iterator find(const T& value);
    const_iterator find(const T& value) const;

    template <typename Func>
    sequence<T>* map(Func f);

    template <typename Func>
    sequence<T>* where(Func f);

    template <typename Func, typename U>
    U reduce(Func f, U initial) const;
};
#include "list_seq.tpp"