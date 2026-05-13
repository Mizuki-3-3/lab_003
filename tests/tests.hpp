#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "assertions.hpp"
#include "deque.hpp"
#include "list_seq.hpp"
#include "array_seq.hpp"
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

#define TEST(test_name) \
    static void test_name##_test(); \
    static void __attribute__((constructor)) test_name##_init() { \
        register_test(#test_name, test_name##_test); \
    } \
    static void test_name##_test()

#define TEST_ENTRY_POINT \
    int main(void) { \
        run_test(); \
        int result = print_stats(); \
        cleanup_tests(); \
        return result; \
    }

template <template<typename> class Container>
struct deque_fixture {
    deque<Container, int>* dq;
    deque_fixture() : dq(new deque<Container, int>()) {}
    ~deque_fixture() { delete dq; }
};

using deque_list_seq_fixture = deque_fixture<list_seq>;
using deque_array_seq_fixture = deque_fixture<array_seq>;

