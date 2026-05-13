#include "deque.hpp"

//seqment
template <template<typename> class Container, typename T>
deque<Container, T>::segment::segment(): data(Container<T>(segment_size)){}

template <template<typename> class Container, typename T>
deque<Container, T>::segment::segment(const segment& other): data(other.data) {}

template <template<typename> class Container, typename T>
size_t deque<Container, T>::segment::size() const { return segment_size; }

template <template<typename> class Container, typename T>
T& deque<Container, T>::segment::operator[](size_t idx) {
    return data[idx];
}

template <template<typename> class Container, typename T>
const T& deque<Container, T>::segment::operator[](size_t idx) const {
    return data[idx];
}

template <template<typename> class Container, typename T>
Container<T>& deque<Container, T>::segment::operator*(){
    return data;
}

template <template<typename> class Container, typename T>
Container<T>* deque<Container, T>::segment::operator->() {
    return &data;
}

//deque
template <template<typename> class Container, typename T>
deque<Container, T>::~deque(){
    for (segment* seg : mapa)
        delete seg;
}

template <template<typename> class Container, typename T>
deque<Container, T>::deque(size_t initial_size) : deque() {
    for (size_t i = 0; i < initial_size; i++)
        push_back(T{});
}

template <template<typename> class Container, typename T>
size_t deque<Container, T>::size() const {
    if (mapa.size() == 0) return 0;
    if (mapa.size() == 1) {
        return last_elem_idx - first_elem_idx + 1;
    }
    return (mapa.size() - 2) * segment_size +
           (segment_size - first_elem_idx) +
           (last_elem_idx + 1);
}



template <template<typename> class Container, typename T>
deque<Container, T>::deque(const deque& other):
    first_elem_idx(other.first_elem_idx), last_elem_idx(other.last_elem_idx)
{
    for (segment* seg : other.mapa) {
        if (seg) {
            mapa.push_back(new segment(*seg));
        } else {
            mapa.push_back(nullptr);
        }
    }
}

template <template<typename> class Container, typename T>
deque<Container, T>::deque(deque&& other) noexcept{
    mapa = std::move(other.mapa);
    first_elem_idx = std::move(other.first_elem_idx);
    last_elem_idx = std::move(other.last_elem_idx);
}

template <template<typename> class Container, typename T>
deque<Container, T>::deque(): mapa(), first_elem_idx(0), last_elem_idx(0){}

template <template<typename> class Container, typename T>
deque<Container, T>& deque<Container, T>::operator=(const deque<Container, T>& other) {
    if (this != &other) {
        deque temp(other);
        std::swap(mapa, temp.mapa);
        std::swap(first_elem_idx, temp.first_elem_idx);
        std::swap(last_elem_idx, temp.last_elem_idx);
    }
    return *this;
}


//кроме конструкторов и дестр
template <template<typename> class Container, typename T>
deque<Container, T>* deque<Container, T>::push_back(const T& value) {
    if (mapa.size() == 0) {
        segment* seg = new segment();
        (*seg)[0] = value;
        mapa.insert(seg, 0);
        first_elem_idx = last_elem_idx = 0;
        return this;
    }

    segment* last_seg = mapa.back();
    if (last_elem_idx + 1 < segment_size) {
        (*last_seg)[++last_elem_idx] = value;
    } else {
        segment* new_seg = new segment();
        (*new_seg)[0] = value;
        mapa.insert(new_seg, mapa.size());
        last_elem_idx = 0;
    }
    return this;
}

template <template<typename> class Container, typename T>
deque<Container, T>* deque<Container, T>::push_front(const T& value) {
    if (mapa.size() == 0) {
        segment* seg = new segment();
       (*seg)[0] = value;
        mapa.insert(seg, 0);
        first_elem_idx = 0;
        last_elem_idx = 0;
        return this;
    }

    segment* first_seg = mapa.front();
    if (first_elem_idx > 0) {
        (*first_seg)[--first_elem_idx] = value;
    } else {
        segment* new_seg = new segment();
        (*new_seg)[segment_size-1] = value;
        mapa.insert(new_seg, 0);
        first_elem_idx = segment_size-1;
    }
    return this;
}

template <template<typename> class Container, typename T>
deque<Container, T>* deque<Container, T>::insert(const T& item, size_t index) {
    size_t sz = size();
    if (index > sz) throw index_out_of_range();
    if (index == 0) return push_front(item);
    if (index == sz) return push_back(item);

    push_back(T{});
    for (size_t i = sz; i > index; --i) {
        (*this)[i] = (*this)[i - 1];
    }
    (*this)[index] = item;
    return this;
}

template <template<typename> class Container, typename T>
T& deque<Container, T>::operator[](size_t index) {
    if (index >= size()) throw index_out_of_range();
    size_t physical = first_elem_idx + index;
    size_t seg_idx = physical / segment_size;
    size_t offset = physical % segment_size;
    return (*mapa[seg_idx])[offset];
}

