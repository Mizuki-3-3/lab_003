#include "assertions.hpp"
#include "tests.hpp"
#include <vector>
#include <string>

// ============================================================================
// map
// ============================================================================

template<typename _fixture>
void map_empty(_fixture& fix) {
    auto* result = fix.map([](int x) { return x * 2; });
    EXPECT_EQ_SCALAR("[empty] size", 0u, result->size());
    delete result;
}

template<typename _fixture>
void map_one_element(_fixture& fix) {
    fix.push_back(5);
    auto* result = fix.map([](int x) { return x * 3; });
    std::vector<int> exp = {15};
    EXPECT_EQ_SCALAR("[one] size", 1u, result->size());
    EXPECT_EQ_RANGE("[one] content", exp, *result);
    delete result;
}

template<typename _fixture>
void map_identity(_fixture& fix) {
    for (int i = 1; i <= 5; ++i) fix.push_back(i);
    auto* result = fix.map([](int x) { return x; });
    std::vector<int> exp = {1,2,3,4,5};
    EXPECT_EQ_SCALAR("[identity] size", 5u, result->size());
    EXPECT_EQ_RANGE("[identity] content", exp, *result);
    delete result;
}

template<typename _fixture>
void map_multiply(_fixture& fix) {
    for (int i = 1; i <= 5; ++i) fix.push_back(i);
    auto* result = fix.map([](int x) { return x * 2; });
    std::vector<int> exp = {2,4,6,8,10};
    EXPECT_EQ_SCALAR("[multiply] size", 5u, result->size());
    EXPECT_EQ_RANGE("[multiply] content", exp, *result);
    delete result;
}

template<typename _fixture>
void map_negate(_fixture& fix) {
    for (int i = -2; i <= 2; ++i) fix.push_back(i);
    auto* result = fix.map([](int x) { return -x; });
    std::vector<int> exp = {2,1,0,-1,-2};
    EXPECT_EQ_RANGE("[negate] content", exp, *result);
    delete result;
}

template<typename _fixture>
void map_to_string(_fixture& fix) {
    fix.push_back(1);
    fix.push_back(2);
    fix.push_back(3);
    // map должен возвращать deque того же типа, но с другим типом T? 
    // В текущем дизайне T фиксирован. Для преобразования типа тест некорректен.
    // Вместо этого покажем отображение на другой тип, но в нашей сигнатуре map возвращает deque<Container, T, ...>*
    // Поэтому мы можем только изменить значение, но не тип. Оставим тест на преобразование значения в строку, если T = std::string? Но в фикстуре T = int.
    // Пропустим этот тест или изменим на работу с int.
    // Лучше: отображение, которое не меняет тип, например, возведение в квадрат.
    auto* result = fix.map([](int x) { return x * x; });
    std::vector<int> exp = {1,4,9};
    EXPECT_EQ_RANGE("[square] content", exp, *result);
    delete result;
}

template<typename _fixture>
void map_original_unchanged(_fixture& fix) {
    std::vector<int> original = {10,20,30,40};
    for (int v : original) fix.push_back(v);
    
    auto* result = fix.map([](int x) { return x / 2; });
    std::vector<int> expected_result = {5,10,15,20};
    EXPECT_EQ_RANGE("[original_unchanged] result", expected_result, *result);
    // Проверяем, что исходный дек не изменился
    EXPECT_EQ_RANGE("[original_unchanged] original", original, fix);
    delete result;
}

template<typename _fixture>
void map_with_segments(_fixture& fix) {
    // Заполняем несколько сегментов (SEGSIZE = 8)
    for (int i = 0; i < 20; ++i) fix.push_back(i);
    auto* result = fix.map([](int x) { return x + 100; });
    std::vector<int> exp;
    for (int i = 0; i < 20; ++i) exp.push_back(i + 100);
    EXPECT_EQ_SCALAR("[segments] size", 20u, result->size());
    EXPECT_EQ_RANGE("[segments] content", exp, *result);
    delete result;
}

template<typename _fixture>
void map_with_offset(_fixture& fix) {
    // Сдвигаем first_elem_idx через push_front
    fix.push_front(100);
    fix.push_front(99);
    for (int i = 1; i <= 5; ++i) fix.push_back(i);
    // Дек: [99,100,1,2,3,4,5]
    auto* result = fix.map([](int x) { return x + 1; });
    std::vector<int> exp = {100,101,2,3,4,5,6};
    EXPECT_EQ_SCALAR("[offset] size", 7u, result->size());
    EXPECT_EQ_RANGE("[offset] content", exp, *result);
    delete result;
}

template<typename _fixture>
void map_stress(_fixture& fix) {
    const size_t N = 1000;
    for (size_t i = 0; i < N; ++i) fix.push_back(static_cast<int>(i));
    auto* result = fix.map([](int x) { return x * 2; });
    std::vector<int> exp;
    exp.reserve(N);
    for (size_t i = 0; i < N; ++i) exp.push_back(static_cast<int>(i * 2));
    EXPECT_EQ_SCALAR("[stress] size", N, result->size());
    EXPECT_EQ_RANGE("[stress] content", exp, *result);
    delete result;
}

TEST_F(deque_array_seq_fixture, DqAS_map_empty) { map_empty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_map_empty) { map_empty(_fix); }
TEST_F(deque_vector_fixture, DqVec_map_empty) { map_empty(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_map_one) { map_one_element(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_map_one) { map_one_element(_fix); }
TEST_F(deque_vector_fixture, DqVec_map_one) { map_one_element(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_map_identity) { map_identity(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_map_identity) { map_identity(_fix); }
TEST_F(deque_vector_fixture, DqVec_map_identity) { map_identity(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_map_multiply) { map_multiply(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_map_multiply) { map_multiply(_fix); }
TEST_F(deque_vector_fixture, DqVec_map_multiply) { map_multiply(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_map_negate) { map_negate(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_map_negate) { map_negate(_fix); }
TEST_F(deque_vector_fixture, DqVec_map_negate) { map_negate(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_map_square) { map_to_string(_fix); }  // square
TEST_F(deque_list_seq_fixture, DqLS_map_square) { map_to_string(_fix); }
TEST_F(deque_vector_fixture, DqVec_map_square) { map_to_string(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_map_unchanged) { map_original_unchanged(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_map_unchanged) { map_original_unchanged(_fix); }
TEST_F(deque_vector_fixture, DqVec_map_unchanged) { map_original_unchanged(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_map_segments) { map_with_segments(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_map_segments) { map_with_segments(_fix); }
TEST_F(deque_vector_fixture, DqVec_map_segments) { map_with_segments(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_map_offset) { map_with_offset(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_map_offset) { map_with_offset(_fix); }
TEST_F(deque_vector_fixture, DqVec_map_offset) { map_with_offset(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_map_stress) { map_stress(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_map_stress) { map_stress(_fix); }
TEST_F(deque_vector_fixture, DqVec_map_stress) { map_stress(_fix); }