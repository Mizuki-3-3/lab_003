#pragma once
#include <cstddef>
#include <functional>
#include <initializer_list>
template <template<typename> class Container, typename T, template<typename> class InternalContainer = Container, size_t SEGSIZE = 8>
class deque{
using segment = InternalContainer<T>;
using segment_iterator = typename Container<segment*>::iterator;
using const_segment_iterator = typename Container<segment*>::const_iterator;
using inner_iterator = typename InternalContainer<T>::iterator;
using const_inner_iterator = typename InternalContainer<T>::const_iterator;
private:
    
    Container<segment*> d_segments;
    static constexpr size_t segment_size = SEGSIZE;
    size_t first_elem_idx; //<segment_size
    size_t last_elem_idx;//<segment_size
    
public:
    class const_iterator;
    class iterator{
        segment_iterator seg;
        inner_iterator it;
    public:
    using iterator_category = std::forward_iterator_tag;
using value_type = T;
using difference_type = std::ptrdiff_t;
using pointer = T*;
using reference = T&;
        iterator(segment_iterator seg_it, inner_iterator inner_it) 
        : seg(seg_it), it(inner_it) {}
        operator const_iterator() const { return const_iterator(seg, it); }
        T& operator*() const{return *it;}
        T* operator->() const{return it.operator->();} //ачто ретурнить то емае
        iterator& operator++(){
            it++;
            if ((*seg)->end() == it){
                seg++;
                it = (*seg)->begin();
            }
            return *this;
        }
        iterator operator++(int){
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        iterator operator+(int right){
            iterator tmp = *this;
            for (int i = 0; i<right;i++) tmp++;
            return tmp;
        }
        bool operator==(const iterator& other) const {return it==other.it&&seg == other.seg;}
        bool operator!=(const iterator& other) const {return !(*this==other);}
        bool operator==(const const_iterator& other) const { return seg == other.seg && it == other.it; }
        bool operator!=(const const_iterator& other) const { return !(*this == other); }
        friend class const_iterator;
    };
    class const_iterator{
        const_segment_iterator seg;
        const_inner_iterator it;
    public:
    using iterator_category = std::forward_iterator_tag;
using value_type = T;
using difference_type = std::ptrdiff_t;
using pointer = T*;
using reference = T&;
        const_iterator(const_segment_iterator seg_it, const_inner_iterator inner_it) 
        : seg(seg_it), it(inner_it) {}
        const_iterator(const iterator& other): seg(other.seg), it(other.it){}
        const T& operator*() const{return *it;}
        const T* operator->() const{return it.operator->();}
        const_iterator& operator++(){
            it++;
            if ((*seg)->end() == it){
                seg++;
                it = (*seg)->begin();
            }
            return *this;
        }
        const_iterator operator++(int){
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        const_iterator operator+(int right){
            const_iterator tmp = *this;
            for (int i = 0; i<right;i++) tmp++;
            return tmp;
        }
        bool operator==(const const_iterator& other) const {return it==other.it&&seg == other.seg;}
        bool operator!=(const const_iterator& other) const {return !(*this==other);}
        bool operator==(const iterator& other) const { return seg == other.seg && it == other.it; }
bool operator!=(const iterator& other) const { return !(*this == other); }
        friend class iterator;
    };
    explicit deque();
    explicit deque(size_t initial_size);
    deque(const T* items, size_t count);
    deque(const std::initializer_list<T> initial_l);
    deque(const deque& other);
    deque(deque&& move);
    deque& operator=(const deque& other);
    ~deque();

    deque<Container, T, InternalContainer, SEGSIZE>* push_back(const T& value) ;
    deque<Container, T, InternalContainer, SEGSIZE>* push_front(const T& value) ;
    deque<Container, T, InternalContainer, SEGSIZE>* insert(const_iterator place, const T& item);//пользуемся insert т.к. есть во всех stl
    deque<Container, T, InternalContainer, SEGSIZE>* pop_back();
    deque<Container, T, InternalContainer, SEGSIZE>* pop_front();

    T& operator[](size_t index) ;
    const T& operator[](size_t index) const ;
    bool operator==(const deque<Container, T, InternalContainer, SEGSIZE>& rdeque) const;

    T front() const;
    T back() const;
    size_t size() const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    iterator find(const T& value);///< returns iterator
    const_iterator find(const T& value) const;

    void sort();
    template <typename Func>
    deque<Container, T, InternalContainer, SEGSIZE>* map(Func func) const;
    template <typename Func>
    deque<Container, T, InternalContainer, SEGSIZE>* where(Func pred) const;
    template <typename Acc, typename Func>
    Acc reduce(Acc init, Func func) const;
    deque<Container, T, InternalContainer, SEGSIZE>* concat(const deque<Container, T, InternalContainer, SEGSIZE>& other) const;
    deque<Container, T, InternalContainer, SEGSIZE>* subdeque(size_t start, size_t end) const;
    iterator find_subsequence(const deque<Container, T, InternalContainer, SEGSIZE>& pattern);/// < ret iterator
    const_iterator find_subsequence(const deque<Container, T, InternalContainer, SEGSIZE>& pattern) const;
    template <typename Func = std::less<T>()>
    deque<Container, T, InternalContainer, SEGSIZE>* merge(const deque<Container, T, InternalContainer, SEGSIZE>* b, Func func) const;
    using value_type = T;
};

#include "deque.tpp"