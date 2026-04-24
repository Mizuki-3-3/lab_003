#pragma once

template<typename T>
class sequence {
public:
    virtual ~sequence() = default;

    virtual T get_first() const = 0;
    virtual T get_last() const = 0;
    virtual unsigned size() const = 0;

    virtual sequence<T>* append(const T& item) = 0;
    virtual sequence<T>* prepend(const T& item) = 0;
    virtual sequence<T>* insert(const T& item, unsigned index) = 0;
    virtual sequence<T>* concat(sequence<T>* other) = 0;

    virtual sequence<T>* get_subsequence(unsigned start, unsigned end) const = 0;
    virtual unsigned find(const T& value) const = 0;

    virtual T& operator[](unsigned index) = 0;
    virtual const T& operator[](unsigned index) const = 0;
};