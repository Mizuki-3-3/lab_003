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
immutable_list_seq<T>::immutable_list_seq(const T* items, unsigned count) : list(new forward_list<T>(items, count)) {
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
T immutable_list_seq<T>::get_first() const {
    return list->get_first();
}

template<typename T>
T immutable_list_seq<T>::get_last() const {
    return list->get_last();
}

template<typename T>
unsigned immutable_list_seq<T>::size() const {
    return list->size();
}

template<typename T>
sequence<T>* immutable_list_seq<T>::append(const T& item) {
    unsigned new_size = list->size() + 1;
    forward_list<T>* new_list = new forward_list<T>(new_size);
    if (!new_list) throw null_ptr();
    for (unsigned i = 0; i < list->size(); ++i)
        (*new_list)[i] = (*list)[i];
    (*new_list)[list->size()] = item;
    immutable_list_seq<T>* result = new immutable_list_seq<T>(*new_list);
    delete new_list;
    return result;
}

template<typename T>
sequence<T>* immutable_list_seq<T>::prepend(const T& item) {
    unsigned new_size = list->size() + 1;
    forward_list<T>* new_list = new forward_list<T>(new_size);
    if (!new_list) throw null_ptr();
    (*new_list)[0] = item;
    for (unsigned i = 0; i < list->size(); ++i)
        (*new_list)[i + 1] = (*list)[i];
    immutable_list_seq<T>* result = new immutable_list_seq<T>(*new_list);
    delete new_list;
    return result;
}

template<typename T>
sequence<T>* immutable_list_seq<T>::insert(const T& item, unsigned index) {
    if (index > list->size()) throw index_out_of_range();
    if (index == 0) return prepend(item);
    if (index == list->size()) return append(item);
    unsigned new_size = list->size() + 1;
    forward_list<T>* new_list = new forward_list<T>(new_size);
    if (!new_list) throw null_ptr();
    for (unsigned i = 0; i < index; ++i)
        (*new_list)[i] = (*list)[i];
    (*new_list)[index] = item;
    for (unsigned i = index; i < list->size(); ++i)
        (*new_list)[i + 1] = (*list)[i];
    immutable_list_seq<T>* result = new immutable_list_seq<T>(*new_list);
    delete new_list;
    return result;
}

template<typename T>
sequence<T>* immutable_list_seq<T>::get_subsequence(unsigned start, unsigned end) const {
    if (start > end || end > size()) throw index_out_of_range();
    unsigned sub_size = end - start;
    forward_list<T>* sub_list = new forward_list<T>(sub_size);
    if (!sub_list) throw null_ptr();
    for (unsigned i = start; i < end; ++i)
        (*sub_list)[i - start] = (*list)[i];
    immutable_list_seq<T>* result = new immutable_list_seq<T>(*sub_list);
    delete sub_list;
    return result;
}

template<typename T>
unsigned immutable_list_seq<T>::find(const T& value) const {
    unsigned idx = 0;
    for (auto it = list->begin(); it != list->end(); ++it, ++idx) {
        if (*it == value) return idx;
    }
    throw invalid_argument();
}

template<typename T>
T& immutable_list_seq<T>::operator[](unsigned index) {
    return (*list)[index];
}

template<typename T>
const T& immutable_list_seq<T>::operator[](unsigned index) const {
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
    unsigned count = 0;
    for (unsigned i = 0; i < list->size(); ++i)
        if (f((*list)[i])) ++count;
    forward_list<T>* new_list = new forward_list<T>(count);
    if (!new_list) throw null_ptr();
    unsigned j = 0;
    for (unsigned i = 0; i < list->size(); ++i) {
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
    for (unsigned i = 1; i < list->size(); ++i) {
        acc = f(acc, (*list)[i]);
    }
    return acc;
}

template<typename T>
sequence<T>* immutable_list_seq<T>::concat(sequence<T>* other) {
    if (!other) throw null_ptr();
    unsigned new_size = list->size() + other->size();
    forward_list<T>* new_list = new forward_list<T>(new_size);
    if (!new_list) throw null_ptr();
    for (unsigned i = 0; i < list->size(); ++i)
        (*new_list)[i] = (*list)[i];
    for (unsigned i = 0; i < other->size(); ++i)
        (*new_list)[list->size() + i] = (*other)[i];
    immutable_list_seq<T>* result = new immutable_list_seq<T>(*new_list);
    delete new_list;
    return result;
}