#include "data_type/Person.hpp"

#include <cstring>
#include <cstdio>

person::person(){
    id = new char[1];
    id[0] = '\0';
    first_name = new char[1];
    first_name[0] = '\0';
    middle_name = new char[1];
    middle_name[0] = '\0';
    last_name = new char[1];
    last_name[0] = '\0';
    role = role::student;
}

person::~person(){
    delete[] id;
    delete[] first_name;
    delete[] middle_name;
    delete[] last_name;
}

person::person(const person& other){
    if (other.id) {
        id = new char[std::strlen(other.id) + 1];
        std::strcpy(id, other.id);
    } else {
        id = new char[1];
        id[0] = '\0';
    }

    if (other.first_name) {
        first_name = new char[std::strlen(other.first_name) + 1];
        std::strcpy(first_name, other.first_name);
    } else {
        first_name = new char[1];
        first_name[0] = '\0';
    }

    if (other.middle_name) {
        middle_name = new char[std::strlen(other.middle_name) + 1];
        std::strcpy(middle_name, other.middle_name);
    } else {
        middle_name = new char[1];
        middle_name[0] = '\0';
    }

    if (other.last_name) {
        last_name = new char[std::strlen(other.last_name) + 1];
        std::strcpy(last_name, other.last_name);
    } else {
        last_name = new char[1];
        last_name[0] = '\0';
    }
}

person::person(const char* id, const char* first, const char* middle, const char* last){
    if (id) {
        this->id = new char[std::strlen(id) + 1];
        std::strcpy(this->id, id);
    } else {
        this->id = new char[1];
        this->id[0] = '\0';
    }
    if (first) {
        first_name = new char[std::strlen(first) + 1];
        std::strcpy(first_name, first);
    } else {
        first_name = new char[1];
        first_name[0] = '\0';
    }
    if (middle) {
        middle_name = new char[std::strlen(middle) + 1];
        std::strcpy(middle_name, middle);
    } else {
        middle_name = new char[1];
        middle_name[0] = '\0';
    }
    if (last) {
        last_name = new char[std::strlen(last) + 1];
        std::strcpy(last_name, last);
    } else {
        last_name = new char[1];
        last_name[0] = '\0';
    }
}

person& person::operator=(const person& copy){
    if (this != &copy) {
        delete[] id;
        delete[] first_name;
        delete[] middle_name;
        delete[] last_name;
        if (copy.id) {
            id = new char[std::strlen(copy.id) + 1];
            std::strcpy(id, copy.id);
        } else {
            id = new char[1];
            id[0] = '\0';
        }
        if (copy.first_name) {
            first_name = new char[std::strlen(copy.first_name) + 1];
            std::strcpy(first_name, copy.first_name);
        } else {
            first_name = new char[1];
            first_name[0] = '\0';
        }
        if (copy.middle_name) {
            middle_name = new char[std::strlen(copy.middle_name) + 1];
            std::strcpy(middle_name, copy.middle_name);
        } else {
            middle_name = new char[1];
            middle_name[0] = '\0';
        }
        if (copy.last_name) {
            last_name = new char[std::strlen(copy.last_name) + 1];
            std::strcpy(last_name, copy.last_name);
        } else {
            last_name = new char[1];
            last_name[0] = '\0';
        }
    }
    return *this;
}

person_ID person::get_ID(){
    return id;
}

char* person::get_first_name(){
    return first_name;
}

char* person::get_middle_name(){
    return middle_name;
}

char* person::get_last_name(){
    return last_name;
}