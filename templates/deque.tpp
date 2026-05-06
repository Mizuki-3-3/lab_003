#include "deque.hpp"

//seqment
template <typename T>
deque<T>::segment::segment(): segment_(T[8]){}

template <typename T>
deque<T>::segment::segment(size_t initial_size): data(initial_size? new T[initial_size]: nullptr),length(initial_size), first(0), count(0){}

template <typename T>
deque<T>::segment::segment(const segment& other): length(other.length), first(0), count(other.count) {
    data = new T[length];
    for (size_t i = 0; i < count; ++i)
        data[i] = other[(other.first + i) % other.length];
    first = 0;
}

template <typename T>
size_t deque<T>::segment::size() const { return count; }

template <typename T>
size_t deque<T>::segment::capacity() const { return length; }

template <typename T>
bool deque<T>::segment::empty() const { return count == 0; }

template <typename T>
bool deque<T>::segment::full() const { return count == length; }

template <typename T>
deque<T>::segment::~segment(): {delete[] data;}

template <typename T>
T& deque<T>::segment::front() { return data[first]; }

template <typename T>
const T& deque<T>::segment::front() const { return data[first]; }

template <typename T>
T& deque<T>::segment::back() { return data[(first + count - 1) % length]; }

template <typename T>
const T& deque<T>::segment::back() const { return data[(first + count - 1) % length]; }

template <typename T>
void deque<T>::segment::push_front(const T& value) {
    if (full()) throw full_segment();
    first = (first == 0) ? length - 1 : first - 1;
    data[first] = value;
    ++count;
}

template <typename T>
void deque<T>::segment::push_back(const T& value) {
    if (full()) throw full_segment();
    data[(first + count) % length] = value;
    ++count;
}

template <typename T>
T& deque<T>::segment::pop_front() {
    if (empty()) throw empty_segment();
    first = (first + 1) % length;
    --count;
    return data[first-1];
}

template <typename T>
T& deque<T>::segment::pop_back() {
    if (empty()) throw empty_segment();
    --count;
    return data[first+count]
}

template <typename T>
T& deque<T>::segment::operator[](size_t idx) {
    return data[(first + idx) % length];
}

template <typename T>
const T& deque<T>::segment::operator[](size_t idx) const {
    return data[(first + idx) % length];
}

//map
template <typename T>
deque<T>::map::map(): map_(nullptr), length(0), cap(0){}

template <typename T>
deque<T>::map::map(size_t initial_size = 4): cap(initial_size), length(0), map_(new *segment[initial_size]){}

template <typename T>
deque<T>::map::map(map& other): map_(other.map_), length(other.length), cap(other.cap){}

template <typename T>
deque<T>::map::~map() { delete[] map_; }

template <typename T>
deque<T>::map::map(const map& other) : cap(other.cap), length(other.length) {
    map_ = new segment*[cap];
    for (size_t i = 0; i < cap; ++i)
        map_[i] = (other.map_[i] ? new segment(*other.map_[i]) : nullptr);
}

template <typename T>
deque<T>::map::map(map&& other) {
    map_ = std::move(other.map_);
    cap = std::move(other.cap);
    length = std::move(other.length);
}

template <typename T>
size_t deque<T>::map::size() const { return length; }
    
template <typename T>
size_t deque<T>::map::capacity() const { return cap; }

template <typename T>
deque<T>::segment*& deque<T>::map::operator[](size_t idx) { return map_[idx]; }

template <typename T>
deque<T>::segment* const& deque<T>::map::operator[](size_t idx) const { return map_[idx]; }

template <typename T>
void deque<T>::map::push_back(segment* seg) {
    if (length == cap) resize(cap * 2);
    map_[length++] = seg;
}

template <typename T>
void deque<T>::map::push_front(segment* seg) {
    if (length == cap) resize(cap * 2);
    for (size_t i = length; i > 0; --i)
        map_[i] = map_[i - 1];
    map_[0] = seg;
    ++length;
}

template <typename T>
void deque<T>::map::pop_back() {
    if (length == 0) throw empty_container();
    --length;
}

template <typename T>
void deque<T>::map::pop_front() {
    if (length == 0) throw empty_container();
    for (size_t i = 0; i < length - 1; ++i)
        map_[i] = map_[i + 1];
    --length;
}

template <typename T>
void deque<T>::map::set(size_t idx, segment* seg) {
    map_[idx] = seg;
}

template <typename T>
void deque<T>::map::resize(size_t new_size) {
    segment** new_map_ = new segment*[new_cap];
    for (size_t i = 0; i < length; ++i)
        new_map_[i] = map_[i];
    for (size_t i = length; i < new_cap; ++i)
        new_map_[i] = nullptr;
    delete[] map_;
    map_ = new_map_;
    cap = new_cap;
}
//iterator
template <typename T>
deque<T>::iterator::operator*(){
    return *current_elem;
}

template <typename T>
deque<T>::iterator::operator++(){
    if(current_elem==begin_of_curr_segment+current_segment.size()){
        current_segment = (*map_map)
    }
    current_elem++;
    return *this;
}

template <typename T>
deque<T>::deque(): map_map(nullptr), segment_size(8), map_size(0), map_start_idx(0), first_elem(), last_elem(){}

