#include "assertions.hpp"
#include "tests.hpp"
#include <vector>

constexpr size_t DEQUE_SEGSIZE = 8;

template<typename _fixture>
void front_back_single(_fixture& fix) {
    fix.push_back(42);
    EXPECT_EQ_SCALAR("[single] front", 42, fix.front());
    EXPECT_EQ_SCALAR("[single] back", 42, fix.back());
    EXPECT_EQ_SCALAR("[single] front == back", fix.front(), fix.back());
}

template<typename _fixture>
void front_back_after_push_backs(_fixture& fix) {
    fix.push_back(10); fix.push_back(20); fix.push_back(30);
    EXPECT_EQ_SCALAR("[push_backs] front", 10, fix.front());
    EXPECT_EQ_SCALAR("[push_backs] back", 30, fix.back());

    EXPECT_EQ_SCALAR("[push_backs] front == [0]", fix.front(), fix[0]);//проверка согласованности с operator[]
    EXPECT_EQ_SCALAR("[push_backs] back == [size-1]", fix.back(), fix[2]);
}

template<typename _fixture>
void front_back_after_push_fronts(_fixture& fix) {
    fix.push_front(30); fix.push_front(20); fix.push_front(10);
    EXPECT_EQ_SCALAR("[push_fronts] front", 10, fix.front());
    EXPECT_EQ_SCALAR("[push_fronts] back", 30, fix.back());
    EXPECT_EQ_SCALAR("[push_fronts] front == [0]", fix.front(), fix[0]);
    EXPECT_EQ_SCALAR("[push_fronts] back == [size-1]", fix.back(), fix[2]);
}

template<typename _fixture>
void front_back_mixed_ops(_fixture& fix) {
    // Последовательность: push_back(1), push_back(2), push_front(3), push_front(4)
    // Итог: [4, 3, 1, 2]
    fix.push_back(1); fix.push_back(2);
    fix.push_front(3); fix.push_front(4);
    
    EXPECT_EQ_SCALAR("[mixed] front", 4, fix.front());
    EXPECT_EQ_SCALAR("[mixed] back", 2, fix.back());
    EXPECT_EQ_SCALAR("[mixed] front == [0]", fix.front(), fix[0]);
    EXPECT_EQ_SCALAR("[mixed] back == [size-1]", fix.back(), fix[3]);
}

template<typename _fixture>
void front_back_segment_boundary(_fixture& fix) {
    fix.make_nonempty_1seg();
    EXPECT_EQ_SCALAR("[full_seg] front", 1, fix.front());
    EXPECT_EQ_SCALAR("[full_seg] back", static_cast<int>(DEQUE_SEGSIZE), fix.back());

    // 2. Добавляем элемент -> триггерит аллокацию нового сегмента и сдвиг last_elem_idx
    fix.push_back(99);
    EXPECT_EQ_SCALAR("[cross_boundary] front", 1, fix.front());
    EXPECT_EQ_SCALAR("[cross_boundary] back", 99, fix.back());
    EXPECT_EQ_SCALAR("[cross_boundary] front consistency", fix.front(), fix[0]);
    EXPECT_EQ_SCALAR("[cross_boundary] back consistency", fix.back(), fix[DEQUE_SEGSIZE]);

    // 3. Добавляем в начало -> проверяем работу с first_elem_idx на стыке сегментов
    fix.push_front(-1);
    EXPECT_EQ_SCALAR("[both_sides] front", -1, fix.front());
    EXPECT_EQ_SCALAR("[both_sides] back", 99, fix.back());
    EXPECT_EQ_SCALAR("[both_sides] front consistency", fix.front(), fix[0]);
    EXPECT_EQ_SCALAR("[both_sides] back consistency", fix.back(), fix[fix.size() - 1]);
}

template<typename _fixture>
void front_back_stress_alternating(_fixture& fix) {
    std::vector<int> exp;
    // 1000 операций: чередование push_front и push_back
    for (int i = 0; i < 1000; ++i) {
        if (i % 2 == 0) {
            fix.push_front(i);
            exp.insert(exp.begin(), i);
        } else {
            fix.push_back(i);
            exp.push_back(i);
        }
    }
    EXPECT_EQ_SCALAR("[stress] size", exp.size(), fix.size());
    EXPECT_EQ_SCALAR("[stress] front", exp.front(), fix.front());
    EXPECT_EQ_SCALAR("[stress] back", exp.back(), fix.back());
    // Кросс-проверка: front/back должны совпадать с доступом по индексу
    EXPECT_EQ_SCALAR("[stress] front == [0]", exp.front(), fix[0]);
    EXPECT_EQ_SCALAR("[stress] back == [last]", exp.back(), fix[fix.size() - 1]);
}


// Single element
TEST_F(deque_array_seq_fixture, DqAS_front_back_single) { front_back_single(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_front_back_single) { front_back_single(_fix); }
TEST_F(deque_vector_fixture, DqVec_front_back_single) { front_back_single(_fix); }

// Push Backs
TEST_F(deque_array_seq_fixture, DqAS_front_back_after_push_backs) { front_back_after_push_backs(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_front_back_after_push_backs) { front_back_after_push_backs(_fix); }
TEST_F(deque_vector_fixture, DqVec_front_back_after_push_backs) { front_back_after_push_backs(_fix); }

// Push Fronts
TEST_F(deque_array_seq_fixture, DqAS_front_back_after_push_fronts) { front_back_after_push_fronts(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_front_back_after_push_fronts) { front_back_after_push_fronts(_fix); }
TEST_F(deque_vector_fixture, DqVec_front_back_after_push_fronts) { front_back_after_push_fronts(_fix); }

// Mixed Operations
TEST_F(deque_array_seq_fixture, DqAS_front_back_mixed) { front_back_mixed_ops(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_front_back_mixed) { front_back_mixed_ops(_fix); }
TEST_F(deque_vector_fixture, DqVec_front_back_mixed) { front_back_mixed_ops(_fix); }

// Segment Boundary (Critical for Segmented Deque)
TEST_F(deque_array_seq_fixture, DqAS_front_back_boundary) { front_back_segment_boundary(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_front_back_boundary) { front_back_segment_boundary(_fix); }
TEST_F(deque_vector_fixture, DqVec_front_back_boundary) { front_back_segment_boundary(_fix); }

// Stress Alternating
TEST_F(deque_array_seq_fixture, DqAS_front_back_stress) { front_back_stress_alternating(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_front_back_stress) { front_back_stress_alternating(_fix); }
TEST_F(deque_vector_fixture, DqVec_front_back_stress) { front_back_stress_alternating(_fix); }