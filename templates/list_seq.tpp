#include "list_seq.hpp"
#include "errors.hpp"

template<typename T, Mutability M>
list_seq<T, M>::list_seq() : list(new s_list<T>()) {
    if (list == nullptr) throw null_ptr();
}

template<typename T, Mutability M>
list_seq<T, M>::list_seq(const s_list<T>& other) : list(new s_list<T>(other)) {
    if (list == nullptr) throw null_ptr();
}

template<typename T, Mutability M>
list_seq<T, M>::list_seq(const T* items, unsigned count) : list(new s_list<T>(items, count)) {
    if (list == nullptr) throw null_ptr();
}

template<typename T, Mutability M>
list_seq<T, M>::list_seq(const list_seq& other) : list(new s_list<T>(*other.list)) {
    if (list == nullptr) throw null_ptr();
}

template<typename T, Mutability M>
list_seq<T, M>::~list_seq() { delete list; }

template<typename T, Mutability M>
list_seq<T, M>& list_seq<T, M>::operator=(const list_seq& other) {
    if (this != &other) {
        s_list<T>* new_list = new s_list<T>(*other.list);
        if (new_list == nullptr) throw null_ptr();
        delete list;
        list = new_list;
    }
    return *this;
}

template<typename T, Mutability M>
T list_seq<T, M>::get_first() const {
    return list->get_first();
}

template<typename T, Mutability M>
T list_seq<T, M>::get_last() const {
    return list->get_last();
}

template<typename T, Mutability M>
unsigned list_seq<T, M>::size() const {
    return list->size();
}

template<typename T, Mutability M>
sequence<T>* list_seq<T, M>::append(const T& item) {
    unsigned new_size = list->size() + 1;
    s_list<T>* new_list = new s_list<T>(new_size);
    
    if (!new_list) throw null_ptr();

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

template<typename T, Mutability M>
sequence<T>* list_seq<T, M>::prepend(const T& item) {
    unsigned new_size = list->size() + 1;
    s_list<T>* new_list = new s_list<T>(new_size);

    if (!new_list) throw null_ptr();

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

template<typename T, Mutability M>
sequence<T>* list_seq<T, M>::insert(const T& item, unsigned index) {
    if (index > list->size()) throw index_out_of_range();
    if (index == 0) return prepend(item);
    if (index == list->size()) return append(item);

    unsigned new_size = list->size() + 1;
    s_list<T>* new_list = new s_list<T>(new_size);
    if (!new_list) throw null_ptr();

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

template<typename T, Mutability M>
sequence<T>* list_seq<T, M>::get_subsequence(unsigned start, unsigned end) const {
    if (start > end || end > size()) throw index_out_of_range();
    unsigned sub_size = end - start;
    s_list<T>* sub_list = new s_list<T>(sub_size);
    if (!sub_list) throw null_ptr();
    for (unsigned i = start; i < end; ++i)
        (*sub_list)[i - start] = (*list)[i];
    return new list_seq<Mutability::Immutable, T>(*sub_list);
}

template<typename T, Mutability M>
unsigned list_seq<T, M>::find(const T& value) const {
    unsigned idx = 0;
    for (auto it = list->begin(); it != list->end(); ++it, ++idx) {
        if (*it == value) return idx;
    }
    throw invalid_argument();
    return 0;
}

template<typename T, Mutability M>
T& list_seq<T, M>::operator[](unsigned index) {
    return (*list)[index];
}

template<typename T, Mutability M>
const T& list_seq<T, M>::operator[](unsigned index) const {
    return (*list)[index];
}

template<typename T, Mutability M>
template <typename Func>
sequence<T>* list_seq<T, M>::map(Func f) {
    if constexpr(M == Mutability::Mutable) {
        for (auto& x : *list) x = f(x);
        return this;
    } else {
        list_seq<Mutability::Immutable, T>* new_l =
            new list_seq<Mutability::Immutable, T>(*list);
        if (!new_l) throw null_ptr();
        for (auto& x : *new_l) x = f(x);
        return new_l;
    }
}

template<typename T, Mutability M>
template <typename Func>
sequence<T>* list_seq<T, M>::where(Func f) {
    unsigned count = 0;
    for (unsigned i = 0; i < list->size(); ++i)
        if (f((*list)[i])) ++count;

    s_list<T>* new_list = new s_list<T>(count);
    if (!new_list) throw null_ptr();

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
template<typename T, Mutability M>
template <typename Func, typename U>
U list_seq<T, M>::reduce(Func f, U initial) const {
    if (list->size() == 0) throw empty_container();
    U acc = f(initial, (*list)[0]);
    for (unsigned i = 1; i < list->size(); ++i) {
        acc = f(acc, (*list)[i]);
    }
    return acc;
}

template<typename T, Mutability M>
sequence<T>* list_seq<T, M>::concat(sequence<T>* other) {
    if (!other) throw null_ptr();
    unsigned new_size = list->size() + other->size();
    s_list<T>* new_list = new s_list<T>(new_size);
    if (!new_list) throw null_ptr();

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