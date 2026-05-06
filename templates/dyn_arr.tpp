#include "dyn_arr.hpp"
#include "errors.hpp"

//contiguous_iterator
template<typename T>
dyn_arr<T>::contiguous_iterator::contiguous_iterator(T* ptr): current(ptr){}

template<typename T>
T& dyn_arr<T>::contiguous_iterator::operator*(){return *current;}

template<typename T>
T* dyn_arr<T>::contiguous_iterator::operator->(){ return current;}

template<typename T>
dyn_arr<T>::contiguous_iterator& dyn_arr<T>::contiguous_iterator::operator++(){
    ++current;
    return *this;
}
        
template<typename T>
dyn_arr<T>::contiguous_iterator dyn_arr<T>::contiguous_iterator::operator++(int){
    contiguous_iterator tmp = *this;
    ++current;
    return tmp;
}
template<typename T>
dyn_arr<T>::contiguous_iterator& dyn_arr<T>::contiguous_iterator::operator--(){
    --current;
    return *this;
}
        
template<typename T>
dyn_arr<T>::contiguous_iterator dyn_arr<T>::contiguous_iterator::operator--(int){
    contiguous_iterator tmp = *this;
    --current;
    return tmp;
}

template<typename T>
bool dyn_arr<T>::contiguous_iterator::operator!=(contiguous_iterator& other) const{
    return current!=other.current;
}

template<typename T>
bool dyn_arr<T>::contiguous_iterator::operator==(contiguous_iterator& other) const{
    return current==other.current;
}

template<typename T>
dyn_arr<T>::contiguous_iterator& dyn_arr<T>::contiguous_iterator::operator+=(std::ptrdiff_t n){
    current += n;
    return *this;
}

template<typename T>
dyn_arr<T>::contiguous_iterator dyn_arr<T>::contiguous_iterator::operator+(std::ptrdiff_t n){
    return contiguous_iterator(current + n);
}

template<typename T>
dyn_arr<T>::contiguous_iterator& dyn_arr<T>::contiguous_iterator::operator-=(std::ptrdiff_t n){
    current -= n;
    return *this;
}
template<typename T>
dyn_arr<T>::contiguous_iterator dyn_arr<T>::contiguous_iterator::operator-(std::ptrdiff_t n){
    return contiguous_iterator(current - n);
}

template<typename T>
bool dyn_arr<T>::contiguous_iterator::operator==(const contiguous_iterator& other) const{return other.current == current;}

template<typename T>
bool dyn_arr<T>::contiguous_iterator::operator!=(const contiguous_iterator& other) const{return !(other==*this);}

template<typename T>
bool dyn_arr<T>::contiguous_iterator::operator< (const contiguous_iterator& other) const{return current < other.current;}
template<typename T>
bool dyn_arr<T>::contiguous_iterator::operator> (const contiguous_iterator& other) const{return current > other.current;}

template<typename T>
bool dyn_arr<T>::contiguous_iterator::operator<=(const contiguous_iterator& other) const{return !(*this > other);}

template<typename T>
bool dyn_arr<T>::contiguous_iterator::operator>=(const contiguous_iterator& other) const{return !(*this < other);}

//const_contiguous_iterator

template<typename T>
dyn_arr<T>::const_contiguous_iterator::const_contiguous_iterator(T* ptr): current(ptr){}

template<typename T>
T& dyn_arr<T>::const_contiguous_iterator::operator*(){return *current;}

template<typename T>
T* dyn_arr<T>::const_contiguous_iterator::operator->(){ return current;}

template<typename T>
dyn_arr<T>::const_contiguous_iterator& dyn_arr<T>::const_contiguous_iterator::operator++(){
    ++current;
    return *this;
}
        
template<typename T>
dyn_arr<T>::const_contiguous_iterator dyn_arr<T>::const_contiguous_iterator::operator++(int){
    const_contiguous_iterator tmp = *this;
    ++current;
    return tmp;
}
template<typename T>
dyn_arr<T>::const_contiguous_iterator& dyn_arr<T>::const_contiguous_iterator::operator--(){
    --current;
    return *this;
}
        
template<typename T>
dyn_arr<T>::const_contiguous_iterator dyn_arr<T>::const_contiguous_iterator::operator--(int){
    const_contiguous_iterator tmp = *this;
    --current;
    return tmp;
}

