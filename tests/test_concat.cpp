#include "assertions.hpp"
#include "errors.hpp"
#include "tests.hpp"
#include <vector>

constexpr size_t DEQUE_SEGSIZE = 8;

//concat
template<typename _fixture>
void concat_both_empty(_fixture& fix) {
    _fixture fix2;
    fix2.set_up();
    
    auto result = fix.concat(fix2);
    EXPECT_EQ_SCALAR("both_empty size", 0u, result->size());
    fix2.tear_down();
}

template<typename _fixture>
void concat_first_empty_second_nonempty(_fixture& fix) {
    _fixture fix2;
    fix2.set_up();
    for (int i = 1; i <= 3; ++i) fix2.push_back(i);
    
    auto result =  fix.concat(fix2);
    std::vector<int> exp = {1,2,3};
    EXPECT_EQ_SCALAR("{} concat {1,2,3} size", 3u, result->size());
    EXPECT_EQ_RANGE("{} concat {1,2,3} content", exp, *result);
    fix2.tear_down();
}

template<typename _fixture>
void concat_second_empty(_fixture& fix) {
    _fixture fix2;
    fix2.set_up();
    for (int i = 1; i <= 3; ++i) fix.push_back(i);
    
    auto result =  fix.concat(fix2);
    std::vector<int> exp = {1,2,3};
    EXPECT_EQ_SCALAR("{1,2,3} concat {} size", 3u, result->size());
    EXPECT_EQ_RANGE("{1,2,3} concat {} content", exp, *result);
    fix2.tear_down();
}

template<typename _fixture>
void concat_nonempty(_fixture& fix) {
    for (int i = 1; i <= 3; ++i) fix.push_back(i);
    _fixture fix2;
    fix2.set_up();
    for (int i = 4; i <= 6; ++i) fix2.push_back(i);
    
    auto result =  fix.concat(fix2);
    std::vector<int> exp = {1,2,3,4,5,6};
    EXPECT_EQ_SCALAR("{1,2,3} concat {4,5,6} size", 6u, result->size());
    EXPECT_EQ_RANGE("{1,2,3} concat {4,5,6} content", exp, *result);
    fix2.tear_down();
}

template<typename _fixture>
void concat_segment_cross(_fixture& fix) {
    for (size_t i = 0; i < DEQUE_SEGSIZE - 2; ++i) fix.push_back(static_cast<int>(i));
    _fixture fix2;
    fix2.set_up();
    for (size_t i = 0; i < DEQUE_SEGSIZE + 3; ++i) fix2.push_back(static_cast<int>(i + 100));
    
    auto result =  fix.concat(fix2);
    std::vector<int> exp;
    for (size_t i = 0; i < DEQUE_SEGSIZE - 2; ++i) exp.push_back(static_cast<int>(i));
    for (size_t i = 0; i < DEQUE_SEGSIZE + 3; ++i) exp.push_back(static_cast<int>(i + 100));
    EXPECT_EQ_SCALAR("[segment_cross] size", exp.size(), result->size());
    EXPECT_EQ_RANGE("[segment_cross] content", exp, *result);
    fix2.tear_down();
}

//find
// template<typename _fixture>
// void find_existing(_fixture& fix) {
//     for (int i = 10; i <= 20; ++i) fix.push_back(i);
//     auto it = fix.find(15);
//     EXPECT_EQ_SCALAR("[existing] value", 15, *it);
//     // Проверим, что итератор указывает на правильную позицию
//     size_t pos = 0;
//     for (auto cur = fix.begin(); cur != it; ++cur) ++pos;
//     EXPECT_EQ_SCALAR("[existing] position", 5u, pos);
// }

// template<typename _fixture>
// void find_first_element(_fixture& fix) {
//     for (int i = 10; i <= 20; ++i) fix.push_back(i);
//     auto it = fix.find(10);
//     EXPECT_EQ_SCALAR("[first] value", 10, *it);
//     EXPECT_TRUE("[first] equals begin", it == fix.begin());
// }

// template<typename _fixture>
// void find_last_element(_fixture& fix) {
//     for (int i = 10; i <= 20; ++i) fix.push_back(i);
//     auto it = fix.find(20);
//     EXPECT_EQ_SCALAR("[last] value", 20, *it);
//     // Проверим, что это последний элемент (не end)
//     auto next = it; ++next;
//     EXPECT_TRUE("[last] next is end", next == fix.end());
// }

// template<typename _fixture>
// void find_not_found(_fixture& fix) {
//     for (int i = 10; i <= 20; ++i) fix.push_back(i);
//     bool thrown = false;
//     try {
//         fix.find(99);
//     } catch (const not_found&) {
//         thrown = true;
//     }
//     EXPECT_TRUE("[not_found] throws not_found", thrown);
// }

