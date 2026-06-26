#include "assertions.hpp"
#include "tests.hpp"
#include <vector>
#include <functional>

template<typename _fixture>
void make_sorted(_fixture& fix, const std::vector<int>& data) {
    for (int v : data) fix.push_back(v);
}

template<typename _fixture>
void merge_both_empty(_fixture& fix) {
    _fixture fix2;
    fix2.set_up();
    
    auto result = fix.merge(fix2, std::less<int>());
    EXPECT_EQ_SCALAR("[both_empty] size", 0u, result->size());
    fix2.tear_down();
}

// Первый пустой, второй непустой
template<typename _fixture>
void merge_first_empty(_fixture& fix) {
    _fixture fix2;
    fix2.set_up();
    for (int i = 1; i <= 3; ++i) fix2.push_back(i);
    
    auto result = fix.merge(fix2, std::less<int>());
    std::vector<int> exp = {1,2,3};
    EXPECT_EQ_SCALAR("[first_empty] size", 3u, result->size());
    EXPECT_EQ_RANGE("[first_empty] content", exp, *result);
    fix2.tear_down();
}

// Второй пустой, первый непустой
template<typename _fixture>
void merge_second_empty(_fixture& fix) {
    for (int i = 1; i <= 3; ++i) fix.push_back(i);
    _fixture fix2;
    fix2.set_up();
    
    auto result = fix.merge(fix2, std::less<int>());
    std::vector<int> exp = {1,2,3};
    EXPECT_EQ_SCALAR("[second_empty] size", 3u, result->size());
    EXPECT_EQ_RANGE("[second_empty] content", exp, *result);
    fix2.tear_down();
}

template<typename _fixture>
void merge_basic(_fixture& fix) {
    for (int i = 1; i <= 3; ++i) fix.push_back(i);
    _fixture fix2;
    fix2.set_up();
    for (int i = 4; i <= 6; ++i) fix2.push_back(i);
    
    auto result = fix.merge(fix2, std::less<int>());
    std::vector<int> exp = {1,2,3,4,5,6};
    EXPECT_EQ_SCALAR("[basic] size", 6u, result->size());
    EXPECT_EQ_RANGE("[basic] content", exp, *result);
    fix2.tear_down();
}

template<typename _fixture>
void merge_interleaved(_fixture& fix) {
    std::vector<int> a = {1,3,5,7};
    std::vector<int> b = {2,4,6,8};
    for (int v : a) fix.push_back(v);
    _fixture fix2;
    fix2.set_up();
    for (int v : b) fix2.push_back(v);
    
    auto result = fix.merge(fix2, std::less<int>());
    std::vector<int> exp = {1,2,3,4,5,6,7,8};
    EXPECT_EQ_RANGE("[interleaved] content", exp, *result);
    fix2.tear_down();
}

template<typename _fixture>
void merge_duplicates(_fixture& fix) {
    std::vector<int> a = {1,2,2,3};
    std::vector<int> b = {2,3,4};
    for (int v : a) fix.push_back(v);
    _fixture fix2;
    fix2.set_up();
    for (int v : b) fix2.push_back(v);
    
    auto result = fix.merge(fix2, std::less<int>());
    std::vector<int> exp = {1,2,2,2,3,3,4};
    EXPECT_EQ_RANGE("[duplicates] content", exp, *result);
    fix2.tear_down();
}

template<typename _fixture>
void merge_different_sizes(_fixture& fix) {
    for (int i = 1; i <= 5; ++i) fix.push_back(i);
    _fixture fix2;
    fix2.set_up();
    for (int i = 6; i <= 8; ++i) fix2.push_back(i);
    
    auto result = fix.merge(fix2, std::less<int>());
    std::vector<int> exp = {1,2,3,4,5,6,7,8};
    EXPECT_EQ_RANGE("[diff_sizes] content", exp, *result);
    fix2.tear_down();
}

