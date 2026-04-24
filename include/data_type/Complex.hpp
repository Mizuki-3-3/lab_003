#pragma once

template<typename T>
class complex{
private:
T Im;
T Re;
public:
T get_Re();
T get_Im();
complex() = default;
~complex();
complex(T Re, T Im);
complex(const complex& other);
complex& operator=(const complex& copy);
};

#include "data_type/Complex.tpp"