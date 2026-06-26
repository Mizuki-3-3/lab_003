#include "array_seq.hpp"
#include "errors.hpp"
#include "sequence.hpp"

template<typename T>
array_seq<T>::array_seq(): arr(new dyn_arr<T>()) {}

template<typename T>
array_seq<T>::array_seq(size_t initial_size) : arr(new dyn_arr<T>(initial_size)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T>
array_seq<T>::array_seq(const T* items, size_t count) : arr(new dyn_arr<T>(items, count)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T>
array_seq<T>::array_seq(const array_seq& other) : arr(new dyn_arr<T>(*other.arr)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T>
array_seq<T>::array_seq(const array_seq&& move){
    arr = std::move(move.arr);
}

template<typename T>
array_seq<T>::array_seq(const std::initializer_list<T> initial_l): arr(new dyn_arr<T>(initial_l)){}

template<typename T>
array_seq<T>::array_seq(const dyn_arr<T>& other) : arr(new dyn_arr<T>(other)) {}

template<typename T>
array_seq<T>::~array_seq() { delete arr; }

template<typename T>
array_seq<T>& array_seq<T>::operator=(const array_seq& other) {
    if (this != &other) {
        arr = other.arr;
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
size_t array_seq<T>::size() const {
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
    for (size_t i = arr->size() - 1; i > 0; --i)
        (*arr)[i] = (*arr)[i - 1];
    (*arr)[0] = item;
    return this;
}

template<typename T>
sequence<T>* array_seq<T>::insert(const T& item, size_t index) {
    if (index > arr->size()) throw index_out_of_range("index out in array_seq");
    if (index == 0) return push_front(item);
    if (index == arr->size()) return push_back(item);
    arr->resize(arr->size() + 1);
    for (size_t i = arr->size() - 1; i > index; --i)
        (*arr)[i] = (*arr)[i - 1];
    (*arr)[index] = item;
    return this;
}

template<typename T>
sequence<T>* array_seq<T>::concat(sequence<T>* other) {
    if (other == nullptr) throw null_ptr();
    size_t new_size = size() + other->size();
    array_seq<T>* result = new array_seq<T>(new_size);
    if (result == nullptr) throw null_ptr();
    for (size_t i = 0; i < size(); ++i)
        (*result->arr)[i] = (*arr)[i];
    for (size_t i = 0; i < other->size(); ++i)
        (*result->arr)[size() + i] = (*other)[i];
    return result;
}

template<typename T>
sequence<T>* array_seq<T>::get_subsequence(size_t start, size_t end) const {
    if (start > end || end > size()) throw invalid_argument();
    size_t sub_size = end - start;
    array_seq<T>* sub = new array_seq<T>(sub_size);
    if (sub == nullptr) throw null_ptr();
    for (size_t i = start; i < end; ++i)
        (*sub->arr)[i - start] = (*arr)[i];
    return sub;
}

template<typename T>
auto array_seq<T>::insert(const_iterator index, const T& item) -> iterator{
    if (index == end()) {
        push_back(item);
        return arr->end() - 1;
    }
    size_t idx = index - arr->begin();
    insert(item, idx);
    return arr->begin() + idx;
}

template<typename T>
T& array_seq<T>::operator[](size_t index) {
    if (index >= size()) throw index_out_of_range();
    return (*arr)[index];
}

template<typename T>
const T& array_seq<T>::operator[](size_t index) const {
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
    size_t wr_i = 0;
    for (size_t r_i = 0; r_i < arr->size(); ++r_i) {
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
U array_seq<T>::reduce(Func f, U initial ) const {
    if (arr->size() == 0) {return initial;}
    U acc = f(initial, (*arr)[0]);
    for (size_t i = 1; i < arr->size(); ++i) {
        acc = f(acc, (*arr)[i]);
    }
    return acc;
}

// template<typename T>
// template <typename Func>
// auto array_seq<T>::reduce(Func f) const {
//     if (arr->size() == 0) throw empty_container();
//     auto acc = (*arr)[0];
//     for (size_t i = 1; i < arr->size(); i++) {
//         acc = f(acc, (*arr)[i]);
//     }
//     return acc;
// }