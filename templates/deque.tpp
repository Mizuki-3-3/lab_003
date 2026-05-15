#include "deque.hpp"
#include "errors.hpp"

//deque
template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>::~deque(){
    for (segment* seg : d_segments)
        delete seg;
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>::deque(size_t initial_size) : deque() {
    for (size_t i = 0; i < initial_size; i++)
        push_back(T{});
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
size_t deque<Container, T, InternalContainer, SEGSIZE>::size() const {
    if (d_segments.size() == 0) return 0;
    if (d_segments.size() == 1) {
        return last_elem_idx - first_elem_idx + 1;
    }
    return (d_segments.size() - 2) * segment_size +
           (segment_size - first_elem_idx) +
           (last_elem_idx + 1);
}



template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>::deque(const deque& other):
    first_elem_idx(other.first_elem_idx), last_elem_idx(other.last_elem_idx)
{
    for (segment* seg : other.d_segments) {
        if (seg) {
            d_segments.push_back(new segment(*seg));
        } else {
            d_segments.push_back(nullptr);
        }
    }
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>::deque(deque&& other) noexcept{
    d_segments = std::move(other.d_segments);
    first_elem_idx = std::move(other.first_elem_idx);
    last_elem_idx = std::move(other.last_elem_idx);
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>::deque(): d_segments(), first_elem_idx(0), last_elem_idx(0){}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>& deque<Container, T, InternalContainer, SEGSIZE>::operator=(const deque<Container, T, InternalContainer, SEGSIZE>& other) {
    if (this != &other) {
        deque temp(other);
        std::swap(d_segments, temp.d_segments);
        std::swap(first_elem_idx, temp.first_elem_idx);
        std::swap(last_elem_idx, temp.last_elem_idx);
    }
    return *this;
}


//кроме конструкторов и дестр
template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::push_back(const T& value) {
    if (d_segments.size() == 0) {
        segment* seg = new segment();
        (*seg)[0] = value;
        d_segments.insert(seg, 0);
        first_elem_idx = last_elem_idx = 0;
        return this;
    }

    segment* last_seg = d_segments.back();
    if (last_elem_idx + 1 < segment_size) {
        (*last_seg)[++last_elem_idx] = value;
    } else {
        segment* new_seg = new segment();
        (*new_seg)[0] = value;
        d_segments.insert(new_seg, d_segments.size());
        last_elem_idx = 0;
    }
    return this;
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::push_front(const T& value) {
    if (d_segments.size() == 0) {
        segment* seg = new segment();
       (*seg)[0] = value;
        d_segments.insert(seg, 0);
        first_elem_idx = 0;
        last_elem_idx = 0;
        return this;
    }

    segment* first_seg = d_segments.front();
    if (first_elem_idx > 0) {
        (*first_seg)[--first_elem_idx] = value;
    } else {
        segment* new_seg = new segment();
        (*new_seg)[segment_size-1] = value;
        d_segments.insert(new_seg, 0);
        first_elem_idx = segment_size-1;
    }
    return this;
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
T& deque<Container, T, InternalContainer, SEGSIZE>::operator[](size_t index) {
    if (index >= size()) throw index_out_of_range();
    size_t physical = first_elem_idx + index;
    size_t seg_idx = physical / segment_size;
    size_t offset = physical % segment_size;
    return (*d_segments[seg_idx])[offset];
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
const T& deque<Container, T, InternalContainer, SEGSIZE>::operator[](size_t index) const {
    if (index >= size()) throw index_out_of_range();
    size_t physical = first_elem_idx + index;
    size_t seg_idx = physical / segment_size;
    size_t offset = physical % segment_size;
    return (*d_segments[seg_idx])[offset];
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
T deque<Container, T, InternalContainer, SEGSIZE>::front() const{
    if (d_segments.size() == 0) throw empty_container();
    return (*d_segments[0])[first_elem_idx];
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
T deque<Container, T, InternalContainer, SEGSIZE>::back() const{
    if (d_segments.size() == 0) throw empty_container();
    return (*d_segments[d_segments.size()-1])[last_elem_idx];
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
auto deque<Container, T, InternalContainer, SEGSIZE>::begin() -> typename Container<T>::iterator{
    return (*d_segments[0]).begin();
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
auto deque<Container, T, InternalContainer, SEGSIZE>::end() -> typename Container<T>::iterator{
    return (*d_segments[d_segments.size()-1]).end();
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
auto deque<Container, T, InternalContainer, SEGSIZE>::begin() const -> typename Container<T>::const_iterator{
    return (*d_segments[0]).begin();
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
auto deque<Container, T, InternalContainer, SEGSIZE>::end() const -> typename Container<T>::const_iterator{
    return (*d_segments[d_segments.size()-1]).end();
}


template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
void deque<Container, T, InternalContainer, SEGSIZE>::sort() {
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

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
template <typename Func>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::map(Func func) const {
    deque<Container, T, InternalContainer, SEGSIZE>* result = new deque<Container, T, InternalContainer, SEGSIZE>;
    size_t sz = size();
    for (size_t i = 0; i < sz; i++)
        result->push_back(func((*this)[i]));
    return result;
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
template <typename Func>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::where(Func func) const {
    deque<Container, T, InternalContainer, SEGSIZE>* result = new deque<Container, T, InternalContainer, SEGSIZE>;
    size_t sz = size();
    for (size_t i = 0; i < sz; i++) {
        const T& val = (*this)[i];
        if (func(val))
            result->push_back(val);
    }
    return result;
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
template <typename Acc, typename Func>
Acc deque<Container, T, InternalContainer, SEGSIZE>::reduce(Acc init, Func func) const {
    Acc result = init;
    size_t sz = size();
    for (size_t i = 0; i < sz; i++)
        result = func(result, (*this)[i]);
    return result;
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::concat(const deque<Container, T, InternalContainer, SEGSIZE>& other) const {
    deque<Container, T, InternalContainer, SEGSIZE>* result = new deque<Container, T, InternalContainer, SEGSIZE>;
    size_t sz1 = size();
    for (size_t i = 0; i < sz1; i++)
        result->push_back((*this)[i]);
    size_t sz2 = other.size();
    for (size_t i = 0; i < sz2; i++)
        result->push_back(other[i]);
    return result;
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::subdeque(size_t start, size_t end) const {
    size_t sz = size();
    if (start > end || start > sz || end > sz+1)
        throw index_out_of_range();
    deque<Container, T, InternalContainer, SEGSIZE>* result = new deque<Container, T, InternalContainer, SEGSIZE>;
    for (size_t i = start; i < end; i++)
        result->push_back((*this)[i]);
    return result;
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
size_t deque<Container, T, InternalContainer, SEGSIZE>::find_subsequence(const deque<Container, T, InternalContainer, SEGSIZE>& pattern) const {
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

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
template<typename Func>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::merge(const deque<Container, T, InternalContainer, SEGSIZE>* b, Func func) {
    deque<Container, T, InternalContainer, SEGSIZE>* result = new deque<Container, T, InternalContainer, SEGSIZE>;
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