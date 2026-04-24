#include "dyn_arr.hpp"
#include "errors.hpp"

template<typename T>
dyn_arr<T>::iterator::iterator(T* ptr) : current(ptr) {}

template<typename T>
typename dyn_arr<T>::iterator& dyn_arr<T>::iterator::operator++() {
    ++current;
    return *this;
}

template<typename T>
typename dyn_arr<T>::iterator dyn_arr<T>::iterator::operator++(int) {
    iterator tmp = *this;
    ++current;
    return tmp;
}

template<typename T>
T& dyn_arr<T>::iterator::operator*() { return *current; }

template<typename T>
const T& dyn_arr<T>::iterator::operator*() const { return *current; }

template<typename T>
int dyn_arr<T>::iterator::operator!=(iterator& other) const { return current != other.current; }

template<typename T>
int dyn_arr<T>::iterator::operator==(iterator& other) const { return current == other.current; }

template<typename T>
dyn_arr<T>::const_iterator::const_iterator(const T* ptr) : current(ptr) {}

template<typename T>
typename dyn_arr<T>::const_iterator& dyn_arr<T>::const_iterator::operator++() {
    ++current;
    return *this;
}

template<typename T>
typename dyn_arr<T>::const_iterator dyn_arr<T>::const_iterator::operator++(int) {
    const_iterator tmp = *this;
    ++current;
    return tmp;
}

template<typename T>
const T& dyn_arr<T>::const_iterator::operator*() const { return *current; }

template<typename T>
int dyn_arr<T>::const_iterator::operator!=(const const_iterator& other) const { return current != other.current; }

template<typename T>
int dyn_arr<T>::const_iterator::operator==(const const_iterator& other) const { return current == other.current; }

template<typename T>
dyn_arr<T>::dyn_arr() : data(nullptr), length(0) {}

template<typename T>
 dyn_arr<T>::dyn_arr(unsigned initial_size) : data(new T[initial_size]), length(initial_size) {
    if (data == nullptr) THROW(ERR_MEMORY);
}

template<typename T>
dyn_arr<T>::dyn_arr(const T* items, unsigned initial_size) : data(new T[initial_size]), length(initial_size) {
    if (data == nullptr) THROW(ERR_MEMORY);
    if (items == nullptr && initial_size > 0) THROW(ERR_NULL);
    for (unsigned i = 0; i < length; ++i)
        data[i] = items[i];
}

template<typename T>
dyn_arr<T>::dyn_arr(const dyn_arr& other) : data(new T[other.length]), length(other.length) {
    if (data == nullptr) THROW(ERR_MEMORY);
    for (unsigned i = 0; i < length; ++i)
        data[i] = other.data[i];
}

template<typename T>
dyn_arr<T>::~dyn_arr() { delete[] data; }

template<typename T>
T& dyn_arr<T>::operator[](unsigned index) {
    if (index >= length) THROW(ERR_INCORRECT_INDEX);
    return data[index];
}

template<typename T>
const T& dyn_arr<T>::operator[](unsigned index) const {
    if (index >= length) THROW(ERR_INCORRECT_INDEX);
    return data[index];
}

template<typename T>
typename dyn_arr<T>::iterator dyn_arr<T>::begin() { return iterator(data); }

template<typename T>
typename dyn_arr<T>::const_iterator dyn_arr<T>::begin() const { return const_iterator(data); }

template<typename T>
typename dyn_arr<T>::iterator dyn_arr<T>::end() { return iterator(data + length); }

template<typename T>
typename dyn_arr<T>::const_iterator dyn_arr<T>::end() const { return const_iterator(data + length); }


template<typename T>
unsigned dyn_arr<T>::size() const { return length; }

template<typename T>
dyn_arr<T>& dyn_arr<T>::operator=(dyn_arr other) {
    std::swap(data, other.data);
    std::swap(length, other.length);
    return *this;
}

template<typename T>
void dyn_arr<T>::resize(unsigned new_size) {
    T* new_data = new T[new_size];
    if (new_data == nullptr) THROW(ERR_MEMORY);
    unsigned copy_size = (new_size < length) ? new_size : length;
    for (unsigned i = 0; i < copy_size; ++i)
        new_data[i] = data[i];
    delete[] data;
    data = new_data;
    length = new_size;
}