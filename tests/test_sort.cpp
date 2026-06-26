#include "assertions.hpp"
#include "tests.hpp"

template <typename _fixture>
void test_sort(_fixture& fix) {
    fix.push_back(3); fix.push_back(1); fix.push_back(4); fix.push_back(2);
    fix.sort();
    std::vector<int> exp{1,2,3,4};
    EXPECT_EQ_RANGE("sort {3,1,4,2}", exp, fix);
}

template <typename _fixture>
void test_sort_empty(_fixture& fix) {
    fix.sort();
    std::vector<int> exp;
    EXPECT_EQ_RANGE("sort {}", exp, fix);
}

template <typename _fixture>
void test_sort_single_elem(_fixture& fix) {
    fix.push_back(42);
    fix.sort();
    std::vector<int> exp{42};
    EXPECT_EQ_RANGE("sort {42}", exp, fix);
}

template <typename _fixture>
void test_sort_2_elems(_fixture& fix) {
    fix.push_back(42); fix.push_back(32);
    fix.sort();
    std::vector<int> exp{32,42};
    EXPECT_EQ_RANGE("sort {42, 32}", exp, fix);
}

template <typename _fixture>
void test_sort_count_of_elems_more_than_1_seg(_fixture& fix) {
    fix.push_back(42); fix.push_back(32); fix.push_back(22);fix.push_back(52);
    fix.push_back(62); fix.push_back(62); fix.push_back(62); fix.push_back(0); fix.push_back(12);
    fix.sort();
    std::vector<int> exp{0,12,22,32,42,52,62,62,62};
    EXPECT_EQ_RANGE("sort {42,32,22,52,62,62,62,0,12}", exp, fix);
}

TEST_F(deque_array_seq_fixture, DqAS_simple_sort){test_sort(_fix);}
TEST_F(deque_list_seq_fixture, DqLS_simple_sort){test_sort(_fix);}
TEST_F(deque_vector_fixture, DqVec_simple_sort){test_sort(_fix);}

TEST_F(deque_array_seq_fixture, DqAS_empty_sort){test_sort_empty(_fix);}
TEST_F(deque_list_seq_fixture, DqLS_empty_sort){test_sort_empty(_fix);}
TEST_F(deque_vector_fixture, DqVec_empty_sort){test_sort_empty(_fix);}

TEST_F(deque_array_seq_fixture, DqAS_single_elem_sort){test_sort_single_elem(_fix);}
TEST_F(deque_list_seq_fixture, DqLS_single_elem_sort){test_sort_single_elem(_fix);}
TEST_F(deque_vector_fixture, DqVec_single_elem_sort){test_sort_single_elem(_fix);}

TEST_F(deque_array_seq_fixture, DqAS_2_elems_sort){test_sort_2_elems(_fix);}
TEST_F(deque_list_seq_fixture, DqLS_2_elems_sort){test_sort_2_elems(_fix);}
TEST_F(deque_vector_fixture, DqVec_2_elems_sort){test_sort_2_elems(_fix);}

TEST_F(deque_array_seq_fixture, DqAS_count_of_elems_more_than_1_seg_sort){test_sort_count_of_elems_more_than_1_seg(_fix);}
TEST_F(deque_list_seq_fixture, DqLS_count_of_elems_more_than_1_seg_sort){test_sort_count_of_elems_more_than_1_seg(_fix);}
TEST_F(deque_vector_fixture, DqVec_count_of_elems_more_than_1_seg_sort){test_sort_count_of_elems_more_than_1_seg(_fix);}
