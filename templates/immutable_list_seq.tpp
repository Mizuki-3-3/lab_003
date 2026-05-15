#include "immutable_list_seq.hpp"
#include "errors.hpp"

template<typename T>
immutable_list_seq<T>::immutable_list_seq() : list(new forward_list<T>()) {
    if (list == nullptr) throw null_ptr();
}

template<typename T>
immutable_list_seq<T>::immutable_list_seq(const forward_list<T>& other) : list(new forward_list<T>(other)) {
    if (list == nullptr) throw null_ptr();
}

template<typename T>
immutable_list_seq<T>::immutable_list_seq(const T* items, size_t count) : list(new forward_list<T>(items, count)) {
    if (list == nullptr) throw null_ptr();
}

template<typename T>
immutable_list_seq<T>::immutable_list_seq(const immutable_list_seq& other) : list(new forward_list<T>(*other.list)) {
    if (list == nullptr) throw null_ptr();
}

template<typename T>
immutable_list_seq<T>::~immutable_list_seq() { delete list; }

template<typename T>
immutable_list_seq<T>& immutable_list_seq<T>::operator=(const immutable_list_seq& other) {
    if (this != &other) {
        forward_list<T>* new_list = new forward_list<T>(*other.list);
        if (new_list == nullptr) throw null_ptr();
        delete list;
        list = new_list;
    }
    return *this;
}

template<typename T>
immutable_list_seq<T>::immutable_list_seq(const immutable_list_seq&& move) {
    list = std::move(move.list);
}

template<typename T>
immutable_list_seq<T>::immutable_list_seq(const std::initializer_list<T> initial_l): list(new forward_list<T>(initial_l)){}


template<typename T>
T immutable_list_seq<T>::front() const {
    return list->get_first();
}

template<typename T>
T immutable_list_seq<T>::back() const {
    return list->get_last();
}

template<typename T>
size_t immutable_list_seq<T>::size() const {
    return list->size();
}

template<typename T>
sequence<T>* immutable_list_seq<T>::push_back(const T& item) {
    immutable_list_seq<T>* result = new immutable_list_seq<T>();
    result->list->insert_after(list->before_end(), item);
    return result;
}

template<typename T>
sequence<T>* immutable_list_seq<T>::push_front(const T& item) {
    immutable_list_seq<T>* result = new immutable_list_seq<T>();
    result->list->insert_after(list->before_begin(), item);
    return result;
}

template<typename T>
sequence<T>* immutable_list_seq<T>::insert(const T& item, size_t index) {
    immutable_list_seq<T>* result = new immutable_list_seq<T>();
    result->list->insert_after(list->before_begin() + index, item);
    return result;
}

template<typename T>
immutable_list_seq<T>* immutable_list_seq<T>::insert(iterator place, const T& item) {
    immutable_list_seq<T>* result = new immutable_list_seq<T>(*this);
    if (place == end()){return this->push_back(item);}
    if (place == begin()){return this->push_front(item);}
    iterator curr = result->begin();
    while (curr + 1 != place && curr != end()){
        curr++;
        if (curr + 1 == place){
            result->list->insert_after(curr, item);
            return result;
        }
    }
    throw place_out_of_range();
}

template<typename T>
sequence<T>* immutable_list_seq<T>::get_subsequence(size_t start, size_t end) const {
    if (start > end || end > size()) throw index_out_of_range();
    size_t sub_size = end - start;
    forward_list<T>* sub_list = new forward_list<T>(sub_size);
    if (!sub_list) throw null_ptr();
    for (size_t i = start; i < end; ++i)
        (*sub_list)[i - start] = (*list)[i];
    immutable_list_seq<T>* result = new immutable_list_seq<T>(*sub_list);
    delete sub_list;
    return result;
}

template<typename T>
auto immutable_list_seq<T>::find(const T& value) const -> immutable_list_seq<T>::const_iterator {
    for (auto it = list->begin(); it != list->end(); it++) {
        if (*it == value) return it;
    }
    throw not_found();
}

template<typename T>
auto immutable_list_seq<T>::find(const T& value) -> immutable_list_seq<T>::iterator {
    for (auto it = list->begin(); it != list->end(); it++) {
        if (*it == value) return it;
    }
    throw not_found();
}

template<typename T>
T& immutable_list_seq<T>::operator[](size_t index) {
    return (*list)[index];
}

template<typename T>
const T& immutable_list_seq<T>::operator[](size_t index) const {
    return (*list)[index];
}

template<typename T>
template <typename Func>
sequence<T>* immutable_list_seq<T>::map(Func f) {
    immutable_list_seq<T>* new_l = new immutable_list_seq<T>(*list);
    if (!new_l) throw null_ptr();
    for (auto& x : *(new_l->list)) x = f(x);
    return new_l;
}

template<typename T>
template <typename Func>
sequence<T>* immutable_list_seq<T>::where(Func f) {
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
    immutable_list_seq<T>* result = new immutable_list_seq<T>(*new_list);
    delete new_list;
    return result;
}

template<typename T>
template <typename Func, typename U>
U immutable_list_seq<T>::reduce(Func f, U initial) const {
    if (list->size() == 0) throw empty_container();
    U acc = f(initial, (*list)[0]);
    for (size_t i = 1; i < list->size(); ++i) {
        acc = f(acc, (*list)[i]);
    }
    return acc;
}

template<typename T>
sequence<T>* immutable_list_seq<T>::concat(sequence<T>* other) {
    if (!other) throw null_ptr();
    size_t new_size = list->size() + other->size();
    forward_list<T>* new_list = new forward_list<T>(new_size);
    if (!new_list) throw null_ptr();
    for (size_t i = 0; i < list->size(); ++i)
        (*new_list)[i] = (*list)[i];
    for (size_t i = 0; i < other->size(); ++i)
        (*new_list)[list->size() + i] = (*other)[i];
    immutable_list_seq<T>* result = new immutable_list_seq<T>(*new_list);
    delete new_list;
    return result;
}