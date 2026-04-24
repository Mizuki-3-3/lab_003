#pragma once

#define assert(expr) \
    ((!!(expr)) \
    ? assert_success (#expr, __FILE__, __LINE__) \
    : assert_fail (#expr, __FILE__, __LINE__))

void int_success(void);
void int_fail(void);

int assert_success(const char* expr, const char* file, unsigned int line);
int assert_fail(const char* expr, const char* file, unsigned int line);

template<typename Seq>
int check_seq(Seq* seq, const int* expected, unsigned len) {
    if (seq->size() != len) return false;
    for (unsigned i = 0; i < len; ++i) {
        if ((*seq)[i] != expected[i]) return false;
    }
    return true;
}
