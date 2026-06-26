#include "assertions.hpp"
#include "tests.hpp"
#include <vector>
#include <algorithm>

// SEGSIZE по умолчанию из deque.hpp. Если меняете его в реализации, обновите и здесь.
constexpr size_t DEQUE_SEGSIZE = 8;

template<typename _fixture>
void push_front_empty(_fixture& fix) {
    fix.push_front(42);
    EXPECT_EQ_SCALAR("push front 42 in empty size", 1u, fix.size());
    EXPECT_EQ_SCALAR("push front 42 in empty front", 42, fix.front());
    EXPECT_EQ_SCALAR("push front 42 in empty back", 42, fix.back());
    EXPECT_EQ_SCALAR("push front 42 in empty operator[]", 42, fix[0]);
}

template<typename _fixture>
void push_front_fill_segment(_fixture& fix) {
    std::vector<int> exp;
    for (size_t i = 0; i < DEQUE_SEGSIZE; ++i) {
        fix.push_front(static_cast<int>(i));
        exp.insert(exp.begin(), static_cast<int>(i));
    }
    EXPECT_EQ_SCALAR("push front {7,6,5,4,3,2,1,0} size", DEQUE_SEGSIZE, fix.size());
    EXPECT_EQ_SCALAR("push front {7,6,5,4,3,2,1,0} front", 7, fix.front());
    EXPECT_EQ_SCALAR("push front {7,6,5,4,3,2,1,0} back", 0, fix.back());
    EXPECT_EQ_RANGE("push front {7,6,5,4,3,2,1,0} content", exp, fix);
}

template<typename _fixture>
void push_front_cross_boundary(_fixture& fix) {
    size_t count = DEQUE_SEGSIZE + 1;
    std::vector<int> exp;
    exp.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        fix.push_front(static_cast<int>(i));
        exp.insert(exp.begin(), static_cast<int>(i));
    }
    EXPECT_EQ_SCALAR("push front elem from 0 to seg_size size", count, fix.size());
    EXPECT_EQ_SCALAR("push front elem from 0 to seg_size front", static_cast<int>(count - 1), fix.front());
    EXPECT_EQ_SCALAR("push front elem from 0 to seg_size back", 0, fix.back());
    EXPECT_EQ_RANGE("push front elem from 0 to seg_size content", exp, fix);
}

template<typename _fixture>
void push_front_multiple_segments(_fixture& fix) {
    size_t count = 3 * DEQUE_SEGSIZE + 5;
    std::vector<int> exp;
    exp.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        fix.push_front(static_cast<int>(i));
        exp.insert(exp.begin(), static_cast<int>(i));
    }
    EXPECT_EQ_SCALAR("push front elem from 0 to seg size+1 size", count, fix.size());
    EXPECT_EQ_SCALAR("push front elem from 0 to seg size+1 front", static_cast<int>(count - 1), fix.front());
    EXPECT_EQ_SCALAR("push front elem from 0 to seg size+1 back", 0, fix.back());
    
    EXPECT_EQ_SCALAR("push front elem from 0 to seg size+1 idx_seg1_start", 21, fix[DEQUE_SEGSIZE - 1]);
    EXPECT_EQ_SCALAR("push front elem from 0 to seg size+1 idx_seg2_start", 20, fix[DEQUE_SEGSIZE]);
    EXPECT_EQ_RANGE("push front elem from 0 to seg size+1 content", exp, fix);
}

template<typename _fixture>
void push_front_after_push_back(_fixture& fix) {
    fix.push_back(100);
    fix.push_back(101);

    fix.push_front(42);
    fix.push_front(43);

    EXPECT_EQ_SCALAR("pf(42,43) after pb(100,101) size", 4u, fix.size());
    EXPECT_EQ_SCALAR("pf(42,43) after pb(100,101) front", 43, fix.front());
    EXPECT_EQ_SCALAR("pf(42,43) after pb(100,101) back", 101, fix.back());

    std::vector<int> exp = {43, 42, 100, 101};
    EXPECT_EQ_RANGE("pf(42,43) after pb(100,101) content", exp, fix);
}

template<typename _fixture>
void push_front_stress_test(_fixture& fix) {
    size_t count = 1000;
    std::vector<int> exp;
    exp.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        fix.push_front(static_cast<int>(i));
        exp.insert(exp.begin(), static_cast<int>(i));
    }
    EXPECT_EQ_SCALAR("push front from 0 to 999 size", count, fix.size());
    EXPECT_EQ_SCALAR("push front from 0 to 999 front", static_cast<int>(count-1), fix.front());
    EXPECT_EQ_SCALAR("push front from 0 to 999 back", 0, fix.back());
    EXPECT_EQ_SCALAR("push front from 0 to 999 mid", 499, fix[500]);
    EXPECT_EQ_RANGE("push front from 0 to 999 content", exp, fix);
}
//empty
TEST_F(deque_array_seq_fixture, DqAS_push_front_empty) { push_front_empty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_push_front_empty) { push_front_empty(_fix); }
TEST_F(deque_vector_fixture, DqVec_push_front_empty) { push_front_empty(_fix); }

//fillseg
TEST_F(deque_array_seq_fixture, DqAS_push_front_fill_segment) { push_front_fill_segment(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_push_front_fill_segment) { push_front_fill_segment(_fix); }
TEST_F(deque_vector_fixture, DqVec_push_front_fill_segment) { push_front_fill_segment(_fix); }

//crossboundary
TEST_F(deque_array_seq_fixture, DqAS_push_front_cross_boundary) { push_front_cross_boundary(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_push_front_cross_boundary) { push_front_cross_boundary(_fix); }
TEST_F(deque_vector_fixture, DqVec_push_front_cross_boundary) { push_front_cross_boundary(_fix); }

//mult
TEST_F(deque_array_seq_fixture, DqAS_push_front_multiple_segments) { push_front_multiple_segments(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_push_front_multiple_segments) { push_front_multiple_segments(_fix); }
TEST_F(deque_vector_fixture, DqVec_push_front_multiple_segments) { push_front_multiple_segments(_fix); }

//with push back
TEST_F(deque_array_seq_fixture, DqAS_push_front_after_push_back) { push_front_after_push_back(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_push_front_after_push_back) { push_front_after_push_back(_fix); }
TEST_F(deque_vector_fixture, DqVec_push_front_after_push_back) { push_front_after_push_back(_fix); }

//stress
TEST_F(deque_array_seq_fixture, DqAS_push_front_stress) { push_front_stress_test(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_push_front_stress) { push_front_stress_test(_fix); }
TEST_F(deque_vector_fixture, DqVec_push_front_stress) { push_front_stress_test(_fix); }