template <typename T>
deque<T>::deque(size_t initial_size): map_begin(initial_size/2), map_size(initial.size), seg_size(sizeof(T)){}

template <typename T>
deque<T>::~deque(){
    delete map_map;
    delete map_size;
    delete segment_size;
    delete map_start_idx;
    delete first elem;
    delete last_elem;
}

template <typename T>
deque<T>::deque()
    : map_map(new map(4)), map_size(0), segment_size(64), map_start_idx(0),
      first_elem(nullptr, nullptr, nullptr), last_elem(nullptr, nullptr, nullptr) {}

template <typename T>
deque<T>::deque(size_t initial_size): deque()
{
    if (initial_size == 0) return;

    size_t seg_cap = segment_size;
    size_t num_segments = (initial_size + seg_cap - 1) / seg_cap;
    while (map_map->capacity() < num_segments)
        map_map->resize(map_map->capacity() * 2);

    for (size_t i = 0; i < num_segments; ++i) {
        segment* seg = new segment(seg_cap);
        map_map->push_back(seg);
        ++map_size;
    }
    size_t remaining = initial_size;
    for (size_t i = 0; i < num_segments && remaining > 0; ++i) {
        segment* seg = (*map_map)[i];
        size_t to_fill = std::min(remaining, seg_cap);
        for (size_t j = 0; j < to_fill; ++j) {
            seg->push_back(T());
        }
        remaining -= to_fill;
    }

    map_start_idx = 0;
}

template <typename T>
deque<T>::deque(const deque& other)
    : map_map(new map(*other.map_map)), map_size(other.map_size),
      segment_size(other.segment_size), map_start_idx(other.map_start_idx),
      first_elem(other.first_elem), last_elem(other.last_elem) {}

template <typename T>
deque<T>::deque(deque&& other) noexcept{
    map_map = std::move(other.map_map);
    map_size = std::move(other.map_size);
    segment_size = std::move(other.segment_size);
    map_start_idx = std::move(other.map_start_idx);
    first_elem = std::move(other.first_elem);
    last_elem = std::move(other.last_elem);
}

template <typename T>
deque<T>::~deque() {
    if (map_map) {
        for (size_t i = 0; i < map_map->size(); ++i) {
            delete (*map_map)[i];
        }
        delete[] map_map;
    }
}


//кроме конструкторов и дестр
template <typename T>
deque<T>* deque<T>::append(const T& value) {
    if (map_size == 0) {
        segment* seg = new segment(segment_size);
        seg->push_back(value);
        map_map->push_back(seg);
        ++map_size;
        map_start_idx = ;
        return this;
    }
    size_t last_idx = map_start_idx + map_size - 1;
    segment* last_seg = (*map_map)[map_size - 1];
    if (!last_seg->full()) {
        last_seg->push_back(value);
    } else {
        segment* new_seg = new segment(segment_size);
        new_seg->push_back(value);
        map_map->push_back(new_seg);
        ++map_size;
    }
    return this;
}

template <typename T>
deque<T>* deque<T>::prepend() {
    return prepend(T());
}

template <typename T>
deque<T>* deque<T>::prepend(const T& value) {
    if (map_size == 0) {
        segment* seg = new segment(segment_size);
        seg->push_back(value); 
        map_map->push_back(seg);
        ++map_size;
        map_start_idx = 0;
        return this;
    }

    segment* first_seg = (*map_map)[0];
    if (!first_seg->full()) {
        first_seg->push_front(value);
    } else {
        segment* new_seg = new segment(segment_size);
        new_seg->push_back(value);
        map_map->push_front(new_seg);
        ++map_size;
    }
    return this;
}


template <typename T>
T& deque<T>::operator[](size_t index) {
    if (index >= size())
        throw index_out_of_range();
    size_t seg_idx = index / segment_size;
    size_t offset = index % segment_size;
    return (*((*map_map)[seg_idx]))[offset];
}

template <typename T>
const T& deque<T>::operator[](size_t index) const {
    if (index >= size())
        throw index_out_of_range();
    size_t seg_idx = index / segment_size;
    size_t offset = index % segment_size;
    return (*((*map_map)[seg_idx]))[offset];
}

template <typename T>
T deque<T>::get_first() const {
    if (map_size == 0) throw empty_container();
    segment* first = (*map_map)[0];
    return first->front();
}

template <typename T>
T deque<T>::get_last() const {
    if (map_size == 0) throw emty_container;
    segment* last = (*map_map)[map_size - 1];
    return last->back();
}

template <typename T>
size_t deque<T>::size() const {
    if (map_size == 0) return 0;
    size_t total = (map_size - 1) * segment_size;
    segment* last = (*map_map)[map_size - 1];
    total += last->size();
    return total;
}

template <typename T>
typename deque<T>::iterator deque<T>::begin() {
    if (map_size == 0) return end();
    segment* first_seg = (*map_map)[0];
    T* ptr = &(first_seg->front());
    return iterator(ptr, nullptr, nullptr);
}

template <typename T>
typename deque<T>::iterator deque<T>::end() {
    return iterator(nullptr, nullptr, nullptr);
}
