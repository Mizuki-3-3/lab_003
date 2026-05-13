#include "immutable_array_seq.hpp"
#include "errors.hpp"

template<typename T>
immutable_array_seq<T>::immutable_array_seq(): arr(new dyn_arr<T>()) {}

template<typename T>
immutable_array_seq<T>::immutable_array_seq(size_t initial_size) : arr(new dyn_arr<T>(initial_size)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T>
immutable_array_seq<T>::immutable_array_seq(const T* items, size_t count) : arr(new dyn_arr<T>(items, count)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T>
immutable_array_seq<T>::immutable_array_seq(const immutable_array_seq& other) : arr(new dyn_arr<T>(*other.arr)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T>
immutable_array_seq<T>::immutable_array_seq(const dyn_arr<T>& other) : arr(new dyn_arr<T>(other)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T>
immutable_array_seq<T>::~immutable_array_seq() { delete arr; }

template<typename T>
immutable_array_seq<T>& immutable_array_seq<T>::operator=(const immutable_array_seq& other) {
    if (this != &other) {
        dyn_arr<T>* new_arr = new dyn_arr<T>(*other.arr);
        if (new_arr == nullptr) throw null_ptr();
        delete arr;
        arr = new_arr;
    }
    return *this;
}

template<typename T>
T immutable_array_seq<T>::get_first() const {
    if (size() == 0) throw empty_container();
    return (*arr)[0];
}

template<typename T>
T immutable_array_seq<T>::get_last() const {
    if (size() == 0) throw empty_container();
    return (*arr)[size() - 1];
}

template<typename T>
size_t immutable_array_seq<T>::size() const {
    return arr->size();
}

template<typename T>
sequence<T>* immutable_array_seq<T>::push_back(const T& item) {
    immutable_array_seq<T>* new_seq = new immutable_array_seq<T>(arr->size() + 1);
    if (!new_seq) throw null_ptr();
    for (size_t i = 0; i < arr->size(); i++) {
        (*new_seq->arr)[i] = (*arr)[i];
    }
    (*new_seq->arr)[arr->size()] = item;
    return new_seq;
}

template<typename T>
sequence<T>* immutable_array_seq<T>::push_front(const T& item) {
    dyn_arr<T>* new_arr = new dyn_arr<T>(arr->size() + 1);
    if (!new_arr) throw null_ptr();
    (*new_arr)[0] = item;
    for (size_t i = 0; i < arr->size(); ++i)
        (*new_arr)[i + 1] = (*arr)[i];
    immutable_array_seq<T>* result = new immutable_array_seq<T>(*new_arr);
    if (!result) throw null_ptr();
    delete new_arr;
    return result;
}

template<typename T>
sequence<T>* immutable_array_seq<T>::insert(const T& item, size_t index) {
    if (index > arr->size()) throw index_out_of_range();
    if (index == 0) return push_front(item);
    if (index == arr->size()) return push_back(item);
    size_t ns = arr->size() + 1;
    dyn_arr<T>* new_arr = new dyn_arr<T>(ns);
    if (new_arr == nullptr) throw null_ptr();
    for (size_t i = 0; i < index; ++i)
        (*new_arr)[i] = (*arr)[i];
    (*new_arr)[index] = item;
    for (size_t i = index; i < arr->size(); ++i)
        (*new_arr)[i + 1] = (*arr)[i];
    immutable_array_seq<T>* result = new immutable_array_seq<T>(*new_arr);
    if (result == nullptr) throw null_ptr();
    delete new_arr;
    return result;
}

template<typename T>
sequence<T>* immutable_array_seq<T>::concat(sequence<T>* other) {
    if (other == nullptr) throw null_ptr();
    size_t new_size = size() + other->size();
    immutable_array_seq<T>* result = new immutable_array_seq<T>(new_size);
    if (result == nullptr) throw null_ptr();
    for (size_t i = 0; i < size(); ++i)
        (*result->arr)[i] = (*arr)[i];
    for (size_t i = 0; i < other->size(); ++i)
        (*result->arr)[size() + i] = (*other)[i];
    return result;
}

template<typename T>
sequence<T>* immutable_array_seq<T>::get_subsequence(size_t start, size_t end) const {
    if (start > end || end > size()) throw invalid_argument();
    size_t sub_size = end - start;
    immutable_array_seq<T>* sub = new immutable_array_seq<T>(sub_size);
    if (sub == nullptr) throw null_ptr();
    for (size_t i = start; i < end; ++i)
        (*sub->arr)[i - start] = (*arr)[i];
    return sub;
}

template<typename T>
size_t immutable_array_seq<T>::find(const T& value) const {
    for (size_t i = 0; i < size(); ++i) {
        if ((*arr)[i] == value) return i;
    }
    throw invalid_argument();
}

template<typename T>
T& immutable_array_seq<T>::operator[](size_t index) {
    if (index >= size()) throw index_out_of_range();
    return (*arr)[index];
}

template<typename T>
const T& immutable_array_seq<T>::operator[](size_t index) const {
    if (index >= size()) throw index_out_of_range();
    return (*arr)[index];
}

template<typename T>
template <typename Func>
sequence<T>* immutable_array_seq<T>::map(Func f) {
    immutable_array_seq<T>* new_arr = new immutable_array_seq<T>(*arr);
    if (new_arr == nullptr) throw null_ptr();
    for (auto& x : *(new_arr->arr)) x = f(x);
    return new_arr;
}

template<typename T>
template <typename Func>
sequence<T>* immutable_array_seq<T>::where(Func f) {
    dyn_arr<T>* new_data = new dyn_arr<T>(arr->size());
    if (!new_data) throw null_ptr();
    size_t pos = 0;
    for (size_t i = 0; i < arr->size(); ++i) {
        if (f((*arr)[i])) {
            (*new_data)[pos++] = (*arr)[i];
        }
    }
    immutable_array_seq<T>* result = new immutable_array_seq<T>(pos);
    if (!result) throw null_ptr();
    for (size_t i = 0; i < pos; ++i) (*result->arr)[i] = (*new_data)[i];
    delete new_data;
    return result;
}

template<typename T>
template <typename Func, typename U>
U immutable_array_seq<T>::reduce(Func f, U initial) const {
    if (arr->size() == 0) throw empty_container();
    U acc = f(initial, (*arr)[0]);
    for (size_t i = 1; i < arr->size(); ++i) {
        acc = f(acc, (*arr)[i]);
    }
    return acc;
}