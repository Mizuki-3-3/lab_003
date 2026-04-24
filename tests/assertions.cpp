#include "assertions.hpp"
#include "tests.hpp"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int success_count = 0;
static int fail_count = 0;
static int success_count_local = 0;
static int fail_count_local = 0;

static _test* head = NULL;
static _test* tail = NULL;

void register_test(const char* name, test_func test) {
    _test* tmp = new _test;
    if (!tmp) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    tmp->name = name;
    tmp->test_func = test;
    tmp->next = NULL;
    
    if (head == NULL && tail == NULL) {
        head = tmp;
        tail = tmp;
        return;
    }
    tail->next = tmp;
    tail = tmp;
}

void int_success(){
    success_count++;success_count_local++;
}

void int_fail(){
    fail_count++;fail_count_local++;
}

int get_local_fail(void) {
    return fail_count_local;
}

int get_local_success(void) {
    return success_count_local;
}

void reset_local_counts() {
    success_count_local = 0;
    fail_count_local = 0;
}

int assert_fail(const char* expr, const char* file, unsigned int lineno) {
    fprintf(stderr, "\033[0;31m%s:%u:\t %s FAILED \033[0m\n", file, lineno, expr);
    int_fail();
    return 0;
}

int assert_success(const char* expr, const char* file, unsigned int lineno) {
    fprintf(stderr, "\033[0;32m%s:%u:\t %s PASSED \033[0m\n", file, lineno, expr);
    int_success();
    return 0;
}


int print_stats() {
    int total = fail_count + success_count;
    if (total == 0) {
        fprintf(stderr, "No tests run\n");
        return 0;
    }
    float percentage = (float)success_count / (float)total * 100.0f;
    fprintf(stderr, "\n%d of %d tests passed. %.2f%% SUCCEEDED\n",
            success_count,
            total,
            percentage);
    return success_count == 0 ? 1 : 0;
}

void run_test() {
    _test* current = head;
    while (current != NULL) {
        fprintf(stderr, "\nRunning test: %s\n", current->name);
        success_count_local = 0;
        fail_count_local = 0;
        
        if (current->test_func != NULL) {
            current->test_func();
        }
        current = current->next;
    }
}

void cleanup_tests() {
    _test* current = head;
    while (current != NULL) {
        _test* next = current->next;
        delete current;
        current = next;
    }
    head = NULL;
    tail = NULL;
}