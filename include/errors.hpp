#pragma once

#include <exception>
#include <string>

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
    index_out_of_range(std::string msg): exception_(msg){}
};

class invalid_argument: public exception_{
public:
    invalid_argument(): exception_("invalid argument"){}
    invalid_argument(std::string msg): exception_(msg){}
};

class null_ptr: public exception_{
public:
    null_ptr(): exception_("null pointer"){}
    null_ptr(std::string msg): exception_(msg){}
};

class size_mismatch: public exception_{
public:
    size_mismatch(): exception_("size mismatch"){}
    size_mismatch(std::string msg): exception_(msg){}
};

class empty_container: public exception_{
public:
    empty_container(): exception_("container is empty"){}
    empty_container(std::string msg): exception_(msg){}
};

class not_found: public exception_{
public:
    not_found(): exception_("element not found"){}
    not_found(std::string msg): exception_(msg){}
};

class iterator_out_of_range: public exception_{
public:
    iterator_out_of_range(): exception_("iterator out of range"){}
    iterator_out_of_range(std::string msg): exception_(msg){}
};
