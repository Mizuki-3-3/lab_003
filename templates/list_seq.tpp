#include "list_seq.hpp"
#include "errors.hpp"
#include "sequence.hpp"
#include <initializer_list>

template<typename T>
list_seq<T>::list_seq() : list(new forward_list<T>()) {
    if (list == nullptr) throw null_ptr();
}
template<typename T>
list_seq<T>::list_seq(size_t initial_size) : list(new forward_list<T>(initial_size)) {
    if (list == nullptr) throw null_ptr();
}

template<typename T>
list_seq<T>::list_seq(const forward_list<T>& other) : list(new forward_list<T>(other)) {
    if (list == nullptr) throw null_ptr();
}

template<typename T>
list_seq<T>::list_seq(const T* items, size_t count) : list(new forward_list<T>(items, count)) {
    if (list == nullptr) throw null_ptr();
}

template<typename T>
list_seq<T>::list_seq(const list_seq& other) : list(new forward_list<T>(*other.list)) {
    if (list == nullptr) throw null_ptr();
}

template<typename T>
list_seq<T>::list_seq(const list_seq&& move) {
    list = std::move(move.list);
}

template<typename T>
list_seq<T>::list_seq(const std::initializer_list<T> initial_l): list(new forward_list<T>(initial_l)){}

template<typename T>
list_seq<T>::~list_seq() { delete list; }

template<typename T>
list_seq<T>& list_seq<T>::operator=(const list_seq& other) {
    if (this != &other) {
        list = other.list;
    }
    return *this;
}

template<typename T>
T list_seq<T>::front() const {
    return list->front();
}

template<typename T>
T list_seq<T>::back() const {
    return list->back();
}

template<typename T>
size_t list_seq<T>::size() const {
    return list->size();
}

template<typename T>
sequence<T>* list_seq<T>::push_back(const T& item) {
    list -> insert_after(list->before_end(), item);
    return this;
}

template<typename T>
sequence<T>* list_seq<T>::push_front(const T& item) {
    list -> insert_after(list->before_begin(),  item);
    return this;
}

template<typename T>
sequence<T>* list_seq<T>::insert(const T& item, size_t index) {
    if (index > list->size()) throw index_out_of_range();
    if (index == 0) return push_front(item);
    if (index == list->size()) return push_back(item);
    size_t new_size = list->size() + 1;
    forward_list<T>* new_list = new forward_list<T>(new_size);
    if (!new_list) throw null_ptr();
    for (size_t i = 0; i < index; ++i)
        (*new_list)[i] = (*list)[i];
    (*new_list)[index] = item;
    for (size_t i = index; i < list->size(); ++i)
        (*new_list)[i + 1] = (*list)[i];
    delete list;
    list = new_list;
    return this;
}

template<typename T>
sequence<T>* list_seq<T>::get_subsequence(size_t start, size_t end) const {
    if (start > end || end > size()) throw index_out_of_range();
    size_t sub_size = end - start;
    forward_list<T>* sub_list = new forward_list<T>(sub_size);
    if (!sub_list) throw null_ptr();
    for (size_t i = start; i < end; ++i)
        (*sub_list)[i - start] = (*list)[i];
    list_seq<T>* result = new list_seq<T>(*sub_list);
    delete sub_list;
    return result;
}

template<typename T>
auto list_seq<T>::insert(const_iterator place, const T& item) -> iterator{
    if (place == end()){this->push_back(item); return end();}
    if (place == begin()){this->push_front(item); return begin();}
    const_iterator prev = begin();
    while (prev + 1 != place && prev != end()){
        prev++;
        if (prev + 1 == place){
            list->insert_after(prev, item);
            return iterator(prev+1);
        }
    }
    throw iterator_out_of_range();
}


template<typename T>
T& list_seq<T>::operator[](size_t index) {
    return (*list)[index];
}

template<typename T>
const T& list_seq<T>::operator[](size_t index) const {
    return (*list)[index];
}

template<typename T>
template <typename Func>
sequence<T>* list_seq<T>::map(Func f) {
    for (auto& x : *list) x = f(x);
    return this;
}

template<typename T>
template <typename Func>
sequence<T>* list_seq<T>::where(Func f) {
    size_t count = 0;
    for (size_t i = 0; i < list->size(); ++i)
        if (f((*list)[i])) ++count;
    forward_list<T>* new_list = new forward_list<T>(count);
    if (!new_list) throw null_ptr();
    size_t j = 0;
    for (size_t i = 0; i < list->size(); ++i) {
        if (f((*list)[i])) {
            (*new_list)[j++] = (*list)[i];
        }
    }
    delete list;
    list = new_list;
    return this;
}

template<typename T>
template <typename Func, typename U>
U list_seq<T>::reduce(Func f, U initial) const {
    if (list->size() == 0) {return initial;}
    U acc = f(initial, (*list)[0]);
    for (size_t i = 1; i < list->size(); ++i) {
        acc = f(acc, (*list)[i]);
    }
    return acc;
}

template<typename T>
sequence<T>* list_seq<T>::concat(sequence<T>* other) {
    if (!other) throw null_ptr();
    size_t new_size = list->size() + other->size();
    forward_list<T>* new_list = new forward_list<T>(new_size);
    if (!new_list) throw null_ptr();
    for (size_t i = 0; i < list->size(); ++i)
        (*new_list)[i] = (*list)[i];
    for (size_t i = 0; i < other->size(); ++i)
        (*new_list)[list->size() + i] = (*other)[i];
    delete list;
    list = new_list;
    return this;
}