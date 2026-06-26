#pragma once
#include "deque.hpp"
#include "list_seq.hpp"
#include "array_seq.hpp"
#include <memory>
#include <vector>

typedef void (*test_func)();

void register_test(const char* name, test_func test);
void run_test();
int print_stats();
void cleanup_tests();

typedef struct _test{
    const char* name;
    void (*test_func)(void);
    struct _test* next;
}_test;

template <typename F>
F& get_fixture() {
    static F instance;
    return instance;
}

#define TEST_F(fixture_type, test_name) \
    static void test_name##_test(fixture_type& _fix); \
    static void test_name##_runner(){ \
        fixture_type& _fix = get_fixture<fixture_type>(); \
        _fix.set_up(); \
        test_name##_test(_fix); \
        _fix.tear_down(); \
    } \
    static void __attribute__((constructor)) test_name##_init() { \
        register_test(#test_name, test_name##_runner); \
    } \
    static void test_name##_test(fixture_type& _fix)

#define TEST_ENTRY_POINT \
    int main(void) { \
        run_test(); \
        int result = print_stats(); \
        cleanup_tests(); \
        return result; \
    }

template <template<typename> class Container>
struct deque_fixture {
protected:
    deque<Container, int>* deq;
public:
    using iterator = deque<Container,int>::iterator;
    using const_iterator = deque<Container,int>::const_iterator;
    
    void set_up(){deq = new deque<Container, int>;}
    void tear_down(){
        delete deq;
        deq = nullptr;
    }
    deque_fixture() = default;
    ~deque_fixture() = default;
    deque<Container, int>* push_back(const int& value) {return deq->push_back(value);}
    deque<Container, int>* push_front(const int& value) {return deq->push_front(value);}
    deque<Container, int>* insert(const_iterator place, const int& item) {return deq->insert(place,item);}


    iterator begin(){return deq->begin();}
    const_iterator begin()const {return deq->begin();}

    iterator end(){return deq->end();}
    const_iterator end()const {return deq->end();}
    int& operator[](size_t index) {return (*deq)[index];}
    const int& operator[](size_t index) const {return (*deq)[index];}
    size_t size(){return deq->size();}

    iterator find(const int& value){return deq->find(value);}
    const_iterator find(const int& value) const{return deq->find(value);}
    int front() const{return deq->front();}
    int back() const{return deq->back();}

    void sort(){deq->sort();}
    template <typename Func>
    deque<Container, int>* map(Func func) const {return deq->map(func);}
    template <typename Func>
    std::unique_ptr<deque<Container, int>> where(Func pred) const{std::unique_ptr<deque<Container, int>> res(deq->where(pred));return res;}
    template <typename Acc, typename Func>
    Acc reduce(Acc init, Func func) const {return deq->reduce(init, func);}
    std::unique_ptr<deque<Container, int>> concat(const deque_fixture& other) const{std::unique_ptr<deque<Container, int>> res (deq->concat(*(other.deq))); return res;}
    std::unique_ptr<deque<Container, int>> subdeque(size_t start, size_t end) const {std::unique_ptr<deque<Container, int>> res (deq->subdeque(start, end)); return res;}
    iterator find_subsequence(const deque_fixture& pattern) {return deq->find_subsequence(*(pattern.deq));}
    const_iterator find_subsequence(const deque<Container, int>& pattern) const {return deq->find_subsequence(pattern);}
    template <typename Func = std::less<int>()>
    std::unique_ptr<deque<Container, int>> merge(const deque_fixture& other, Func func){return std::unique_ptr<deque<Container, int>>(deq->merge(other.deq, func));}
    
    void make_nonempty_1seg(){
        delete deq;
        deq = new deque<Container,int>{1,2,3,4,5,6,7,8};
    }
    void make_nonempty_1seg_pl_1elem(){
        delete deq;
        deq = new deque<Container,int>{1,2,3,4,5,6,7,8,9};
    }
    template <template<typename> class C>
    friend bool operator==(const deque_fixture<C>&, const std::vector<int>&);
    template <template<typename> class C>
    friend bool operator==(const std::vector<int>&, const deque_fixture<C>&);
};

template <template<typename> class Container>
inline bool operator==(const deque_fixture<Container>& left, const std::vector<int>& right) {
    if (left.deq->size() != right.size()) return false;
    for (size_t i = 0; i < right.size(); ++i) {
        if (left[i] != right[i]) return false;
    }
    return true;
}

template <template<typename> class Container>
inline bool operator==(const std::vector<int>& left, const deque_fixture<Container>& right) {
    if (right.deq->size() != left.size()) return false;
    for (size_t i = 0; i < left.size(); ++i) {
        if (right[i] != left[i]) return false;
    }
    return true;
}

template <template<typename> class Container>
inline bool operator==(const deque<Container,int>& left, const std::vector<int>& right) {
    if (left.size() != right.size()) return false;
    for (size_t i = 0; i < right.size(); ++i) {
        if (left[i] != right[i]) return false;
    }
    return true;
}

template <template<typename> class Container>
inline bool operator==(const std::vector<int>& left, const deque<Container,int>& right) {
    if (right.size() != left.size()) return false;
    for (size_t i = 0; i < left.size(); ++i) {
        if (right[i] != left[i]) return false;
    }
    return true;
}

using deque_list_seq_fixture = deque_fixture<list_seq>;
using deque_array_seq_fixture = deque_fixture<array_seq>;
using deque_vector_fixture = deque_fixture<std::vector>;

