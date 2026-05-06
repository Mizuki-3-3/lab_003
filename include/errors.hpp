#pragma once

#include <exception>
#include <cstdio>
#include "string"

class exception_ : public std::exception {
private:
    std::string message;
public:
    exception_(const std::string msg): message(msg){}
    const char* what() const noexcept override{return message.c_str();}
};

class index_out_of_range: public exception_{
public:
    index_out_of_range(): exception_("index out of range"){}
};

class invalid_argument: public exception_{
public:
    invalid_argument(): exception_("invalid argument"){}
};

class null_ptr: public exception_{
public:
    null_ptr(): exception_("null pointer"){}
};

class size_mismatch: public exception_{
public:
    size_mismatch(): exception_("size mismatch"){}
};

class empty_container: public exception_{
public:
    empty_container(): exception_("container is empty"){}
};