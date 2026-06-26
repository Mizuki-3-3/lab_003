#include "assertions.hpp"
#include "tests.hpp"

template <template<typename> class Container>
void test_find_existing() {
    deque_fixture<Container> fix;
    for (int i = 0; i < 10; ++i) fix.dq->push_back(i * 2);
    unsigned idx = fix.dq->find(6);
    EXPECT_EQ(3u, idx, "find 6");
}

template <template<typename> class Container>
void test_find_missing() {
    deque_fixture<Container> fix;
    fix.dq->push_back(1)->push_back(2);
    
    bool caught = false;
    try {
        fix.dq->find(99);
    } catch (const not_found&) {
        caught = true;
    }
    EXPECT_TRUE(caught, "find should throw not_found when pattern not present");
}

TEST(find_existing_list_seq) {
    test_find_existing<list_seq>();
}

TEST(find_existing_array_seq) {
    test_find_existing<array_seq>();
}

TEST(find_missing_list_seq) {
    test_find_missing<list_seq>();
}

TEST(find_missing_array_seq) {
    test_find_missing<array_seq>();
}