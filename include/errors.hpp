#pragma once

#include <exception>
#include <cstdio>

enum errors {
    NO_ERR = 0,
    ERR_NULL,
    ERR_MEMORY,
    ERR_INCORRECT_INDEX,
    ERR_TYPE_MISMATCH,
};

class exception_ : public std::exception {
private:
    errors err_code;
    const char* stack_files[10];
    int stack_lines[10];
    int stack_depth;
    mutable char what_buf[256];

public:
    exception_(errors code, const char* file, int line) noexcept;
    void add_context(const char* file, int line) noexcept;
    errors get_code() const noexcept;
    const char* what() const noexcept override;
    void print_stack(FILE* out = stderr) const;
};

const char* err_get_message(errors err);

#define TRY try {
#define CATCH(err_var) } catch (const exception_& e) { err_var = e.get_code();
#define CATCH_TESTS } catch (const exception_&) {
#define ETRY }
#define THROW(err_code) do { throw exception_(err_code, __FILE__, __LINE__); } while(0)
#define RETURN_ON_ERROR(cond, err_code) do { if (cond) { THROW(err_code); return; } } while(0)
#define RETURN_VAL_ON_ERROR(cond, err_code, ret) do { if (cond) { THROW(err_code); return ret; } } while(0)

#include "errors.tpp" 