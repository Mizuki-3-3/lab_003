#include "linked_list.hpp"
#include "errors.hpp"

template <typename T>
node<T>::node(const T& value) : value(value), next(nullptr) {}

template <typename T>
s_list<T>::iterator::iterator(node<T>* ptr) : curr(ptr) {}

template <typename T>
typename s_list<T>::iterator& s_list<T>::iterator::operator++() {
    if (curr) curr = curr->next;
    return *this;
}

template <typename T>
T& s_list<T>::iterator::operator*() { return curr->value; }

template <typename T>
const T& s_list<T>::iterator::operator*() const { return curr->value; }

template <typename T>
int s_list<T>::iterator::operator!=(const iterator& other) const {
    return curr != other.curr;
}

template <typename T>
int s_list<T>::iterator::operator==(const iterator& other) const {
    return curr == other.curr;
}

template <typename T>
s_list<T>::const_iterator::const_iterator(const node<T>* ptr) : curr(ptr) {}

template <typename T>
typename s_list<T>::const_iterator& s_list<T>::const_iterator::operator++() {
    if (curr) curr = curr->next;
    return *this;
}

template <typename T>
const T& s_list<T>::const_iterator::operator*() const { return curr->value; }

template <typename T>
int s_list<T>::const_iterator::operator!=(const const_iterator& other) const {
    return curr != other.curr;
}

template <typename T>
int s_list<T>::const_iterator::operator==(const const_iterator& other) const {
    return curr == other.curr;
}

template <typename T>
s_list<T>::s_list() : head(nullptr), tail(nullptr), length(0) {}

template <typename T>
s_list<T>::s_list(unsigned initial_size) : length(initial_size) {
    if (initial_size == 0) {
        head = tail = nullptr;
        return;
    }
    head = new node<T>(T());
    if (head == nullptr) THROW(ERR_MEMORY);
    node<T>* curr = head;
    for (unsigned i = 1; i < initial_size; ++i) {
        curr->next = new node<T>(T());
        if (curr->next == nullptr) THROW(ERR_MEMORY);
        curr = curr->next;
    }
    tail = curr;
    tail->next = nullptr;
}

template <typename T>
s_list<T>::s_list(const T* data, unsigned initial_size) : length(initial_size) {
    if (initial_size == 0) {
        head = tail = nullptr;
        return;
    }
    if (data == nullptr) THROW(ERR_NULL);
    head = new node<T>(data[0]);
    if (head == nullptr) THROW(ERR_MEMORY);
    node<T>* curr = head;
    for (unsigned i = 1; i < initial_size; ++i) {
        curr->next = new node<T>(data[i]);
        if (curr->next == nullptr) THROW(ERR_MEMORY);
        curr = curr->next;
    }
    tail = curr;
    tail->next = nullptr;
}

template <typename T>
s_list<T>::s_list(const s_list& other) : length(other.length) {
    if (other.head == nullptr) {
        head = tail = nullptr;
        return;
    }
    head = new node<T>(other.head->value);
    if (head == nullptr) THROW(ERR_MEMORY);
    node<T>* curr = head;
    node<T>* other_current = other.head->next;
    while (other_current) {
        curr->next = new node<T>(other_current->value);
        if (curr->next == nullptr) THROW(ERR_MEMORY);
        curr = curr->next;
        other_current = other_current->next;
    }
    tail = curr;
    tail->next = nullptr;
}

template <typename T>
s_list<T>::~s_list() {
    node<T>* curr = head;
    while (curr) {
        node<T>* next = curr->next;
        delete curr;
        curr = next;
    }
}

template <typename T>
T& s_list<T>::operator[](unsigned index) {
    if (index >= length) THROW(ERR_INCORRECT_INDEX);
    node<T>* curr = head;
    for (unsigned i = 0; i < index; ++i) curr = curr->next;
    return curr->value;
}

template <typename T>
const T& s_list<T>::operator[](unsigned index) const {
    if (index >= length) THROW(ERR_INCORRECT_INDEX);
    node<T>* curr = head;
    for (unsigned i = 0; i < index; ++i) curr = curr->next;
    return curr->value;
}

template<typename T>
s_list<T>& s_list<T>::operator=(s_list other){
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(length, other.length);
    return *this;
}
template <typename T>
s_list<T> s_list<T>::operator+(const s_list& right) {
    if (length == 0 && right.length == 0) return s_list();
    s_list<T> new_l(length + right.length);
    node<T>* curr = new_l.head;
    node<T>* old_current = head;
    while (old_current) {
        curr->value = old_current->value;
        curr = curr->next;
        old_current = old_current->next;
    }
    old_current = right.head;
    while (old_current) {
        curr->value = old_current->value;
        curr = curr->next;
        old_current = old_current->next;
    }
    return new_l;
}

template <typename T>
unsigned s_list<T>::size() const { return length; }

template <typename T>
T s_list<T>::get_first() const {
    if (length == 0) THROW(ERR_INCORRECT_INDEX);
    return head->value;
}

template <typename T>
T s_list<T>::get_last() const {
    if (length == 0) THROW(ERR_INCORRECT_INDEX);
    return tail->value;
}

template <typename T>
typename s_list<T>::iterator s_list<T>::begin() { return iterator(head); }

template <typename T>
typename s_list<T>::iterator s_list<T>::end() { return iterator(nullptr); }

template <typename T>
typename s_list<T>::const_iterator s_list<T>::begin() const {
    return const_iterator(head);
}

template <typename T>
typename s_list<T>::const_iterator s_list<T>::end() const {
    return const_iterator(nullptr);
}

template <typename T>
s_list<T> s_list<T>::slice(unsigned start, unsigned end) {
    if (start > end || end > length) THROW(ERR_INCORRECT_INDEX);
    s_list<T> result(end - start);
    node<T>* cur = head;
    node<T>* cur_r = result.head;
    for (unsigned i = 0; i < start; ++i) cur = cur->next;
    for (unsigned i = start; i < end; ++i) {
        cur_r->value = cur->value;
        cur_r = cur_r->next;
        cur = cur->next;
    }
    return result;
}