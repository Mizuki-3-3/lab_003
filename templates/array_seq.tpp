#include "array_seq.hpp"
#include "errors.hpp"

template<Mutability M, typename T>
array_seq<M,T>::array_seq() : arr(new dyn_arr<T>()) {}

template<Mutability M, typename T>
array_seq<M,T>::array_seq(unsigned initial_size) : arr(new dyn_arr<T>(initial_size)) {
    if (arr == nullptr) THROW(ERR_MEMORY);
}

template<Mutability M, typename T>
array_seq<M,T>::array_seq(const T* items, unsigned count) : arr(new dyn_arr<T>(items, count)) {
    if (arr == nullptr) THROW(ERR_MEMORY);
}

template<Mutability M, typename T>
array_seq<M,T>::array_seq(const array_seq& other) : arr(new dyn_arr<T>(*other.arr)) {
    if (arr == nullptr) THROW(ERR_MEMORY);
}

template<Mutability M, typename T>
array_seq<M,T>::array_seq(const dyn_arr<T>& other) : arr(new dyn_arr<T>(other)) {
    if (arr == nullptr) THROW(ERR_MEMORY);
}

template<Mutability M, typename T>
array_seq<M,T>::~array_seq() {
    delete arr;
}

template<Mutability M, typename T>
array_seq<M,T>& array_seq<M,T>::operator=(const array_seq& other) {
    if (this != &other) {
        dyn_arr<T>* new_arr = new dyn_arr<T>(*other.arr);
        if (new_arr == nullptr) THROW(ERR_MEMORY);
        delete arr;
        arr = new_arr;
    }
    return *this;
}

template<Mutability M, typename T>
T array_seq<M,T>::get_first() const {
    if (size() == 0) THROW(ERR_INCORRECT_INDEX);
    return (*arr)[0];
}

template<Mutability M, typename T>
T array_seq<M,T>::get_last() const {
    if (size() == 0) THROW(ERR_INCORRECT_INDEX);
    return (*arr)[size() - 1];
}

template<Mutability M, typename T>
unsigned array_seq<M,T>::size() const {
    return arr->size();
}

template<Mutability M, typename T>
sequence<T>* array_seq<M,T>::append(const T& item) {
    if constexpr (M == Mutability::Mutable) {
        arr->resize(arr->size() + 1);
        (*arr)[arr->size() - 1] = item;
        return this;
    } else {
        array_seq<Mutability::Immutable, T>* new_seq =
            new array_seq<Mutability::Immutable, T>(arr->size() + 1);
        if (!new_seq) THROW(ERR_MEMORY);
        for (unsigned i = 0; i<arr->size(); i++){
            (*new_seq)[i] = (*arr)[i];
        }
        (*new_seq)[arr->size()] = item;
        return new_seq;
    }
}

template<Mutability M, typename T>
sequence<T>* array_seq<M,T>::prepend(const T& item) {
    if constexpr (M == Mutability::Mutable) {
        arr->resize(arr->size() + 1);
        for (unsigned i = arr->size() - 1; i > 0; --i)
            (*arr)[i] = (*arr)[i - 1];
        (*arr)[0] = item;
        return this;
    } else {
        dyn_arr<T>* new_arr = new dyn_arr<T>(arr->size() + 1);
        if (!new_arr) THROW(ERR_MEMORY);
        (*new_arr)[0] = item;
        for (unsigned i = 0; i < arr->size(); ++i)
            (*new_arr)[i + 1] = (*arr)[i];
        array_seq<Mutability::Immutable, T>* result =
            new array_seq<Mutability::Immutable, T>(*new_arr);
        if (!result) THROW(ERR_MEMORY);
        delete new_arr;
        return result;
    }
}

template<Mutability M, typename T>
sequence<T>* array_seq<M,T>::insert(const T& item, unsigned index) {
    if (index > arr->size()) THROW(ERR_INCORRECT_INDEX);
    if (index == 0) return prepend(item);
    if (index == arr->size()) return append(item);

    if constexpr (M == Mutability::Mutable) {
        arr->resize(arr->size() + 1);
        for (unsigned i = arr->size() - 1; i > index; --i)
            (*arr)[i] = (*arr)[i - 1];
        (*arr)[index] = item;
        return this;
    } else {
        unsigned ns = arr->size() + 1;
        dyn_arr<T>* new_arr = new dyn_arr<T>(ns);
        if (new_arr == nullptr) THROW(ERR_MEMORY);
        for (unsigned i = 0; i < index; ++i)
            (*new_arr)[i] = (*arr)[i];
        (*new_arr)[index] = item;
        for (unsigned i = index; i < arr->size(); ++i)
            (*new_arr)[i + 1] = (*arr)[i];
        array_seq<Mutability::Immutable, T>* result =
            new array_seq<Mutability::Immutable, T>(*new_arr);
        if (result == nullptr) THROW(ERR_MEMORY);
        delete new_arr;
        return result;
    }
}

