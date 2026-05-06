#include "array_seq.hpp"
#include "errors.hpp"

template<typename T, Mutability M>
array_seq<T, M>::array_seq(): arr(new dyn_arr<T>()) {}

template<typename T, Mutability M>
array_seq<T, M>::array_seq(unsigned initial_size) : arr(new dyn_arr<T>(initial_size)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T, Mutability M>
array_seq<T, M>::array_seq(const T* items, unsigned count) : arr(new dyn_arr<T>(items, count)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T, Mutability M>
array_seq<T, M>::array_seq(const array_seq& other) : arr(new dyn_arr<T>(*other.arr)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T, Mutability M>
array_seq<T, M>::array_seq(const dyn_arr<T>& other) : arr(new dyn_arr<T>(other)) {
    if (arr == nullptr) throw null_ptr();
}

template<typename T, Mutability M>
array_seq<T, M>::~array_seq() {
    delete arr;
}

template<typename T, Mutability M>
array_seq<T, M>& array_seq<T, M>::operator=(const array_seq& other) {
    if (this != &other) {
        dyn_arr<T>* new_arr = new dyn_arr<T>(*other.arr);
        if (new_arr == nullptr) throw null_ptr();
        delete arr;
        arr = new_arr;
    }
    return *this;
}

template<typename T, Mutability M>
T array_seq<T, M>::get_first() const {
    if (size() == 0) throw empty_container();
    return (*arr)[0];
}

template<typename T, Mutability M>
T array_seq<T, M>::get_last() const {
    if (size() == 0) throw empty_container();
    return (*arr)[size() - 1];
}

template<typename T, Mutability M>
unsigned array_seq<T, M>::size() const {
    return arr->size();
}

template<typename T, Mutability M>
sequence<T>* array_seq<T, M>::append(const T& item) {
    if constexpr (M == Mutability::Mutable) {
        arr->resize(arr->size() + 1);
        (*arr)[arr->size() - 1] = item;
        return this;
    } else {
        array_seq<Mutability::Immutable, T>* new_seq =
            new array_seq<Mutability::Immutable, T>(arr->size() + 1);
        if (!new_seq) throw null_ptr();
        for (unsigned i = 0; i<arr->size(); i++){
            (*new_seq)[i] = (*arr)[i];
        }
        (*new_seq)[arr->size()] = item;
        return new_seq;
    }
}

template<typename T, Mutability M>
sequence<T>* array_seq<T, M>::prepend(const T& item) {
    if constexpr (M == Mutability::Mutable) {
        arr->resize(arr->size() + 1);
        for (unsigned i = arr->size() - 1; i > 0; --i)
            (*arr)[i] = (*arr)[i - 1];
        (*arr)[0] = item;
        return this;
    } else {
        dyn_arr<T>* new_arr = new dyn_arr<T>(arr->size() + 1);
        if (!new_arr) throw null_ptr();
        (*new_arr)[0] = item;
        for (unsigned i = 0; i < arr->size(); ++i)
            (*new_arr)[i + 1] = (*arr)[i];
        array_seq<Mutability::Immutable, T>* result =
            new array_seq<Mutability::Immutable, T>(*new_arr);
        if (!result) throw null_ptr();
        delete new_arr;
        return result;
    }
}

template<typename T, Mutability M>
sequence<T>* array_seq<T, M>::insert(const T& item, unsigned index) {
    if (index > arr->size()) throw index_out_of_range();
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
        if (new_arr == nullptr) throw null_ptr();
        for (unsigned i = 0; i < index; ++i)
            (*new_arr)[i] = (*arr)[i];
        (*new_arr)[index] = item;
        for (unsigned i = index; i < arr->size(); ++i)
            (*new_arr)[i + 1] = (*arr)[i];
        array_seq<Mutability::Immutable, T>* result =
            new array_seq<Mutability::Immutable, T>(*new_arr);
        if (result == nullptr) throw null_ptr();
        delete new_arr;
        return result;
    }
}

template<typename T, Mutability M>
sequence<T>* array_seq<T, M>::concat(sequence<T>* other) {
    if (other == nullptr) throw null_ptr();
    array_seq<Mutability::Immutable, T>* result =
        new array_seq<Mutability::Immutable, T>(*arr);
    if (result == nullptr) throw null_ptr();
    for (unsigned i = 0; i < other->size(); ++i) {
        result = static_cast<array_seq<Mutability::Immutable, T>*>(
            result->append((*other)[i])
        );
    }
    return result;
}

template<typename T, Mutability M>
sequence<T>* array_seq<T, M>::get_subsequence(unsigned start, unsigned end) const {
    if (start > end || end > size()) throw invalid_argument();
    array_seq<Mutability::Immutable, T>* sub =
        new array_seq<Mutability::Immutable, T>();
    if (sub == nullptr) throw null_ptr();
    for (unsigned i = start; i < end; ++i) {
        sub = static_cast<array_seq<Mutability::Immutable, T>*>(
            sub->append((*arr)[i])
        );
    }
    return sub;
}

template<typename T, Mutability M>
unsigned array_seq<T, M>::find(const T& value) const {
    for (unsigned i = 0; i < size(); ++i) {
        if ((*arr)[i] == value) return i;
    }
    throw invalid_argument();
    return 0;
}

template<typename T, Mutability M>
T& array_seq<T, M>::operator[](unsigned index) {
    if (index >= size()) throw index_out_of_range();
    return (*arr)[index];
}

template<typename T, Mutability M>
const T& array_seq<T, M>::operator[](unsigned index) const {
    if (index >= size()) throw index_out_of_range();
    return (*arr)[index];
}

template<typename T, Mutability M>
template <typename Func>
sequence<T>* array_seq<T, M>::map(Func f) {
    if constexpr (M == Mutability::Mutable) {
        for (auto& x : *arr) x = f(x);
        return this;
    } else {
        array_seq<Mutability::Immutable, T>* new_arr =
            new array_seq<Mutability::Immutable, T>(*arr);
        if (new_arr == nullptr) throw null_ptr();
        for (auto& x : *(new_arr->arr)) x = f(x);
        return new_arr;
    }
}

template<typename T, Mutability M>
template <typename Func>
sequence<T>* array_seq<T, M>::where(Func f) {
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
        if (!new_data) throw null_ptr();
        unsigned pos = 0;
        for (unsigned i = 0; i < arr->size(); ++i) {
            if (f((*arr)[i])) {
                (*new_data)[pos++] = (*arr)[i];
            }
        }
        array_seq<Mutability::Immutable, T>* result =
            new array_seq<Mutability::Immutable, T>(pos);
        if (!result) throw null_ptr();
        for (unsigned i = 0; i < pos; ++i){(*result)[i] = (*new_data)[i];}
        delete new_data;
        return result;
    }
}

template<typename T, Mutability M>
template <typename Func, typename U>
U array_seq<T, M>::reduce(Func f, U initial) const {
    if (arr->size() == 0) throw empty_container();
    U acc = f(initial, (*arr)[0]);
    for (unsigned i = 1; i < arr->size(); ++i) {
        acc = f(acc, (*arr)[i]);
    }
    return acc;
}