// template<typename _fixture>
// void find_in_empty(_fixture& fix) {
//     // fix пуст
//     bool thrown = false;
//     try {
//         fix.find(42);
//     } catch (const not_found&) {
//         thrown = true;
//     }
//     EXPECT_TRUE("[empty] throws not_found", thrown);
// }

// ============================================================================
// find_subsequence
// ============================================================================

template<typename _fixture>
void find_subseq_existing_begin(_fixture& fix) {
    fix.make_nonempty_1seg();
    // Образец: [1,2,3]
    _fixture pattern;
    pattern.set_up();
    for (int i = 1; i <= 3; ++i) pattern.push_back(i);
    
    auto it = fix.find_subsequence(pattern);
    EXPECT_EQ_SCALAR("[begin] value at found pos", 1, *it);
    EXPECT_TRUE("[begin] iterator equals begin", it == fix.begin());
    pattern.tear_down();
}

template<typename _fixture>
void find_subseq_existing_middle(_fixture& fix) {
    fix.make_nonempty_1seg();
    // Образец: [3,4,5]
    _fixture pattern;
    pattern.set_up();
    for (int i = 3; i <= 5; ++i) pattern.push_back(i);
    
    auto it = fix.find_subsequence(pattern);
    EXPECT_EQ_SCALAR("[middle] value at found pos", 3, *it);
    // Проверим, что следующие элементы совпадают
    auto tmp = it;
    for (int i = 3; i <= 5; ++i, ++tmp) EXPECT_EQ_SCALAR("[middle] element", i, *tmp);
    pattern.tear_down();
}

template<typename _fixture>
void find_subseq_existing_end(_fixture& fix) {
    fix.make_nonempty_1seg();
    // Образец: [6,7,8]
    _fixture pattern;
    pattern.set_up();
    for (int i = 6; i <= 8; ++i) pattern.push_back(i);
    
    auto it = fix.find_subsequence(pattern);
    EXPECT_EQ_SCALAR("[end] value at found pos", 6, *it);
    pattern.tear_down();
}

template<typename _fixture>
void find_subseq_overlap(_fixture& fix) {
    // Дек: [1,2,1,2,1]
    fix.push_back(1); fix.push_back(2); fix.push_back(1); fix.push_back(2); fix.push_back(1);
    // Образец: [1,2,1] – должно найти первое вхождение на индексе 0
    _fixture pattern;
    pattern.set_up();
    pattern.push_back(1); pattern.push_back(2); pattern.push_back(1);
    
    auto it = fix.find_subsequence(pattern);
    EXPECT_EQ_SCALAR("[overlap] value at found pos", 1, *it);
    size_t pos = 0;
    for (auto cur = fix.begin(); cur != it; ++cur) ++pos;
    EXPECT_EQ_SCALAR("[overlap] position", 0u, pos);
    pattern.tear_down();
}

template<typename _fixture>
void find_subseq_not_found(_fixture& fix) {
    for (int i = 1; i <= 5; ++i) fix.push_back(i);
    // Образец: [5,6]
    _fixture pattern;
    pattern.set_up();
    pattern.push_back(5); pattern.push_back(6);
    
    bool thrown = false;
    try {
        fix.find_subsequence(pattern);
    } catch (const not_found&) {
        thrown = true;
    }
    EXPECT_TRUE("[not_found] throws not_found", thrown);
    pattern.tear_down();
}

template<typename _fixture>
void find_subseq_pattern_larger_than_deque(_fixture& fix) {
    for (int i = 1; i <= 3; ++i) fix.push_back(i);
    // Образец: [1,2,3,4]
    _fixture pattern;
    pattern.set_up();
    for (int i = 1; i <= 4; ++i) pattern.push_back(i);
    
    bool thrown = false;
    try {
        fix.find_subsequence(pattern);
    } catch (const size_mismatch&) {
        thrown = true;
    }
    EXPECT_TRUE("pattern larger, func throws size mismatch", thrown);
    pattern.tear_down();
}

template<typename _fixture>
void find_subseq_empty_pattern(_fixture& fix) {
    fix.push_back(42);
    _fixture pattern;
    pattern.set_up(); // пустой образец
    
    // По логике find_subsequence (в коде) при m==0 возвращает 0 (итератор на начало)
    // Мы ожидаем, что вернётся begin()
    auto it = fix.find_subsequence(pattern);
    EXPECT_TRUE("[empty_pattern] returns begin", it == fix.begin());
    pattern.tear_down();
}