template<Mutability M, typename T>
sequence<T>* array_seq<M,T>::concat(sequence<T>* other) {
    if (other == nullptr) THROW(ERR_NULL);
    array_seq<Mutability::Immutable, T>* result =
        new array_seq<Mutability::Immutable, T>(*arr);
    if (result == nullptr) THROW(ERR_MEMORY);
    for (unsigned i = 0; i < other->size(); ++i) {
        result = static_cast<array_seq<Mutability::Immutable, T>*>(
            result->append((*other)[i])
        );
    }
    return result;
}

template<Mutability M, typename T>
sequence<T>* array_seq<M,T>::get_subsequence(unsigned start, unsigned end) const {
    if (start > end || end > size()) THROW(ERR_INCORRECT_INDEX);
    array_seq<Mutability::Immutable, T>* sub =
        new array_seq<Mutability::Immutable, T>();
    if (sub == nullptr) THROW(ERR_MEMORY);
    for (unsigned i = start; i < end; ++i) {
        sub = static_cast<array_seq<Mutability::Immutable, T>*>(
            sub->append((*arr)[i])
        );
    }
    return sub;
}

template<Mutability M, typename T>
unsigned array_seq<M,T>::find(const T& value) const {
    for (unsigned i = 0; i < size(); ++i) {
        if ((*arr)[i] == value) return i;
    }
    THROW(ERR_INCORRECT_INDEX);
    return 0;
}

template<Mutability M, typename T>
T& array_seq<M,T>::operator[](unsigned index) {
    if (index >= size()) THROW(ERR_INCORRECT_INDEX);
    return (*arr)[index];
}

template<Mutability M, typename T>
const T& array_seq<M,T>::operator[](unsigned index) const {
    if (index >= size()) THROW(ERR_INCORRECT_INDEX);
    return (*arr)[index];
}

template<Mutability M, typename T>
template <typename Func>
array_seq<M,T>* array_seq<M,T>::map(Func f) {
    if constexpr (M == Mutability::Mutable) {
        for (auto& x : *arr) x = f(x);
        return this;
    } else {
        array_seq<Mutability::Immutable, T>* new_arr =
            new array_seq<Mutability::Immutable, T>(*arr);
        if (new_arr == nullptr) THROW(ERR_MEMORY);
        for (auto& x : *(new_arr->arr)) x = f(x);
        return new_arr;
    }
}

template<Mutability M, typename T>
template <typename Func>
array_seq<M,T>* array_seq<M,T>::where(Func f) {
    if constexpr (M == Mutability::Mutable) {
        unsigned wr_i = 0;
        for (unsigned r_i = 0; r_i < arr->size(); ++r_i) {
            if (f((*arr)[r_i])) {
                if (wr_i != r_i) (*arr)[wr_i] = (*arr)[r_i];
                ++wr_i;
            }
        }
        arr->resize(wr_i);
        return this;
    } else {
        dyn_arr<T>* new_data = new dyn_arr<T>(arr->size());
        if (!new_data) THROW(ERR_MEMORY);
        unsigned pos = 0;
        for (unsigned i = 0; i < arr->size(); ++i) {
            if (f((*arr)[i])) {
                (*new_data)[pos++] = (*arr)[i];
            }
        }
        array_seq<Mutability::Immutable, T>* result =
            new array_seq<Mutability::Immutable, T>(pos);
        if (!result) THROW(ERR_MEMORY);
        for (unsigned i = 0; i < pos; ++i){(*result)[i] = (*new_data)[i];}
        delete new_data;
        return result;
    }
}

template<Mutability M, typename T>
template <typename Func, typename U>
U array_seq<M,T>::reduce(Func f, U initial) const {
    if (arr->size() == 0) THROW(ERR_INCORRECT_INDEX);
    U acc = f(initial, (*arr)[0]);
    for (unsigned i = 1; i < arr->size(); ++i) {
        acc = f(acc, (*arr)[i]);
    }
    return acc;
}