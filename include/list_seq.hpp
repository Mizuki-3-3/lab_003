#pragma once

#include "sequence.hpp"
#include "linked_list.hpp"
#include "Mutability.hpp"

template<Mutability M, typename T>
class list_seq : public sequence<T> {
private:
    s_list<T>* list;

public:
    list_seq();
    explicit list_seq(const s_list<T>& other);
    list_seq(const T* items, unsigned count);
    list_seq(const list_seq& other);
    ~list_seq();

    list_seq& operator=(const list_seq& other);

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

    auto begin() { return list->begin(); }
    auto end()   { return list->end(); }
    auto begin() const { return list->begin(); }
    auto end()   const { return list->end(); }

    template <typename Func>
    list_seq<M,T>* map(Func f);

    template <typename Func>
    list_seq<M,T>* where(Func f);

    template <typename Func, typename U>
    U reduce(Func f, U initial) const;
};

#include "list_seq.tpp"