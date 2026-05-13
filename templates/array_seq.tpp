#include "array_seq.hpp"
#include "errors.hpp"

template<typename T>
array_seq<T>::array_seq(): arr(new dyn_arr<T>()) {}

template<typename T>
array_seq<T>::array_seq(unsigned initial_size) : arr(new dyn_arr<T>(initial_size)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T>
array_seq<T>::array_seq(const T* items, unsigned count) : arr(new dyn_arr<T>(items, count)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T>
array_seq<T>::array_seq(const array_seq& other) : arr(new dyn_arr<T>(*other.arr)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T>
array_seq<T>::array_seq(const dyn_arr<T>& other) : arr(new dyn_arr<T>(other)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T>
array_seq<T>::~array_seq() { delete arr; }

template<typename T>
array_seq<T>& array_seq<T>::operator=(const array_seq& other) {
    if (this != &other) {
        dyn_arr<T>* new_arr = new dyn_arr<T>(*other.arr);
        if (new_arr == nullptr) throw null_ptr();
        delete arr;
        arr = new_arr;
    }
    return *this;
}

template<typename T>
T array_seq<T>::front() const {
    if (size() == 0) throw empty_container();
    return (*arr)[0];
}

template<typename T>
T array_seq<T>::back() const {
    if (size() == 0) throw empty_container();
    return (*arr)[size() - 1];
}

template<typename T>
unsigned array_seq<T>::size() const {
    return arr->size();
}

template<typename T>
sequence<T>* array_seq<T>::push_back(const T& item) {
    arr->resize(arr->size() + 1);
    (*arr)[arr->size() - 1] = item;
    return this;
}

template<typename T>
sequence<T>* array_seq<T>::push_front(const T& item) {
    arr->resize(arr->size() + 1);
    for (unsigned i = arr->size() - 1; i > 0; --i)
        (*arr)[i] = (*arr)[i - 1];
    (*arr)[0] = item;
    return this;
}

template<typename T>
sequence<T>* array_seq<T>::insert(const T& item, unsigned index) {
    if (index > arr->size()) throw index_out_of_range();
    if (index == 0) return push_front(item);
    if (index == arr->size()) return push_back(item);
    arr->resize(arr->size() + 1);
    for (unsigned i = arr->size() - 1; i > index; --i)
        (*arr)[i] = (*arr)[i - 1];
    (*arr)[index] = item;
    return this;
}

template<typename T>
sequence<T>* array_seq<T>::concat(sequence<T>* other) {
    if (other == nullptr) throw null_ptr();
    unsigned new_size = size() + other->size();
    array_seq<T>* result = new array_seq<T>(new_size);
    if (result == nullptr) throw null_ptr();
    for (unsigned i = 0; i < size(); ++i)
        (*result->arr)[i] = (*arr)[i];
    for (unsigned i = 0; i < other->size(); ++i)
        (*result->arr)[size() + i] = (*other)[i];
    return result;
}

template<typename T>
sequence<T>* array_seq<T>::get_subsequence(unsigned start, unsigned end) const {
    if (start > end || end > size()) throw invalid_argument();
    unsigned sub_size = end - start;
    array_seq<T>* sub = new array_seq<T>(sub_size);
    if (sub == nullptr) throw null_ptr();
    for (unsigned i = start; i < end; ++i)
        (*sub->arr)[i - start] = (*arr)[i];
    return sub;
}

template<typename T>
unsigned array_seq<T>::find(const T& value) const {
    for (unsigned i = 0; i < size(); ++i) {
        if ((*arr)[i] == value) return i;
    }
    throw invalid_argument();
}

template<typename T>
T& array_seq<T>::operator[](unsigned index) {
    if (index >= size()) throw index_out_of_range();
    return (*arr)[index];
}

template<typename T>
const T& array_seq<T>::operator[](unsigned index) const {
    if (index >= size()) throw index_out_of_range();
    return (*arr)[index];
}

template<typename T>
template <typename Func>
sequence<T>* array_seq<T>::map(Func f) {
    for (auto& x : *arr) x = f(x);
    return this;
}

template<typename T>
template <typename Func>
sequence<T>* array_seq<T>::where(Func f) {
    unsigned wr_i = 0;
    for (unsigned r_i = 0; r_i < arr->size(); ++r_i) {
        if (f((*arr)[r_i])) {
            if (wr_i != r_i) (*arr)[wr_i] = (*arr)[r_i];
            ++wr_i;
        }
    }
    arr->resize(wr_i);
    return this;
}

template<typename T>
template <typename Func, typename U>
U array_seq<T>::reduce(Func f, U initial) const {
    if (arr->size() == 0) throw empty_container();
    U acc = f(initial, (*arr)[0]);
    for (unsigned i = 1; i < arr->size(); ++i) {
        acc = f(acc, (*arr)[i]);
    }
    return acc;
}