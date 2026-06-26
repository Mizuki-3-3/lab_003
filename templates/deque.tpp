#include "deque.hpp"
#include "errors.hpp"
#include <cstddef>
#include <initializer_list>

//deque
template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>::deque(): d_segments(), first_elem_idx(0), last_elem_idx(0){}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>::deque(size_t initial_size) : d_segments(initial_size/SEGSIZE + static_cast<size_t>(!!(initial_size%SEGSIZE == 0))) {
    for (size_t i = 0; i < d_segments.size(); ++i)
    d_segments[i] = new segment(SEGSIZE);
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>::deque(const T* items, size_t count) : deque(){
    for (size_t i = 0; i < count; ++i)
    this->push_back(items[i]);
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>::deque(const std::initializer_list<T> init) : deque(){
    for (auto& elem: init){
        this->push_back(elem);
    }
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>::deque(const deque& other):
    d_segments(other.d_segments.size()), first_elem_idx(other.first_elem_idx), last_elem_idx(other.last_elem_idx) {
    for (size_t i = 0; i < other.d_segments.size(); i++) {
        d_segments[i] = new segment(*(other.d_segments[i]));
    }
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>::deque(deque&& move){
    d_segments = std::move(move.d_segments);
    first_elem_idx = std::move(move.first_elem_idx);
    last_elem_idx = std::move(move.last_elem_idx);
}

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

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>::~deque(){
    for (segment* seg : d_segments)
        delete seg;
}

//кроме конструкторов и дестр
template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::push_back(const T& value) {
    if (d_segments.size() == 0) {
        InternalContainer<T>* seg = new InternalContainer<T>(SEGSIZE);
        (*seg)[0] = value;
        d_segments.insert(d_segments.end(), seg);
        return this;
    }

    InternalContainer<T>* last_seg = d_segments.back();
    if (last_elem_idx + 1 < segment_size) {
        last_elem_idx++;
        (*last_seg)[last_elem_idx] = value;
    } else {
        InternalContainer<T>* new_seg = new InternalContainer<T>(SEGSIZE);
        (*new_seg)[0] = value;
        d_segments.insert(d_segments.end(), new_seg);
        last_elem_idx = 0;
    }
    return this;
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::push_front(const T& value) {
    if (d_segments.size() == 0) {
        InternalContainer<T>* seg = new InternalContainer<T>(SEGSIZE);
       (*seg)[0] = value;
        d_segments.insert(d_segments.begin(), seg);
        return this;
    }

    InternalContainer<T>* first_seg = d_segments.front();
    if (first_elem_idx > 0) {
        (*first_seg)[--first_elem_idx] = value;
    } else {
        InternalContainer<T>* new_seg = new InternalContainer<T>(SEGSIZE);
        (*new_seg)[segment_size-1] = value;
        d_segments.insert(d_segments.begin(), new_seg);
        first_elem_idx = segment_size-1;
    }
    return this;
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::insert(const_iterator place, const T& value){
    if (place == end()){push_back(value); return this;}
    if (place == begin()){push_front(value); return this;}

    size_t idx = 0;
    iterator current = begin();
    while(current!=place && current != end()){
        current++;
        idx++;
    }
    if (current == end()){
        throw index_out_of_range("incorrect iterator in insert");
    }
    if (idx> (size()/2)){
        push_back(T());
        for(size_t i = size()-1; i > idx; i--){
            (*this)[i] = (*this)[i-1];
        }
    }else{
        push_front(T());
        for(size_t i = 0; i < idx; i++){
            (*this)[i] = (*this)[i+1];
        }
    }
    (*this)[idx] = value;
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
auto deque<Container, T, InternalContainer, SEGSIZE>::begin() -> iterator{
    if (d_segments.size() == 0) return iterator(d_segments.end(), inner_iterator(nullptr));
    return iterator(d_segments.begin(), (*d_segments.begin())->begin() + first_elem_idx);
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
auto deque<Container, T, InternalContainer, SEGSIZE>::end() -> iterator{
    if (d_segments.size()==0) {
        return iterator(d_segments.end(), inner_iterator(nullptr));
    }
    size_t last_seg_index = d_segments.size() - 1;
    auto last_seg_it = d_segments.begin() + last_seg_index;
    InternalContainer<T>* last_seg = *last_seg_it;
    if (last_elem_idx + 1 < segment_size) {
        return iterator(last_seg_it, last_seg->begin() + last_elem_idx + 1);
    } else {
        return iterator(last_seg_it, last_seg->end());
    }
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
auto deque<Container, T, InternalContainer, SEGSIZE>::begin() const -> const_iterator{
    if (d_segments.size() == 0) return const_iterator(d_segments.end(), const_inner_iterator(nullptr));
    return const_iterator(d_segments.begin(), const_inner_iterator((*d_segments.begin())->begin()) + first_elem_idx);
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
auto deque<Container, T, InternalContainer, SEGSIZE>::end() const -> const_iterator{
    if (d_segments.size()==0) {
        return const_iterator(d_segments.end(), const_inner_iterator(nullptr));
    }
    size_t last_seg_index = d_segments.size() - 1;
    auto last_seg_it = d_segments.begin() + last_seg_index;
    InternalContainer<T>* last_seg = *last_seg_it;
    if (last_elem_idx + 1 < segment_size) {
        return const_iterator(last_seg_it, const_inner_iterator(last_seg->begin()) + last_elem_idx + 1);
    } else {
        return const_iterator(last_seg_it, const_inner_iterator(last_seg->end()));
    }
}


template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
auto deque<Container, T, InternalContainer, SEGSIZE>::find(const T& value) -> iterator {
    size_t sz = size();
    for (size_t i = 0; i < sz; i++) {
        if ((*this)[i] == value) {
            size_t physical = first_elem_idx + i;
            size_t seg_idx = physical / segment_size;
            size_t offset = physical % segment_size;
            auto seg_it = d_segments.begin() + seg_idx;
            return iterator(seg_it, (*seg_it)->begin() + offset);
        }
    }
    throw not_found();
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
auto deque<Container, T, InternalContainer, SEGSIZE>::find(const T& value) const -> const_iterator {
    size_t sz = size();
    for (size_t i = 0; i < sz; i++) {
        if ((*this)[i] == value) {
            size_t physical = first_elem_idx + i;
            size_t seg_idx = physical / segment_size;
            size_t offset = physical % segment_size;
            auto seg_it = d_segments.begin() + seg_idx;
            return const_iterator(seg_it, (*seg_it)->begin() + offset);
        }
    }
    throw not_found();
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
void deque<Container, T, InternalContainer, SEGSIZE>::sort() {
    if (size() == 0){return;}
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
    deque<Container, T, InternalContainer, SEGSIZE>* result = new deque<Container, T, InternalContainer, SEGSIZE>();
    size_t sz1 = (*this).size();
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
        throw index_out_of_range("wrong indexes in subdeque()");
    deque<Container, T, InternalContainer, SEGSIZE>* result = new deque<Container, T, InternalContainer, SEGSIZE>;
    for (size_t i = start; i < end; i++)
        result->push_back((*this)[i]);
    return result;
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
auto deque<Container, T, InternalContainer, SEGSIZE>::find_subsequence(const deque<Container, T, InternalContainer, SEGSIZE>& pattern) -> iterator{
    size_t n = size();
    size_t m = pattern.size();
    if (m == 0) return begin();
    if (m > n) throw size_mismatch("pattern size is larger than deque size");
    iterator it = begin();
    for (size_t i = 0; i <= n - m; i++) {
        bool found = true;
        for (size_t j = 0; j < m; j++) {
            if ((*this)[i + j] != pattern[j]) {
                found = false;
                break;
            }
        }
        if (found) return it;
        it++;
    }
    throw not_found("no such pattern in deque");
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
auto deque<Container, T, InternalContainer, SEGSIZE>::find_subsequence(const deque<Container, T, InternalContainer, SEGSIZE>& pattern) const -> const_iterator{
    size_t n = size();
    size_t m = pattern.size();
    if (m == 0) return begin();
    if (m > n) throw size_mismatch("pattern size is larger than deque size");
    const_iterator it = begin();
    for (size_t i = 0; i <= n - m; i++) {
        bool found = true;
        for (size_t j = 0; j < m; j++) {
            if ((*this)[i + j] != pattern[j]) {
                found = false;
                break;
            }
        }
        if (found) return it;
        it++;
    }
    throw not_found("no such pattern in deque");
}

template <template<typename> class Container, typename T, template<typename> class InternalContainer, size_t SEGSIZE>
template<typename Func>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::merge(const deque<Container, T, InternalContainer, SEGSIZE>* b, Func func) const {
    deque<Container, T, InternalContainer, SEGSIZE>* result = new deque<Container, T, InternalContainer, SEGSIZE>;
    if (result == nullptr) throw null_ptr("allocate fault in merge for deque");
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

template <template <typename> class Container, typename T, template <typename> class InternalContainer, size_t SEGSIZE>
bool deque<Container, T, InternalContainer, SEGSIZE>::operator==(const deque<Container, T, InternalContainer, SEGSIZE>& rdeque) const {
    if (rdeque.size()!= size()){
        return false;
    }
    for (size_t i = 0; i < rdeque.size(); i++){
        if ((*this)[i] != rdeque[i]){return false;}
    }
    return true;
}

template <template <typename> class Container, typename T, template <typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::pop_back(){
    if (last_elem_idx == 0){
        last_elem_idx = SEGSIZE -1;
        delete d_segments[d_segments.size()-1];
    }else{
        last_elem_idx--;
    }
    return this;
}


template <template <typename> class Container, typename T, template <typename> class InternalContainer, size_t SEGSIZE>
deque<Container, T, InternalContainer, SEGSIZE>* deque<Container, T, InternalContainer, SEGSIZE>::pop_front(){
    if (first_elem_idx == SEGSIZE -1){
        last_elem_idx = 0;
        delete d_segments[0];
    }else{
        last_elem_idx--;
    }
    return this;
}