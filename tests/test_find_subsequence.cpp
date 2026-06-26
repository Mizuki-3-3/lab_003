#include "assertions.hpp"
#include "tests.hpp"

template <template<typename> class Container>
void test_find_subsequence() {
    deque_fixture<Container> main, pattern;
    for (int x : {1,2,3,1,2,4,5}) main.dq->push_back(x);
    for (int x : {1,2,4}) pattern.dq->push_back(x);
    size_t pos = main.dq->find_subsequence(*pattern.dq);
    EXPECT_EQ(3u, pos, "pattern at index 3");
}

template <template<typename> class Container>
void test_find_subsequence_not_found() {
    deque_fixture<Container> main, pattern;
    main.dq->push_back(1)->push_back(2)->push_back(3);
    pattern.dq->push_back(4)->push_back(5);

    bool caught = false;
    try {
        main.dq->find_subsequence(*pattern.dq);
    } catch (const not_found&) {
        caught = true;
    }
    EXPECT_TRUE(caught, "find_subsequence should throw not_found when pattern not present");
}

TEST(find_subsequence_list_seq) {
    test_find_subsequence<list_seq>();
}

TEST(find_subsequence_array_seq) {
    test_find_subsequence<array_seq>();
}

TEST(find_subsequence_not_found_list_seq) {
    test_find_subsequence_not_found<list_seq>();
}

TEST(find_subsequence_not_found_array_seq) {
    test_find_subsequence_not_found<array_seq>();
}