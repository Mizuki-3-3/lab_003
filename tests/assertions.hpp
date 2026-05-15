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

#define EXPECT_EQ(input, expected, actual) \
    do { \
        auto _inp = (input); \
        auto _exp = (expected); \
        auto _act = (actual); \
        std::cerr << __FILE__ << ":" << __LINE__ << std::endl<< " Input: " << input << std::endl; \
        if (_exp == _act) { \
            std::cerr << COLOR_GREEN \
                       << " Expected: " << _exp << ", Actual: " << _act \
                      << " [PASS]" << COLOR_RESET << std::endl; \
            int_success(); \
        } else { \
            std::cerr << COLOR_RED << \
                      << " Expected: " << _exp << ", Actual: " << _act \
                      << " [FAIL]" << COLOR_RESET << std::endl; \
            int_fail(); \
        } \
    } while(0)

#define EXPECT_TRUE(con) \
    do { \
        if (cond) { \
            std::cout << COLOR_GREEN << __FILE__ << ":" << __LINE__ << " " \
                       << " [PASS]" << COLOR_RESET << std::endl; \
            int_success(); \
        } else { \
            std::cout << COLOR_RED << __FILE__ << ":" << __LINE__ << " " \
                       << " [FAIL]" << COLOR_RESET << std::endl; \
            int_fail(); \
        } \
    } while(0)

