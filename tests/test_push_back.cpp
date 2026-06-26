#include "assertions.hpp"
#include "tests.hpp"
#include <vector>

constexpr size_t DEQUE_SEGSIZE = 8;

template<typename _fixture>
void push_back_empty(_fixture& fix) {
    fix.push_back(42);
    EXPECT_EQ_SCALAR("[empty] size", 1u, fix.size());
    EXPECT_EQ_SCALAR("[empty] front", 42, fix.front());
    EXPECT_EQ_SCALAR("[empty] back", 42, fix.back());
    EXPECT_EQ_SCALAR("[empty] operator[]", 42, fix[0]);
}

template<typename _fixture>
void push_back_fill_segment(_fixture& fix) {
    std::vector<int> exp;
    for (size_t i = 0; i < DEQUE_SEGSIZE; ++i) {
        fix.push_back(static_cast<int>(i));
        exp.push_back(static_cast<int>(i));
    }
    EXPECT_EQ_SCALAR("[fill_segment] size", DEQUE_SEGSIZE, fix.size());
    EXPECT_EQ_SCALAR("[fill_segment] front", 0, fix.front());
    EXPECT_EQ_SCALAR("[fill_segment] back", 7, fix.back());
    EXPECT_EQ_RANGE("[fill_segment] content", exp, fix);
}

template<typename _fixture>
void push_back_cross_boundary(_fixture& fix) {
    // Добавляем DEQUE_SEGSIZE + 1 элемент, чтобы гарантировать аллокацию второго сегмента
    std::vector<int> exp;
    exp.reserve(DEQUE_SEGSIZE + 1);
    for (size_t i = 0; i <= DEQUE_SEGSIZE; ++i) {
        fix.push_back(static_cast<int>(i));
        exp.push_back(static_cast<int>(i));
    }
    EXPECT_EQ_SCALAR("[cross_boundary] size", DEQUE_SEGSIZE + 1, fix.size());
    EXPECT_EQ_SCALAR("[cross_boundary] front", 0, fix.front());
    EXPECT_EQ_SCALAR("[cross_boundary] back", 8, fix.back());
    EXPECT_EQ_RANGE("[cross_boundary] content", exp, fix);
}

template<typename _fixture>
void push_back_multiple_segments(_fixture& fix) {
    // 3 полных сегмента + 5 элементов = 29 элементов
    size_t count = 3 * DEQUE_SEGSIZE + 5;
    std::vector<int> exp;
    exp.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        fix.push_back(static_cast<int>(i));
        exp.push_back(static_cast<int>(i));
    }
    EXPECT_EQ_SCALAR("[multi_segment] size", count, fix.size());
    EXPECT_EQ_SCALAR("[multi_segment] front", 0, fix.front());
    EXPECT_EQ_SCALAR("[multi_segment] back", 28, fix.back());
    
    // Проверка корректного маппинга индексов на границы сегментов
    EXPECT_EQ_SCALAR("[multi_segment] idx_seg1_end", 7, fix[DEQUE_SEGSIZE - 1]);
    EXPECT_EQ_SCALAR("[multi_segment] idx_seg2_start", 8, fix[DEQUE_SEGSIZE]);
    EXPECT_EQ_RANGE("[multi_segment] content", exp, fix);
}

template<typename _fixture>
void push_back_after_push_front(_fixture& fix) {
    // Критический тест: проверяем работу last_elem_idx и распределения памяти,
    // когда first_elem_idx сдвинут операциями push_front.
    fix.push_front(100);
    fix.push_front(101); // Теперь deque: [101, 100], first_elem_idx != 0

    fix.push_back(42);
    fix.push_back(43);

    EXPECT_EQ_SCALAR("[after_push_front] size", 4u, fix.size());
    EXPECT_EQ_SCALAR("[after_push_front] front", 101, fix.front());
    EXPECT_EQ_SCALAR("[after_push_front] back", 43, fix.back());

    std::vector<int> exp = {101, 100, 42, 43};
    EXPECT_EQ_RANGE("[after_push_front] content", exp, fix);
}

template<typename _fixture>
void push_back_stress_test(_fixture& fix) {
    size_t count = 1000;
    for (size_t i = 0; i < count; ++i) {
        fix.push_back(static_cast<int>(i));
    }
    EXPECT_EQ_SCALAR("[stress] size", count, fix.size());
    EXPECT_EQ_SCALAR("[stress] front", 0, fix.front());
    EXPECT_EQ_SCALAR("[stress] back", 999, fix.back());
    EXPECT_EQ_SCALAR("[stress] mid", 500, fix[500]);
    EXPECT_EQ_SCALAR("[stress] last_idx", 999, fix[count - 1]);
}

// ============================================================================
// Регистрация тестов для всех доступных фикстур
// ============================================================================

// Empty
TEST_F(deque_array_seq_fixture, DqAS_push_back_empty) { push_back_empty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_push_back_empty) { push_back_empty(_fix); }
TEST_F(deque_vector_fixture, DqVec_push_back_empty) { push_back_empty(_fix); }

// Fill Segment
TEST_F(deque_array_seq_fixture, DqAS_push_back_fill_segment) { push_back_fill_segment(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_push_back_fill_segment) { push_back_fill_segment(_fix); }
TEST_F(deque_vector_fixture, DqVec_push_back_fill_segment) { push_back_fill_segment(_fix); }

// Cross Boundary
TEST_F(deque_array_seq_fixture, DqAS_push_back_cross_boundary) { push_back_cross_boundary(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_push_back_cross_boundary) { push_back_cross_boundary(_fix); }
TEST_F(deque_vector_fixture, DqVec_push_back_cross_boundary) { push_back_cross_boundary(_fix); }

// Multiple Segments
TEST_F(deque_array_seq_fixture, DqAS_push_back_multiple_segments) { push_back_multiple_segments(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_push_back_multiple_segments) { push_back_multiple_segments(_fix); }
TEST_F(deque_vector_fixture, DqVec_push_back_multiple_segments) { push_back_multiple_segments(_fix); }

// After Push Front (offset check)
TEST_F(deque_array_seq_fixture, DqAS_push_back_after_push_front) { push_back_after_push_front(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_push_back_after_push_front) { push_back_after_push_front(_fix); }
TEST_F(deque_vector_fixture, DqVec_push_back_after_push_front) { push_back_after_push_front(_fix); }

// Stress
TEST_F(deque_array_seq_fixture, DqAS_push_back_stress) { push_back_stress_test(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_push_back_stress) { push_back_stress_test(_fix); }
TEST_F(deque_vector_fixture, DqVec_push_back_stress) { push_back_stress_test(_fix); }