#include "assertions.hpp"
#include "tests.hpp"
#include <vector>
#include <iterator>

constexpr size_t DEQUE_SEGSIZE = 8;

template<typename _fixture>
void insert_empty_begin(_fixture& fix) {
    fix.insert(fix.begin(), 42);
    EXPECT_EQ_SCALAR("[empty_begin] size", 1u, fix.size());
    EXPECT_EQ_SCALAR("[empty_begin] [0]", 42, fix[0]);
    EXPECT_EQ_SCALAR("[empty_begin] front/back", 42, fix.back());
}

template<typename _fixture>
void insert_empty_end(_fixture& fix) {
    fix.insert(fix.end(), 42);
    EXPECT_EQ_SCALAR("[empty_end] size", 1u, fix.size());
    EXPECT_EQ_SCALAR("[empty_end] [0]", 42, fix[0]);
    EXPECT_EQ_SCALAR("[empty_end] front/back", 42, fix.back());
}

template<typename _fixture>
void insert_begin_nonempty(_fixture& fix) {
    fix.push_back(1); fix.push_back(2); fix.push_back(3);
    fix.insert(fix.begin(), 0);
    std::vector<int> exp = {0, 1, 2, 3};
    EXPECT_EQ_SCALAR("[begin_nonempty] size", 4u, fix.size());
    EXPECT_EQ_RANGE("[begin_nonempty] content", exp, fix);
}

template<typename _fixture>
void insert_end_nonempty(_fixture& fix) {
    fix.push_back(1); fix.push_back(2); fix.push_back(3);
    fix.insert(fix.end(), 4);
    std::vector<int> exp = {1, 2, 3, 4};
    EXPECT_EQ_SCALAR("[end_nonempty] size", 4u, fix.size());
    EXPECT_EQ_RANGE("[end_nonempty] content", exp, fix);
}

template<typename _fixture>
void insert_middle_single_segment(_fixture& fix) {
    for (size_t i = 0; i < 4; ++i) fix.push_back(static_cast<int>(i));
    auto it = fix.begin();
    std::advance(it, 2);
    fix.insert(it, 99);
    std::vector<int> exp = {0, 1, 99, 2, 3};
    EXPECT_EQ_SCALAR("[mid_single] size", 5u, fix.size());
    EXPECT_EQ_RANGE("[mid_single] content", exp, fix);
}

template<typename _fixture>
void insert_full_segment_split(_fixture& fix) {
    for (size_t i = 0; i < DEQUE_SEGSIZE; ++i) fix.push_back(static_cast<int>(i));
    auto it = fix.begin();
    std::advance(it, DEQUE_SEGSIZE / 2);
    fix.insert(it, 88);
    std::vector<int> exp = {0,1,2,3,88,4,5,6,7};
    EXPECT_EQ_SCALAR("[split] size", DEQUE_SEGSIZE + 1, fix.size());
    EXPECT_EQ_RANGE("[split] content", exp, fix);
}

template<typename _fixture>
void insert_after_push_front(_fixture& fix) {
    fix.push_front(20); fix.push_front(10);
    fix.push_back(30); fix.push_back(40);
    auto it = fix.begin();
    std::advance(it, 2);
    fix.insert(it, 99);
    std::vector<int> exp = {10, 20, 99, 30, 40};
    EXPECT_EQ_SCALAR("[after_pf] size", 5u, fix.size());
    EXPECT_EQ_RANGE("[after_pf] content", exp, fix);
}

template<typename _fixture>
void insert_stress_test(_fixture& fix) {
    std::vector<int> exp = {0, 1, 2};
    fix.push_back(0); fix.push_back(1); fix.push_back(2);
    for (int i = 10; i < 60; ++i) {
        auto it = fix.begin();
        std::advance(it, fix.size() / 2);
        fix.insert(it, i);
        exp.insert(exp.begin() + exp.size()/2, i);
    }
    EXPECT_EQ_SCALAR("[stress] size", exp.size(), fix.size());
    EXPECT_EQ_RANGE("[stress] content", exp, fix);
}

// Empty Begin/End
TEST_F(deque_array_seq_fixture, DqAS_insert_empty_begin) { insert_empty_begin(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_insert_empty_begin) { insert_empty_begin(_fix); }
TEST_F(deque_vector_fixture, DqVec_insert_empty_begin) { insert_empty_begin(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_insert_empty_end) { insert_empty_end(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_insert_empty_end) { insert_empty_end(_fix); }
TEST_F(deque_vector_fixture, DqVec_insert_empty_end) { insert_empty_end(_fix); }

// Non-empty Begin/End
TEST_F(deque_array_seq_fixture, DqAS_insert_begin_nonempty) { insert_begin_nonempty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_insert_begin_nonempty) { insert_begin_nonempty(_fix); }
TEST_F(deque_vector_fixture, DqVec_insert_begin_nonempty) { insert_begin_nonempty(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_insert_end_nonempty) { insert_end_nonempty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_insert_end_nonempty) { insert_end_nonempty(_fix); }
TEST_F(deque_vector_fixture, DqVec_insert_end_nonempty) { insert_end_nonempty(_fix); }

// Middle & Split
TEST_F(deque_array_seq_fixture, DqAS_insert_mid_single) { insert_middle_single_segment(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_insert_mid_single) { insert_middle_single_segment(_fix); }
TEST_F(deque_vector_fixture, DqVec_insert_mid_single) { insert_middle_single_segment(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_insert_split) { insert_full_segment_split(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_insert_split) { insert_full_segment_split(_fix); }
TEST_F(deque_vector_fixture, DqVec_insert_split) { insert_full_segment_split(_fix); }

// Offset & Stress
TEST_F(deque_array_seq_fixture, DqAS_insert_after_pf) { insert_after_push_front(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_insert_after_pf) { insert_after_push_front(_fix); }
TEST_F(deque_vector_fixture, DqVec_insert_after_pf) { insert_after_push_front(_fix); } // Примечание: исправлено имя

TEST_F(deque_array_seq_fixture, DqAS_insert_stress) { insert_stress_test(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_insert_stress) { insert_stress_test(_fix); }
TEST_F(deque_vector_fixture, DqVec_insert_stress) { insert_stress_test(_fix); }