template<typename _fixture>
void merge_negative(_fixture& fix) {
    std::vector<int> a = {-5,-3,0};
    std::vector<int> b = {-4,-2,1};
    for (int v : a) fix.push_back(v);
    _fixture fix2;
    fix2.set_up();
    for (int v : b) fix2.push_back(v);
    
    auto result = fix.merge(fix2, std::less<int>());
    std::vector<int> exp = {-5,-4,-3,-2,0,1};
    EXPECT_EQ_RANGE("[negative] content", exp, *result);
    fix2.tear_down();
}

template<typename _fixture>
void merge_custom_comparator(_fixture& fix) {
    std::vector<int> a = {5,3,1};
    std::vector<int> b = {4,2,0};
    for (int v : a) fix.push_back(v);
    _fixture fix2;
    fix2.set_up();
    for (int v : b) fix2.push_back(v);
    
    auto result = fix.merge(fix2, std::greater<int>());
    std::vector<int> exp = {5,4,3,2,1,0};
    EXPECT_EQ_RANGE("[greater] content", exp, *result);
    fix2.tear_down();
}

template<typename _fixture>
void merge_multiple_segments(_fixture& fix) {
    for (size_t i = 0; i < 20; ++i) fix.push_back(static_cast<int>(i));
    _fixture fix2;
    fix2.set_up();
    for (size_t i = 20; i < 32; ++i) fix2.push_back(static_cast<int>(i));
    
    auto result = fix.merge(fix2, std::less<int>());
    std::vector<int> exp;
    for (int i = 0; i < 32; ++i) exp.push_back(i);
    EXPECT_EQ_SCALAR("[segments] size", 32u, result->size());
    EXPECT_EQ_RANGE("[segments] content", exp, *result);
    fix2.tear_down();
}

template<typename _fixture>
void merge_with_offset(_fixture& fix) {
    // Сдвигаем начало первого дека
    fix.push_front(100);
    fix.push_front(99);   // [99,100]
    for (int i = 1; i <= 3; ++i) fix.push_back(i); // [99,100,1,2,3]
    // Второй дек: [4,5,6]
    _fixture fix2;
    fix2.set_up();
    for (int i = 4; i <= 6; ++i) fix2.push_back(i);
    
    auto* result = fix.merge(&fix2, std::less<int>());
    std::vector<int> exp = {1,2,3,4,5,6,99,100};
    // Обратите внимание: merge ожидает, что оба дека уже отсортированы.
    // Но наш первый дек [99,100,1,2,3] не отсортирован. Поэтому результат будет неверным.
    // Для корректного теста нужно, чтобы оба дека были отсортированы.
    // Сделаем так: сначала отсортируем первый дек (например, вставим элементы в порядке возрастания, но со сдвигом первого сегмента).
    // Лучше: создадим дек через push_back в правильном порядке, а затем искусственно сдвинем first_elem_idx.
    // Это сложно. Вместо этого просто используем push_back для создания отсортированных деков.
    // Заменим тест: создадим два отсортированных дека, но у одного first_elem_idx != 0 за счёт добавления в начало и потом удаления? 
    // Упростим: не будем использовать offset-тест для merge, т.к. merge предполагает, что исходные деки уже отсортированы, а смещение не влияет на порядок, если элементы в порядке возрастания.
    // Например, если мы добавим элементы в порядке возрастания через push_front, они будут в убывающем порядке – не подходит.
    // Поэтому этот тест не имеет смысла. Удалим его или заменим на проверку, что merge работает с деком, имеющим пустые места в первом сегменте.
    
    // Более адекватный тест: создадим дек с помощью push_back, а затем используем push_front, но так, чтобы элементы остались отсортированными.
    // Нельзя. Пропустим этот тест. Вместо него сделаем тест с деком, у которого first_elem_idx > 0, но элементы всё равно в порядке.
    // Пример: сначала добавим несколько элементов через push_front (они станут в начале, но в обратном порядке), а потом остальные через push_back.
    // Получится неотсортированный дек. Для merge это недопустимо.
    // Значит, тест на offset для merge не нужен.
    delete result;
    fix2.tear_down();
}

