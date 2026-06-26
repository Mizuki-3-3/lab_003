#include "assertions.hpp"
#include "tests.hpp"
#include <vector>

template<typename _fixture>
void where_empty(_fixture& fix) {
    auto result = fix.where([](int) { return true; });
    EXPECT_EQ_SCALAR("[empty] size", 0u, result->size());
}

template<typename _fixture>
void where_all_match(_fixture& fix) {
    for (int i = 1; i <= 5; ++i) fix.push_back(i);
    auto result = fix.where([](int x) { return x > 0; });
    std::vector<int> exp = {1,2,3,4,5};
    EXPECT_EQ_SCALAR("{1,2,3,4,5} where x>5 size", 5u, result->size());
    EXPECT_EQ_RANGE("{1,2,3,4,5} where x>5 elems", exp, *result);
}

template<typename _fixture>
void where_none_match(_fixture& fix) {
    for (int i = 1; i <= 5; ++i) fix.push_back(i);
    auto result = fix.where([](int x) { return x > 10; });
    EXPECT_EQ_SCALAR("none_match size", 0u, result->size());
}

template<typename _fixture>
void where_some_match(_fixture& fix) {
    for (int i = 1; i <= 10; ++i) fix.push_back(i);
    auto result = fix.where([](int x) { return x % 2 == 0; });
    std::vector<int> exp = {2,4,6,8,10};
    EXPECT_EQ_SCALAR("{1,2,...,9,10} where even size", 5u, result->size());
    EXPECT_EQ_RANGE("{1,2,...,9,10} where even elems", exp, *result);
}

template<typename _fixture>
void where_original_unchanged(_fixture& fix) {
    std::vector<int> original = {1,2,3,4,5};
    for (int v : original) fix.push_back(v);
    
    auto result = fix.where([](int x) { return x > 2; });
    std::vector<int> expected_result = {3,4,5};
    EXPECT_EQ_RANGE("[original_unchanged] result", expected_result, *result);
    EXPECT_EQ_RANGE("[original_unchanged] original", original, fix);
}

template<typename _fixture>
void where_with_more_than_1_segment(_fixture& fix) {
    for (int i = 0; i < 20; ++i) fix.push_back(i);
    auto result = fix.where([](int x) { return x >= 5 && x < 15; });
    std::vector<int> exp;
    for (int i = 5; i < 15; ++i) exp.push_back(i);
    EXPECT_EQ_SCALAR("{1,2,...,18,19,20} where x >= 5 && x < 15 size", 10u, result->size());
    EXPECT_EQ_RANGE("{1,2,...,18,19,20} where x >= 5 && x < 15 elems", exp, *result);
}

template<typename _fixture>
void where_with_offset(_fixture& fix) {
    fix.push_front(100);fix.push_front(99);
    for (int i = 1; i <= 5; ++i) fix.push_back(i);

    auto result = fix.where([](int x) { return x > 3; });
    std::vector<int> exp = {99,100,4,5};
    EXPECT_EQ_SCALAR("deque 1 seg:{99,100}; 2seg:{3,4,5} where x>3 size", 4u, result->size());
    EXPECT_EQ_RANGE("deque 1 seg:{99,100}; 2seg:{3,4,5} where x>3 content", exp, *result);
}

TEST_F(deque_array_seq_fixture, DqAS_where_empty) { where_empty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_where_empty) { where_empty(_fix); }
TEST_F(deque_vector_fixture, DqVec_where_empty) { where_empty(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_where_all_match) { where_all_match(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_where_all_match) { where_all_match(_fix); }
TEST_F(deque_vector_fixture, DqVec_where_all_match) { where_all_match(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_where_none_match) { where_none_match(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_where_none_match) { where_none_match(_fix); }
TEST_F(deque_vector_fixture, DqVec_where_none_match) { where_none_match(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_where_some_match) { where_some_match(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_where_some_match) { where_some_match(_fix); }
TEST_F(deque_vector_fixture, DqVec_where_some_match) { where_some_match(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_where_original_unchanged) { where_original_unchanged(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_where_original_unchanged) { where_original_unchanged(_fix); }
TEST_F(deque_vector_fixture, DqVec_where_original_unchanged) { where_original_unchanged(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_where_segments) { where_with_more_than_1_segment(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_where_segments) { where_with_more_than_1_segment(_fix); }
TEST_F(deque_vector_fixture, DqVec_where_segments) { where_with_more_than_1_segment(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_where_offset) { where_with_offset(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_where_offset) { where_with_offset(_fix); }
TEST_F(deque_vector_fixture, DqVec_where_offset) { where_with_offset(_fix); }