#pragma once

template<typename T>
class sequence {
public:
    virtual ~sequence() = default;

    virtual T front() const = 0;
    virtual T back() const = 0;
    virtual size_t size() const = 0;

    virtual sequence<T>* push_back(const T& item) = 0;
    virtual sequence<T>* push_front(const T& item) = 0;
    virtual sequence<T>* insert(const T& item, size_t idx) = 0;
    virtual sequence<T>* concat(sequence<T>* other) = 0;

    virtual sequence<T>* get_subsequence(size_t start, size_t end) const = 0;

    virtual T& operator[](size_t index) = 0;
    virtual const T& operator[](size_t index) const = 0;
};