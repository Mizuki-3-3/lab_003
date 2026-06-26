#include "assertions.hpp"
#include "tests.hpp"

template<typename _fixture>
void test_empty_subd(_fixture& fix){
    fix.make_nonempty_1seg();
    auto res = fix.subdeque(4u,4u);
    std::vector<int> exp;
    EXPECT_EQ_SCALAR("", exp.size(), (*res).size());
}

TEST_F(deque_array_seq_fixture, DqAS_test_empty_subd) { test_empty_subd(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_test_empty_subd) { test_empty_subd(_fix); }
TEST_F(deque_vector_fixture, DqVec_test_empty_subd) { test_empty_subd(_fix); }