// Стресс-тест: большие деки
template<typename _fixture>
void merge_stress(_fixture& fix) {
    const size_t N = 1000;
    for (size_t i = 0; i < N; ++i) fix.push_back(static_cast<int>(i));
    _fixture fix2;
    fix2.set_up();
    for (size_t i = N; i < 2*N; ++i) fix2.push_back(static_cast<int>(i));
    
    auto result = fix.merge(fix2, std::less<int>());
    std::vector<int> exp;
    exp.reserve(2*N);
    for (size_t i = 0; i < 2*N; ++i) exp.push_back(static_cast<int>(i));
    EXPECT_EQ_SCALAR("[stress] size", 2*N, result->size());
    EXPECT_EQ_RANGE("[stress] content", exp, *result);
    fix2.tear_down();
}

// Проверка, что исходные деки не изменяются (merge создаёт новый)
template<typename _fixture>
void merge_original_unchanged(_fixture& fix) {
    std::vector<int> a = {1,3,5};
    std::vector<int> b = {2,4,6};
    for (int v : a) fix.push_back(v);
    _fixture fix2;
    fix2.set_up();
    for (int v : b) fix2.push_back(v);
    
    auto result = fix.merge(fix2, std::less<int>());
    std::vector<int> exp = {1,2,3,4,5,6};
    EXPECT_EQ_RANGE("[unchanged] result", exp, *result);
    // Проверяем, что исходные деки остались неизменными
    EXPECT_EQ_RANGE("[unchanged] original a", a, fix);
    EXPECT_EQ_RANGE("[unchanged] original b", b, fix2);
    fix2.tear_down();
}

TEST_F(deque_array_seq_fixture, DqAS_merge_both_empty) { merge_both_empty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_merge_both_empty) { merge_both_empty(_fix); }
TEST_F(deque_vector_fixture, DqVec_merge_both_empty) { merge_both_empty(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_merge_first_empty) { merge_first_empty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_merge_first_empty) { merge_first_empty(_fix); }
TEST_F(deque_vector_fixture, DqVec_merge_first_empty) { merge_first_empty(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_merge_second_empty) { merge_second_empty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_merge_second_empty) { merge_second_empty(_fix); }
TEST_F(deque_vector_fixture, DqVec_merge_second_empty) { merge_second_empty(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_merge_basic) { merge_basic(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_merge_basic) { merge_basic(_fix); }
TEST_F(deque_vector_fixture, DqVec_merge_basic) { merge_basic(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_merge_interleaved) { merge_interleaved(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_merge_interleaved) { merge_interleaved(_fix); }
TEST_F(deque_vector_fixture, DqVec_merge_interleaved) { merge_interleaved(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_merge_duplicates) { merge_duplicates(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_merge_duplicates) { merge_duplicates(_fix); }
TEST_F(deque_vector_fixture, DqVec_merge_duplicates) { merge_duplicates(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_merge_different_sizes) { merge_different_sizes(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_merge_different_sizes) { merge_different_sizes(_fix); }
TEST_F(deque_vector_fixture, DqVec_merge_different_sizes) { merge_different_sizes(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_merge_negative) { merge_negative(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_merge_negative) { merge_negative(_fix); }
TEST_F(deque_vector_fixture, DqVec_merge_negative) { merge_negative(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_merge_greater) { merge_custom_comparator(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_merge_greater) { merge_custom_comparator(_fix); }
TEST_F(deque_vector_fixture, DqVec_merge_greater) { merge_custom_comparator(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_merge_segments) { merge_multiple_segments(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_merge_segments) { merge_multiple_segments(_fix); }
TEST_F(deque_vector_fixture, DqVec_merge_segments) { merge_multiple_segments(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_merge_stress) { merge_stress(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_merge_stress) { merge_stress(_fix); }
TEST_F(deque_vector_fixture, DqVec_merge_stress) { merge_stress(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_merge_unchanged) { merge_original_unchanged(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_merge_unchanged) { merge_original_unchanged(_fix); }
TEST_F(deque_vector_fixture, DqVec_merge_unchanged) { merge_original_unchanged(_fix); }