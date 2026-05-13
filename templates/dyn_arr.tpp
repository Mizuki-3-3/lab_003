#include "dyn_arr.hpp"
#include "errors.hpp"

//iterator
template<typename T>
dyn_arr<T>::iterator::iterator(T* ptr): current(ptr){}

template<typename T>
T& dyn_arr<T>::iterator::operator*(){return *current;}

template<typename T>
T* dyn_arr<T>::iterator::operator->(){ return current;}

template<typename T>
auto dyn_arr<T>::iterator::operator++() -> dyn_arr<T>::iterator&{
    ++current;
    return *this;
}
        
template<typename T>
auto dyn_arr<T>::iterator::operator++(int) -> dyn_arr<T>::iterator{
   iterator tmp = *this;
    ++current;
    return tmp;
}
template<typename T>
auto dyn_arr<T>::iterator::operator--() -> dyn_arr<T>::iterator&{
    --current;
    return *this;
}
        
template<typename T>
auto dyn_arr<T>::iterator::operator--(int) -> dyn_arr<T>::iterator{
   iterator tmp = *this;
    --current;
    return tmp;
}

template<typename T>
bool dyn_arr<T>::iterator::operator!=(iterator& other) const{
    return current!=other.current;
}

template<typename T>
bool dyn_arr<T>::iterator::operator==(iterator& other) const{
    return current==other.current;
}

template<typename T>
auto dyn_arr<T>::iterator::operator+=(std::ptrdiff_t n) -> dyn_arr<T>::iterator&{
    current += n;
    return *this;
}

template<typename T>
auto dyn_arr<T>::iterator::operator+(std::ptrdiff_t n) -> dyn_arr<T>::iterator{
    returniterator(current + n);
}

template<typename T>
auto dyn_arr<T>::iterator::operator-=(std::ptrdiff_t n) -> dyn_arr<T>::iterator&{
    current -= n;
    return *this;
}
template<typename T>
auto dyn_arr<T>::iterator::operator-(std::ptrdiff_t n) -> dyn_arr<T>::iterator{
    returniterator(current - n);
}

template<typename T>
bool dyn_arr<T>::iterator::operator==(const iterator& other) const{return other.current == current;}

template<typename T>
bool dyn_arr<T>::iterator::operator!=(const iterator& other) const{return !(other==*this);}

template<typename T>
bool dyn_arr<T>::iterator::operator< (const iterator& other) const{return current < other.current;}
template<typename T>
bool dyn_arr<T>::iterator::operator> (const iterator& other) const{return current > other.current;}

template<typename T>
bool dyn_arr<T>::iterator::operator<=(const iterator& other) const{return !(*this > other);}

template<typename T>
bool dyn_arr<T>::iterator::operator>=(const iterator& other) const{return !(*this < other);}

//const_iterator

template<typename T>
dyn_arr<T>::const_iterator::const_iterator(T* ptr): current(ptr){}

template<typename T>
T& dyn_arr<T>::const_iterator::operator*(){return *current;}

template<typename T>
T* dyn_arr<T>::const_iterator::operator->(){ return current;}

template<typename T>
auto dyn_arr<T>::const_iterator::operator++() -> dyn_arr<T>::const_iterator&{
    ++current;
    return *this;
}
        
template<typename T>
auto dyn_arr<T>::const_iterator::operator++(int) -> dyn_arr<T>::const_iterator{
    const_iterator tmp = *this;
    ++current;
    return tmp;
}
template<typename T>
auto dyn_arr<T>::const_iterator::operator--() -> dyn_arr<T>::const_iterator&{
    --current;
    return *this;
}
        
template<typename T>
auto dyn_arr<T>::const_iterator::operator--(int) -> dyn_arr<T>::const_iterator{
    const_iterator tmp = *this;
    --current;
    return tmp;
}

template<typename T>
bool dyn_arr<T>::const_iterator::operator!=(const_iterator& other) const{
    return current!=other.current;
}

template<typename T>
bool dyn_arr<T>::const_iterator::operator==(const_iterator& other) const{
    return current==other.current;
}

template<typename T>
auto dyn_arr<T>::const_iterator::operator+=(std::ptrdiff_t n) -> dyn_arr<T>::const_iterator&{
    current += n;
    return *this;
}

template<typename T>
auto dyn_arr<T>::const_iterator::operator+(std::ptrdiff_t n) -> dyn_arr<T>::const_iterator{
    returniterator(current+n);
}

template<typename T>
auto dyn_arr<T>::const_iterator::operator-=(std::ptrdiff_t n) -> dyn_arr<T>::const_iterator&{
    current -= n;
    return *this;
}
template<typename T>
auto dyn_arr<T>::const_iterator::operator-(std::ptrdiff_t n)-> dyn_arr<T>::const_iterator{
    returniterator(current-n);
}

template<typename T>
bool dyn_arr<T>::const_iterator::operator==(const const_iterator& other) const{return other.current == current;}

template<typename T>
bool dyn_arr<T>::const_iterator::operator!=(const const_iterator& other) const{return !(other==*this);}

template<typename T>
bool dyn_arr<T>::const_iterator::operator< (const const_iterator& other) const{return current < other.current;}
template<typename T>
bool dyn_arr<T>::const_iterator::operator> (const const_iterator& other) const{return current > other.current;}

template<typename T>
bool dyn_arr<T>::const_iterator::operator<=(const const_iterator& other) const{return !(*this > other);}

template<typename T>
bool dyn_arr<T>::const_iterator::operator>=(const const_iterator& other) const{return !(*this < other);}

//dyn_arr
template<typename T>
dyn_arr<T>::dyn_arr() : data(nullptr), length(0) {}

template<typename T>
 dyn_arr<T>::dyn_arr(size_t initial_size) : data(new T[initial_size]), length(initial_size) {
    if (data == nullptr) throw null_ptr();
}

template<typename T>
dyn_arr<T>::dyn_arr(const T* items, size_t initial_size) : data(new T[initial_size]), length(initial_size) {
    if (data == nullptr) throw null_ptr();
    if (items == nullptr && initial_size > 0) throw null_ptr();
    for (size_t i = 0; i < length; ++i)
        data[i] = items[i];
}

template<typename T>
dyn_arr<T>::dyn_arr(const dyn_arr& other) : data(new T[other.length]), length(other.length) {
    if (data == nullptr) throw null_ptr();
    for (size_t i = 0; i < length; ++i)
        data[i] = other.data[i];
}

template<typename T>
dyn_arr<T>::~dyn_arr() { delete[] data; }

template<typename T>
T& dyn_arr<T>::operator[](size_t index) {
    if (index >= length) throw index_out_of_range();
    return data[index];
}

template<typename T>
const T& dyn_arr<T>::operator[](size_t index) const {
    if (index >= length) throw index_out_of_range();
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
size_t dyn_arr<T>::size() const { return length; }

template<typename T>
dyn_arr<T>& dyn_arr<T>::operator=(dyn_arr other) {
    std::swap(data, other.data);
    std::swap(length, other.length);
    return *this;
}

template<typename T>
void dyn_arr<T>::resize(size_t new_size) {
    T* new_data = new T[new_size];
    if (new_data == nullptr) throw null_ptr();
    size_t copy_size = (new_size < length) ? new_size : length;
    for (size_t i = 0; i < copy_size; ++i)
        new_data[i] = data[i];
    delete[] data;
    data = new_data;
    length = new_size;
}