template<typename T>
bool dyn_arr<T>::const_contiguous_iterator::operator!=(const_contiguous_iterator& other) const{
    return current!=other.current;
}

template<typename T>
bool dyn_arr<T>::const_contiguous_iterator::operator==(const_contiguous_iterator& other) const{
    return current==other.current;
}

template<typename T>
dyn_arr<T>::const_contiguous_iterator& dyn_arr<T>::const_contiguous_iterator::operator+=(std::ptrdiff_t n){
    current += n;
    return *this;
}

template<typename T>
dyn_arr<T>::const_contiguous_iterator dyn_arr<T>::const_contiguous_iterator::operator+(std::ptrdiff_t n){
    return contiguous_iterator(current+n);
}

template<typename T>
dyn_arr<T>::const_contiguous_iterator& dyn_arr<T>::const_contiguous_iterator::operator-=(std::ptrdiff_t n){
    current -= n;
    return *this;
0}
template<typename T>
dyn_arr<T>::const_contiguous_iterator dyn_arr<T>::const_contiguous_iterator::operator-(std::ptrdiff_t n){
    return contiguous_iterator(current-n);
}

template<typename T>
bool dyn_arr<T>::const_contiguous_iterator::operator==(const const_contiguous_iterator& other) const{return other.current == current;}

template<typename T>
bool dyn_arr<T>::const_contiguous_iterator::operator!=(const const_contiguous_iterator& other) const{return !(other==*this);}

template<typename T>
bool dyn_arr<T>::const_contiguous_iterator::operator< (const const_contiguous_iterator& other) const{return current < other.current;}
template<typename T>
bool dyn_arr<T>::const_contiguous_iterator::operator> (const const_contiguous_iterator& other) const{return current > other.current;}

template<typename T>
bool dyn_arr<T>::const_contiguous_iterator::operator<=(const const_contiguous_iterator& other) const{return !(*this > other);}

template<typename T>
bool dyn_arr<T>::const_contiguous_iterator::operator>=(const const_contiguous_iterator& other) const{return !(*this < other);}

//dyn_arr
template<typename T>
dyn_arr<T>::dyn_arr() : data(nullptr), length(0) {}

template<typename T>
 dyn_arr<T>::dyn_arr(unsigned initial_size) : data(new T[initial_size]), length(initial_size) {
    if (data == nullptr) throw null_ptr();
}

template<typename T>
dyn_arr<T>::dyn_arr(const T* items, unsigned initial_size) : data(new T[initial_size]), length(initial_size) {
    if (data == nullptr) throw null_ptr();
    if (items == nullptr && initial_size > 0) throw null_ptr();
    for (unsigned i = 0; i < length; ++i)
        data[i] = items[i];
}

template<typename T>
dyn_arr<T>::dyn_arr(const dyn_arr& other) : data(new T[other.length]), length(other.length) {
    if (data == nullptr) throw null_ptr();
    for (unsigned i = 0; i < length; ++i)
        data[i] = other.data[i];
}

template<typename T>
dyn_arr<T>::~dyn_arr() { delete[] data; }

template<typename T>
T& dyn_arr<T>::operator[](unsigned index) {
    if (index >= length) throw index_out_of_range();
    return data[index];
}

template<typename T>
const T& dyn_arr<T>::operator[](unsigned index) const {
    if (index >= length) throw index_out_of_range();
    return data[index];
}

template<typename T>
typename dyn_arr<T>::contiguous_iterator dyn_arr<T>::begin() { return contiguous_iterator(data); }

template<typename T>
typename dyn_arr<T>::const_contiguous_iterator dyn_arr<T>::begin() const { return const_contiguous_iterator(data); }

template<typename T>
typename dyn_arr<T>::contiguous_iterator dyn_arr<T>::end() { return contiguous_iterator(data + length); }

template<typename T>
typename dyn_arr<T>::const_contiguous_iterator dyn_arr<T>::end() const { return const_contiguous_iterator(data + length); }


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
    if (new_data == nullptr) throw null_ptr();
    unsigned copy_size = (new_size < length) ? new_size : length;
    for (unsigned i = 0; i < copy_size; ++i)
        new_data[i] = data[i];
    delete[] data;
    data = new_data;
    length = new_size;
}