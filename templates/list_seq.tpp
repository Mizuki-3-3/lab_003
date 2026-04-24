#include "list_seq.hpp"
#include "errors.hpp"

template<Mutability M, typename T>
list_seq<M,T>::list_seq() : list(new s_list<T>()) {
    if (list == nullptr) THROW(ERR_MEMORY);
}

template<Mutability M, typename T>
list_seq<M,T>::list_seq(const s_list<T>& other) : list(new s_list<T>(other)) {
    if (list == nullptr) THROW(ERR_MEMORY);
}

template<Mutability M, typename T>
list_seq<M,T>::list_seq(const T* items, unsigned count) : list(new s_list<T>(items, count)) {
    if (list == nullptr) THROW(ERR_MEMORY);
}

template<Mutability M, typename T>
list_seq<M,T>::list_seq(const list_seq& other) : list(new s_list<T>(*other.list)) {
    if (list == nullptr) THROW(ERR_MEMORY);
}

template<Mutability M, typename T>
list_seq<M,T>::~list_seq() { delete list; }

template<Mutability M, typename T>
list_seq<M,T>& list_seq<M,T>::operator=(const list_seq& other) {
    if (this != &other) {
        s_list<T>* new_list = new s_list<T>(*other.list);
        if (new_list == nullptr) THROW(ERR_MEMORY);
        delete list;
        list = new_list;
    }
    return *this;
}

template<Mutability M, typename T>
T list_seq<M,T>::get_first() const {
    return list->get_first();
}

template<Mutability M, typename T>
T list_seq<M,T>::get_last() const {
    return list->get_last();
}

template<Mutability M, typename T>
unsigned list_seq<M,T>::size() const {
    return list->size();
}

template<Mutability M, typename T>
sequence<T>* list_seq<M,T>::append(const T& item) {
    unsigned new_size = list->size() + 1;
    s_list<T>* new_list = new s_list<T>(new_size);
    
    if (!new_list) THROW(ERR_MEMORY);

    for (unsigned i = 0; i < list->size(); ++i)
        (*new_list)[i] = (*list)[i];
    (*new_list)[list->size()] = item;

    if constexpr (M == Mutability::Mutable) {
        delete list;
        list = new_list;
        return this;
    } else {
        return new list_seq<Mutability::Immutable, T>(*new_list);
    }
}

template<Mutability M, typename T>
sequence<T>* list_seq<M,T>::prepend(const T& item) {
    unsigned new_size = list->size() + 1;
    s_list<T>* new_list = new s_list<T>(new_size);

    if (!new_list) THROW(ERR_MEMORY);

    (*new_list)[0] = item;
    for (unsigned i = 0; i < list->size(); ++i)
        (*new_list)[i + 1] = (*list)[i];
    
    if constexpr(M == Mutability::Mutable) {
        delete list;
        list = new_list;
        return this;
    } else {
        return new list_seq<Mutability::Immutable, T>(*new_list);
    }
}

template<Mutability M, typename T>
sequence<T>* list_seq<M,T>::insert(const T& item, unsigned index) {
    if (index > list->size()) THROW(ERR_INCORRECT_INDEX);
    if (index == 0) return prepend(item);
    if (index == list->size()) return append(item);

    unsigned new_size = list->size() + 1;
    s_list<T>* new_list = new s_list<T>(new_size);
    if (!new_list) THROW(ERR_MEMORY);

    for (unsigned i = 0; i < index; ++i)
        (*new_list)[i] = (*list)[i];
    (*new_list)[index] = item;
    for (unsigned i = index; i < list->size(); ++i)
        (*new_list)[i + 1] = (*list)[i];

    if constexpr(M == Mutability::Mutable) {
        delete list;
        list = new_list;
        return this;
    } else {
        return new list_seq<Mutability::Immutable, T>(*new_list);
    }
}

template<Mutability M, typename T>
sequence<T>* list_seq<M,T>::get_subsequence(unsigned start, unsigned end) const {
    if (start > end || end > size()) THROW(ERR_INCORRECT_INDEX);
    unsigned sub_size = end - start;
    s_list<T>* sub_list = new s_list<T>(sub_size);
    if (!sub_list) THROW(ERR_MEMORY);
    for (unsigned i = start; i < end; ++i)
        (*sub_list)[i - start] = (*list)[i];
    return new list_seq<Mutability::Immutable, T>(*sub_list);
}

template<Mutability M, typename T>
unsigned list_seq<M,T>::find(const T& value) const {
    unsigned idx = 0;
    for (auto it = list->begin(); it != list->end(); ++it, ++idx) {
        if (*it == value) return idx;
    }
    THROW(ERR_INCORRECT_INDEX);
    return 0;
}

template<Mutability M, typename T>
T& list_seq<M,T>::operator[](unsigned index) {
    return (*list)[index];
}

template<Mutability M, typename T>
const T& list_seq<M,T>::operator[](unsigned index) const {
    return (*list)[index];
}

template<Mutability M, typename T>
template <typename Func>
list_seq<M,T>* list_seq<M,T>::map(Func f) {
    if constexpr(M == Mutability::Mutable) {
        for (auto& x : *list) x = f(x);
        return this;
    } else {
        list_seq<Mutability::Immutable, T>* new_l =
            new list_seq<Mutability::Immutable, T>(*list);
        if (!new_l) THROW(ERR_MEMORY);
        for (auto& x : *new_l) x = f(x);
        return new_l;
    }
}

template<Mutability M, typename T>
template <typename Func>
list_seq<M,T>* list_seq<M,T>::where(Func f) {
    unsigned count = 0;
    for (unsigned i = 0; i < list->size(); ++i)
        if (f((*list)[i])) ++count;

    s_list<T>* new_list = new s_list<T>(count);
    if (!new_list) THROW(ERR_MEMORY);

    unsigned j = 0;
    for (unsigned i = 0; i < list->size(); ++i) {
        if (f((*list)[i])) {
            (*new_list)[j++] = (*list)[i];
        }
    }

    if constexpr(M == Mutability::Mutable) {
        delete list;
        list = new_list;
        return this;
    } else {
        return new list_seq<Mutability::Immutable, T>(*new_list);
    }
}
template<Mutability M, typename T>
template <typename Func, typename U>
U list_seq<M,T>::reduce(Func f, U initial) const {
    if (list->size() == 0) THROW(ERR_INCORRECT_INDEX);
    U acc = f(initial, (*list)[0]);
    for (unsigned i = 1; i < list->size(); ++i) {
        acc = f(acc, (*list)[i]);
    }
    return acc;
}

template<Mutability M, typename T>
sequence<T>* list_seq<M,T>::concat(sequence<T>* other) {
    if (!other) THROW(ERR_NULL);
    unsigned new_size = list->size() + other->size();
    s_list<T>* new_list = new s_list<T>(new_size);
    if (!new_list) THROW(ERR_MEMORY);

    for (unsigned i = 0; i < list->size(); ++i)
        (*new_list)[i] = (*list)[i];
    for (unsigned i = 0; i < other->size(); ++i)
        (*new_list)[list->size() + i] = (*other)[i];

    if constexpr(M == Mutability::Immutable){
        list_seq<Mutability::Immutable, T>* result = new list_seq<Mutability::Immutable, T>(*new_list);
        delete new_list;
        return result;
    }else{
        delete list;
        list = new_list;
        return this;
    }
}