template <template<typename> class Container, typename T>
const T& deque<Container, T>::operator[](size_t index) const {
    if (index >= size()) throw index_out_of_range();
    size_t physical = first_elem_idx + index;
    size_t seg_idx = physical / segment_size;
    size_t offset = physical % segment_size;
    return (*mapa[seg_idx])[offset];
}

template <template<typename> class Container, typename T>
T deque<Container, T>::front() const{
    if (mapa.size() == 0) throw empty_container();
    return (*mapa[0])[first_elem_idx];
}

template <template<typename> class Container, typename T>
T deque<Container, T>::back() const{
    if (mapa.size() == 0) throw empty_container();
    return (*mapa[mapa.size()-1])[last_elem_idx];
}

template <template<typename> class Container, typename T>
auto deque<Container, T>::begin() -> typename Container<T>::iterator{
    return (*mapa[0]).begin();
}

template <template<typename> class Container, typename T>
auto deque<Container, T>::end() -> typename Container<T>::iterator{
    return (*mapa[mapa.size()-1]).end();
}

template <template<typename> class Container, typename T>
auto deque<Container, T>::begin() const -> typename Container<T>::const_iterator{
    return (*mapa[0]).begin();
}

template <template<typename> class Container, typename T>
auto deque<Container, T>::end() const -> typename Container<T>::const_iterator{
    return (*mapa[mapa.size()-1]).end();
}

template <template<typename> class Container, typename T>
size_t deque<Container, T>::find(const T& value) const {
    size_t sz = size();
    for (size_t i = 0; i < sz; i++) {
        if ((*this)[i] == value)
            return static_cast<size_t>(i);
    }
    throw not_found();
}

template <template<typename> class Container, typename T>
void deque<Container, T>::sort() {
    for (size_t i = 0; i<size()-1; i++){
        for (size_t j = 0; j<size()-i-1; j++){
            if ((*this)[j]>(*this)[j+1]){
                T tmp = (*this)[j];
                (*this)[j] = (*this)[j+1];
                (*this)[j+1] = tmp;
            }
        }
    }
}

template <template<typename> class Container, typename T>
template <typename Func>
deque<Container, T>* deque<Container, T>::map(Func func) const {
    deque<Container, T>* result = new deque<Container, T>;
    size_t sz = size();
    for (size_t i = 0; i < sz; i++)
        result->push_back(func((*this)[i]));
    return result;
}

template <template<typename> class Container, typename T>
template <typename Func>
deque<Container, T>* deque<Container, T>::where(Func func) const {
    deque<Container, T>* result = new deque<Container, T>;
    size_t sz = size();
    for (size_t i = 0; i < sz; i++) {
        const T& val = (*this)[i];
        if (func(val))
            result->push_back(val);
    }
    return result;
}

template <template<typename> class Container, typename T>
template <typename Acc, typename Func>
Acc deque<Container, T>::reduce(Acc init, Func func) const {
    Acc result = init;
    size_t sz = size();
    for (size_t i = 0; i < sz; i++)
        result = func(result, (*this)[i]);
    return result;
}

template <template<typename> class Container, typename T>
deque<Container, T>* deque<Container, T>::concat(const deque<Container, T>& other) const {
    deque<Container, T>* result = new deque<Container, T>;
    size_t sz1 = size();
    for (size_t i = 0; i < sz1; i++)
        result->push_back((*this)[i]);
    size_t sz2 = other.size();
    for (size_t i = 0; i < sz2; i++)
        result->push_back(other[i]);
    return result;
}

template <template<typename> class Container, typename T>
deque<Container, T>* deque<Container, T>::subdeque(size_t start, size_t end) const {
    size_t sz = size();
    if (start > end || start > sz || end > sz+1)
        throw index_out_of_range();
    deque<Container, T>* result = new deque<Container, T>;
    for (size_t i = start; i < end; i++)
        result->push_back((*this)[i]);
    return result;
}

template <template<typename> class Container, typename T>
size_t deque<Container, T>::find_subsequence(const deque<Container, T>& pattern) const {
    size_t n = size();
    size_t m = pattern.size();
    if (m == 0) return 0;
    if (m > n) return -1;
    for (size_t i = 0; i <= n - m; i++) {
        bool found = true;
        for (size_t j = 0; j < m; j++) {
            if ((*this)[i + j] != pattern[j]) {
                found = false;
                break;
            }
        }
        if (found) return i;
    }
    throw not_found();
}

template <template<typename> class Container, typename T>
template<typename Func>
deque<Container, T>* deque<Container, T>::merge(const deque<Container, T>* b, Func func) {
    deque<Container, T>* result = new deque<Container, T>;
    if (result == nullptr) throw null_ptr();
    size_t i = 0, j = 0;
    size_t na = size(), nb = b->size();
    while (i < na && j < nb) {
        if (func((*this)[i], (*b)[j])){
            result->push_back((*this)[i++]);
        }else{
            result->push_back((*b)[j++]);
        }
    }
    while (i < na) result->push_back((*this)[i++]);
    while (j < nb) result->push_back((*b)[j++]);
    return result;
}