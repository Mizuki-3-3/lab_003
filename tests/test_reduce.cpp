#include "assertions.hpp"
#include "tests.hpp"
#include <vector>
#include <string>

struct SumAndProd {
    int sum;
    int prod;
    SumAndProd(int s = 0, int p = 1) : sum(s), prod(p) {}
    bool operator==(const SumAndProd& other) const {
        return sum == other.sum && prod == other.prod;
    }
};

template<typename _fixture>
void reduce_empty(_fixture& fix) {
    auto result = fix.reduce(0, [](int acc, int x) { return acc + x; });
    EXPECT_EQ_SCALAR("[empty] sum init 0", 0, result);
    
    auto result2 = fix.reduce(10, [](int acc, int x) { return acc + x; });
    EXPECT_EQ_SCALAR("[empty] sum init 10", 10, result2);
    
    std::string init = "start";
    auto result3 = fix.reduce(init, [](std::string acc, int x) { return acc + std::to_string(x); });
    EXPECT_TRUE("[empty] string", result3 == "start");
}

template<typename _fixture>
void reduce_one_element(_fixture& fix) {
    fix.push_back(42);
    
    auto sum = fix.reduce(0, [](int acc, int x) { return acc + x; });
    EXPECT_EQ_SCALAR("{42} sum", 42, sum);
    
    auto prod = fix.reduce(1, [](int acc, int x) { return acc * x; });
    EXPECT_EQ_SCALAR("{42} product", 42, prod);
    
    auto concat = fix.reduce(std::string(""), [](std::string acc, int x) { return acc + std::to_string(x); });
    EXPECT_TRUE("{42} concat", concat == "42");
}

template<typename _fixture>
void reduce_sum_prod(_fixture& fix) {
    for (int i = 1; i <= 5; ++i) fix.push_back(i);
    
    auto sum = fix.reduce(0, [](int acc, int x) { return acc + x; });
    EXPECT_EQ_SCALAR("{1,2,3,4,5} sum", 15, sum);
    
    auto prod = fix.reduce(1, [](int acc, int x) { return acc * x; });
    EXPECT_EQ_SCALAR("{1,2,3,4,5} prod", 120, prod);
}

template<typename _fixture>
void reduce_order_subtraction(_fixture& fix) {
    fix.push_back(10);
    fix.push_back(2);
    fix.push_back(3);
    auto result = fix.reduce(0, [](int acc, int x) { 
        static bool first = true;
        if (first) { first = false; return x; } // первый элемент как начальный
        return acc - x;
    });
    auto result2 = fix.reduce(10, [](int acc, int x) { return acc - x; });
    EXPECT_EQ_SCALAR("{10,2,3} sub with init = 0", 5, result);
    EXPECT_EQ_SCALAR("{10,2,3} sub with init = 10", -5, result2);
}

template<typename _fixture>
void reduce_string_concat(_fixture& fix) {
    fix.push_back(1);
    fix.push_back(2);
    fix.push_back(3);
    
    auto result = fix.reduce(std::string("["), [](std::string acc, int x) {
        return acc + std::to_string(x) + ",";
    });
    EXPECT_TRUE("[string] concat", result == "[1,2,3,");
    
    std::vector<int> init_vec;
    auto vec_result = fix.reduce(init_vec, [](std::vector<int> acc, int x) {
        acc.push_back(x);
        return acc;
    });
    std::vector<int> expected = {1,2,3};
    EXPECT_EQ_RANGE("[vector] reduce", expected, vec_result);
}

//Разные типы аккумулятора: подсчёт количества элементов
template<typename _fixture>
void reduce_count(_fixture& fix) {
    for (int i = 0; i < 10; ++i) fix.push_back(i);
    auto count = fix.reduce(0u, [](size_t acc, int) { return acc + 1; });
    EXPECT_EQ_SCALAR("[count] size", 10u, count);
}

// Сложный аккумулятор (структура)
template<typename _fixture>
void reduce_complex_accum(_fixture& fix) {
    fix.push_back(1);
    fix.push_back(2);
    fix.push_back(3);
    fix.push_back(4);
    
    auto result = fix.reduce(SumAndProd(0, 1), [](SumAndProd acc, int x) {
        return SumAndProd(acc.sum + x, acc.prod * x);
    });
    EXPECT_EQ_SCALAR("[complex] sum", 10, result.sum);
    EXPECT_EQ_SCALAR("[complex] prod", 24, result.prod);
}

