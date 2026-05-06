#include "data_type/Complex.hpp"

template <typename T>
complex<T>::complex(T newRe, T newIm): Re(newRe), Im(newIm){}

template <typename T>
complex<T>::complex(const complex& other): Re(other.Re), Im(other.Im){}

template <typename T>
complex<T>& complex<T>::operator=(const complex& copy){
    std::swap(Re, other.Re);
    std::swap(Im, other.Im);
}

template <typename T>
complex<T>::~complex(){
    delete Im;
    delete Re;
}

template <typename T>
T complex<T>::get_Re(){
    return this->Re;
}

template <typename T>
T complex<T>::get_Im(){
    return this->Im;
}
