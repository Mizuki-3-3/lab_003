#pragma once

#include <cstddef>
#include <errors.hpp>

template <template<typename> class Container, typename T>
class deque{

private:
    class segment {
    private:
        Container<T> data;
    public:
        segment();
        segment(const segment& other);
        ~segment() = default;

        size_t size() const;

        T& operator[](size_t idx);
        const T& operator[](size_t idx) const;
        Container<T>& operator*();
        Container<T>* operator->();
    
        auto begin() { return data.begin(); }
        auto end()   { return data.end(); }
        auto begin() const { return data.begin(); }
        auto end()   const { return data.end(); }
    };
    
    Container<segment*> map;
    static const size_t segment_size = 8;
    size_t first_elem_idx; //<segment_size
    size_t last_elem_idx;//<segment_size

    
public:
    deque();
    explicit deque(size_t initial_size);
    deque(const deque& other);
    deque(deque&& other) noexcept;
    ~deque();

    deque<Container, T>* push_back(const T& value) ;
    deque<Container, T>* push_front(const T& value) ;
    deque<Container, T>* insert(const T& item, unsigned index);//пользуемся insert и push_back

    T& operator[](size_t index) ;
    const T& operator[](size_t index) const ;

    T get_first() const;
    T get_last() const;
    size_t size() const;

    Container<T>::iterator begin();
    Container<T>::iterator end();
    Container<T>::const_iterator begin() const;
    Container<T>::const_iterator end() const;

    //какая-то попа

    unsigned find(const T& value)  const;
    void sort();
    template <typename Func>
    deque<Container, T>* map(Func func) const;
    template <typename Predicate>
    deque<Container, T>* where(Predicate pred) const;
    template <typename Acc, typename Func>
    Acc reduce(Acc init, Func func) const;
    deque<Container, T>* concat(const deque<Container, T>& other) const;
    deque<Container, T>* subdeque(unsigned start, unsigned end) const;
    size_t find_subsequence(const deque<Container, T>& pattern) const;
    bool less_than(T a, T b){return a < b;} //чтоб функция по умолчанию была
    template <typename Func>
    deque<Container, T>* merge(const deque<Container, T>* a, const deque<Container, T>* b, Func func = less_than);
};

#include "deque.tpp"