// Тест со сдвигом first_elem_idx (после push_front)
template<typename _fixture>
void reduce_with_offset(_fixture& fix) {
    fix.push_front(100);
    fix.push_front(99);
    fix.push_back(1);
    fix.push_back(2);
    
    auto sum = fix.reduce(0, [](int acc, int x) { return acc + x; });
    EXPECT_EQ_SCALAR("[offset] sum", 99+100+1+2, sum);
    
    auto concat = fix.reduce(std::string(""), [](std::string acc, int x) {
        return acc + std::to_string(x) + " ";
    });
    EXPECT_TRUE("[offset] concat", concat == "99 100 1 2 ");
}

template<typename _fixture>
void reduce_multiple_segments(_fixture& fix) {
    constexpr size_t SEG = 8;
    for (size_t i = 0; i < 3 * SEG + 5; ++i) fix.push_back(static_cast<int>(i));
    
    auto sum = fix.reduce(0ll, [](long long acc, int x) { return acc + x; });
    long long expected = (3*SEG+4)*(3*SEG+5)/2;
    EXPECT_EQ_SCALAR("from 0 to 28 sum", expected, sum);
}

template<typename _fixture>
void reduce_stress(_fixture& fix) {
    const size_t N = 1000;
    for (size_t i = 0; i < N; ++i) fix.push_back(static_cast<int>(i));
    
    auto sum = fix.reduce(0ll, [](long long acc, int x) { return acc + x; });
    long long expected = (N-1) * N / 2;
    EXPECT_EQ_SCALAR("from 1 to 1000 sum", expected, sum);

    auto prod = fix.reduce(1ll, [](long long acc, int x) { return acc * x; });
    EXPECT_TRUE("from 1 to 1000 prod not zero", prod != 0);
}

TEST_F(deque_array_seq_fixture, DqAS_reduce_empty) { reduce_empty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_reduce_empty) { reduce_empty(_fix); }
TEST_F(deque_vector_fixture, DqVec_reduce_empty) { reduce_empty(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_reduce_one) { reduce_one_element(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_reduce_one) { reduce_one_element(_fix); }
TEST_F(deque_vector_fixture, DqVec_reduce_one) { reduce_one_element(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_reduce_sum_prod) { reduce_sum_prod(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_reduce_sum_prod) { reduce_sum_prod(_fix); }
TEST_F(deque_vector_fixture, DqVec_reduce_sum_prod) { reduce_sum_prod(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_reduce_order) { reduce_order_subtraction(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_reduce_order) { reduce_order_subtraction(_fix); }
TEST_F(deque_vector_fixture, DqVec_reduce_order) { reduce_order_subtraction(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_reduce_string) { reduce_string_concat(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_reduce_string) { reduce_string_concat(_fix); }
TEST_F(deque_vector_fixture, DqVec_reduce_string) { reduce_string_concat(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_reduce_count) { reduce_count(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_reduce_count) { reduce_count(_fix); }
TEST_F(deque_vector_fixture, DqVec_reduce_count) { reduce_count(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_reduce_complex) { reduce_complex_accum(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_reduce_complex) { reduce_complex_accum(_fix); }
TEST_F(deque_vector_fixture, DqVec_reduce_complex) { reduce_complex_accum(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_reduce_offset) { reduce_with_offset(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_reduce_offset) { reduce_with_offset(_fix); }
TEST_F(deque_vector_fixture, DqVec_reduce_offset) { reduce_with_offset(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_reduce_segments) { reduce_multiple_segments(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_reduce_segments) { reduce_multiple_segments(_fix); }
TEST_F(deque_vector_fixture, DqVec_reduce_segments) { reduce_multiple_segments(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_reduce_stress) { reduce_stress(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_reduce_stress) { reduce_stress(_fix); }
TEST_F(deque_vector_fixture, DqVec_reduce_stress) { reduce_stress(_fix); }