// concat
TEST_F(deque_array_seq_fixture, DqAS_concat_both_empty) { concat_both_empty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_concat_both_empty) { concat_both_empty(_fix); }
TEST_F(deque_vector_fixture, DqVec_concat_both_empty) { concat_both_empty(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_concat_first_empty_second_nonempty) { concat_first_empty_second_nonempty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_concat_first_empty_second_nonempty) { concat_first_empty_second_nonempty(_fix); }
TEST_F(deque_vector_fixture, DqVec_concat_first_empty_second_nonempty) { concat_first_empty_second_nonempty(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_concat_second_empty) { concat_second_empty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_concat_second_empty) { concat_second_empty(_fix); }
TEST_F(deque_vector_fixture, DqVec_concat_second_empty) { concat_second_empty(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_concat_nonempty) { concat_nonempty(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_concat_nonempty) { concat_nonempty(_fix); }
TEST_F(deque_vector_fixture, DqVec_concat_nonempty) { concat_nonempty(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_concat_segment_cross) { concat_segment_cross(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_concat_segment_cross) { concat_segment_cross(_fix); }
TEST_F(deque_vector_fixture, DqVec_concat_segment_cross) { concat_segment_cross(_fix); }

// // find
// TEST_F(deque_array_seq_fixture, DqAS_find_existing) { find_existing(_fix); }
// TEST_F(deque_list_seq_fixture, DqLS_find_existing) { find_existing(_fix); }
// TEST_F(deque_vector_fixture, DqVec_find_existing) { find_existing(_fix); }

// TEST_F(deque_array_seq_fixture, DqAS_find_first) { find_first_element(_fix); }
// TEST_F(deque_list_seq_fixture, DqLS_find_first) { find_first_element(_fix); }
// TEST_F(deque_vector_fixture, DqVec_find_first) { find_first_element(_fix); }

// TEST_F(deque_array_seq_fixture, DqAS_find_last) { find_last_element(_fix); }
// TEST_F(deque_list_seq_fixture, DqLS_find_last) { find_last_element(_fix); }
// TEST_F(deque_vector_fixture, DqVec_find_last) { find_last_element(_fix); }

// TEST_F(deque_array_seq_fixture, DqAS_find_not_found) { find_not_found(_fix); }
// TEST_F(deque_list_seq_fixture, DqLS_find_not_found) { find_not_found(_fix); }
// TEST_F(deque_vector_fixture, DqVec_find_not_found) { find_not_found(_fix); }

// TEST_F(deque_array_seq_fixture, DqAS_find_empty) { find_in_empty(_fix); }
// TEST_F(deque_list_seq_fixture, DqLS_find_empty) { find_in_empty(_fix); }
// TEST_F(deque_vector_fixture, DqVec_find_empty) { find_in_empty(_fix); }

// find_subsequence
TEST_F(deque_array_seq_fixture, DqAS_find_subseq_begin) { find_subseq_existing_begin(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_find_subseq_begin) { find_subseq_existing_begin(_fix); }
TEST_F(deque_vector_fixture, DqVec_find_subseq_begin) { find_subseq_existing_begin(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_find_subseq_middle) { find_subseq_existing_middle(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_find_subseq_middle) { find_subseq_existing_middle(_fix); }
TEST_F(deque_vector_fixture, DqVec_find_subseq_middle) { find_subseq_existing_middle(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_find_subseq_end) { find_subseq_existing_end(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_find_subseq_end) { find_subseq_existing_end(_fix); }
TEST_F(deque_vector_fixture, DqVec_find_subseq_end) { find_subseq_existing_end(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_find_subseq_overlap) { find_subseq_overlap(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_find_subseq_overlap) { find_subseq_overlap(_fix); }
TEST_F(deque_vector_fixture, DqVec_find_subseq_overlap) { find_subseq_overlap(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_find_subseq_not_found) { find_subseq_not_found(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_find_subseq_not_found) { find_subseq_not_found(_fix); }
TEST_F(deque_vector_fixture, DqVec_find_subseq_not_found) { find_subseq_not_found(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_find_subseq_larger) { find_subseq_pattern_larger_than_deque(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_find_subseq_larger) { find_subseq_pattern_larger_than_deque(_fix); }
TEST_F(deque_vector_fixture, DqVec_find_subseq_larger) { find_subseq_pattern_larger_than_deque(_fix); }

TEST_F(deque_array_seq_fixture, DqAS_find_subseq_empty) { find_subseq_empty_pattern(_fix); }
TEST_F(deque_list_seq_fixture, DqLS_find_subseq_empty) { find_subseq_empty_pattern(_fix); }
TEST_F(deque_vector_fixture, DqVec_find_subseq_empty) { find_subseq_empty_pattern(_fix); }