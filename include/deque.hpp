#pragma once
#include <cstddef>

template <template<typename> class Container, typename T, template<typename> class InternalContainer = Container, size_t SEGSIZE = 8>
class deque{
using segment = InternalContainer<T>;
using iterator = typename InternalContainer<T>::iterator;
using const_iterator = typename InternalContainer<T>::const_iterator;
private:
    
    Container<segment*> d_segments;
    static constexpr size_t segment_size = SEGSIZE;
    size_t first_elem_idx; //<segment_size
    size_t last_elem_idx;//<segment_size
    
public:
    explicit deque();
    explicit deque(size_t initial_size);
    deque(const deque& other);
    deque(deque&& move);
    deque& operator=(const deque& other);
    ~deque();

    deque<Container, T, InternalContainer, SEGSIZE>* push_back(const T& value) ;
    deque<Container, T, InternalContainer, SEGSIZE>* push_front(const T& value) ;
    deque<Container, T, InternalContainer, SEGSIZE>* insert(const T& item, iterator place);//пользуемся insert т.к. есть во всех
    deque<Container, T, InternalContainer, SEGSIZE>* insert(const T& item, const_iterator place);

    T& operator[](size_t index) ;
    const T& operator[](size_t index) const ;

    T front() const;
    T back() const;
    size_t size() const;

    typename Container<T>::iterator begin();
    typename Container<T>::iterator end();
    typename Container<T>::const_iterator begin() const;
    typename Container<T>::const_iterator end() const;

    iterator find(const T& value);///< returns iterator
    const_iterator find(const T& value) const;

    void sort();
    template <typename Func>
    deque<Container, T>* map(Func func) const;
    template <typename Func>
    deque<Container, T>* where(Func pred) const;
    template <typename Acc, typename Func>
    Acc reduce(Acc init, Func func) const;
    deque<Container, T>* concat(const deque<Container, T>& other) const;
    deque<Container, T>* subdeque(size_t start, size_t end) const;
    iterator find_subsequence(const deque<Container, T>& pattern);/// < ret iterator
    const_iterator find_subsequence(const deque<Container, T>& pattern) const;
    template <typename Func>
    deque<Container, T>* merge(const deque<Container, T>* b, Func func);
    using value_type = T;
};

