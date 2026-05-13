#pragma once

#include <iostream>

#define assert(expr) \
    ((!!(expr)) \
    ? assert_success (#expr, __FILE__, __LINE__) \
    : assert_fail (#expr, __FILE__, __LINE__))

void int_success(void);
void int_fail(void);

int assert_success(const char* expr, const char* file, unsigned int line);
int assert_fail(const char* expr, const char* file, unsigned int line);

#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_RESET   "\033[0m"

#define EXPECT_EQ(expected, actual, msg) \
    do { \
        auto _exp = (expected); \
        auto _act = (actual); \
        if (_exp == _act) { \
            std::cout << COLOR_GREEN << __FILE__ << ":" << __LINE__ << " " \
                      << msg << " Expected: " << _exp << ", Actual: " << _act \
                      << " [PASS]" << COLOR_RESET << std::endl; \
            int_success(); \
        } else { \
            std::cout << COLOR_RED << __FILE__ << ":" << __LINE__ << " " \
                      << msg << " Expected: " << _exp << ", Actual: " << _act \
                      << " [FAIL]" << COLOR_RESET << std::endl; \
            int_fail(); \
        } \
    } while(0)

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (cond) { \
            std::cout << COLOR_GREEN << __FILE__ << ":" << __LINE__ << " " \
                      << msg << " [PASS]" << COLOR_RESET << std::endl; \
            int_success(); \
        } else { \
            std::cout << COLOR_RED << __FILE__ << ":" << __LINE__ << " " \
                      << msg << " [FAIL]" << COLOR_RESET << std::endl; \
            int_fail(); \
        } \
    } while(0)

