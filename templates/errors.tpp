#pragma once

#include "errors.hpp"
#include <cstring>
#include <cstdio>

inline exception_::exception_(errors code, const char* file, int line) noexcept: err_code(code), stack_depth(0) {
    add_context(file, line);
}

inline void exception_::add_context(const char* file, int line) noexcept {
    if (stack_depth < 10) {
        stack_files[stack_depth] = file;
        stack_lines[stack_depth] = line;
        ++stack_depth;
    }
}

inline errors exception_::get_code() const noexcept {
    return err_code;
}

inline const char* exception_::what() const noexcept {
    const char* msg = err_get_message(err_code);
    if (stack_depth == 0) {
        std::snprintf(what_buf, sizeof(what_buf), "[%d] %s", err_code, msg);
    } else {
        std::snprintf(what_buf, sizeof(what_buf), "[%d] %s\nStack:\n", err_code, msg);
        char* pos = what_buf + strlen(what_buf);
        for (int i = 0; i < stack_depth && pos < what_buf + sizeof(what_buf); ++i) {
            pos += std::snprintf(pos, sizeof(what_buf) - (pos - what_buf),
                                 "  [%d] %s:%d\n", i, stack_files[i], stack_lines[i]);
        }
    }
    return what_buf;
}

inline void exception_::print_stack(FILE* out) const {
    std::fprintf(out, "Исключение: %s", what());
    std::fflush(out);
}

inline const char* err_get_message(errors err) {
    switch (err) {
        case ERR_NULL:           return "Нулевой указатель\n";
        case ERR_MEMORY:         return "Не удалось выделить память\n";
        case ERR_INCORRECT_INDEX:return "Неверный индекс\n";
        case ERR_TYPE_MISMATCH:  return "Несовместимые типы\n";
        default:                 return "Неизвестная ошибка\n";
    }
}