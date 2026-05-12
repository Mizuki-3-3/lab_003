#include "linked_list.hpp"
#include "errors.hpp"

template <typename T>
forward_list<T>::node::node(const T& value) : value(value), next(nullptr) {}


//forward_operator
template <typename T>
T& forward_list<T>::forward_iterator::operator*() const { return curr->value;}

template <typename T>
T* forward_list<T>::forward_iterator::operator->() const { return &curr->value;}


template <typename T>
auto forward_list<T>::forward_iterator::operator++() -> forward_list<T>::forward_iterator& {
    if (curr) curr = curr->next;
    return *this;
}

template <typename T>
auto forward_list<T>::forward_iterator::operator++(int) -> forward_list<T>::forward_iterator {
    forward_iterator tmp = *this;
    if (curr) curr = curr->next;
    return tmp;
}

template <typename T>
bool forward_list<T>::forward_iterator::operator!=(const forward_iterator& other) const {
    return curr != other.curr;
}

template <typename T>
bool forward_list<T>::forward_iterator::operator==(const forward_iterator& other) const {
    return curr == other.curr;
}

//const_forward_operator
template <typename T>
const T& forward_list<T>::const_forward_iterator::operator*() const { return curr->value;}

template <typename T>
const T* forward_list<T>::const_forward_iterator::operator->() const { return &(curr->value);}


template <typename T>
auto forward_list<T>::const_forward_iterator::operator++() -> forward_list<T>::const_forward_iterator& {
    if (curr) curr = curr->next;
    return *this;
}

template <typename T>
auto forward_list<T>::const_forward_iterator::operator++(int) -> forward_list<T>::const_forward_iterator {
    const_forward_iterator tmp = *this;
    if (curr) curr = curr->next;
    return tmp; 
}

template <typename T>
bool forward_list<T>::const_forward_iterator::operator!=(const const_forward_iterator& other) const {
    return curr != other.curr;
}

template <typename T>
bool forward_list<T>::const_forward_iterator::operator==(const const_forward_iterator& other) const {
    return curr == other.curr;
}

template <typename T>
forward_list<T>::forward_list() : head(nullptr), tail(nullptr), length(0) {}

template <typename T>
forward_list<T>::forward_list(unsigned initial_size) noexcept: length(initial_size) {
    if (initial_size == 0) {
        head = tail = nullptr;
        return;
    }
    head = new node(T());
    if (head == nullptr) throw null_ptr();
    node* curr = head;
    for (unsigned i = 1; i < initial_size; ++i) {
        curr->next = new node(T());
        if (curr->next == nullptr) throw null_ptr();
        curr = curr->next;
    }
    tail = curr;
    tail->next = nullptr;
}

template <typename T>
forward_list<T>::forward_list(const T* data, unsigned initial_size) noexcept: length(initial_size) {
    if (initial_size == 0) {
        head = tail = nullptr;
        return;
    }
    if (data == nullptr) throw null_ptr();
    head = new node(data[0]);
    if (head == nullptr) throw null_ptr();
    node* curr = head;
    for (unsigned i = 1; i < initial_size; ++i) {
        curr->next = new node(data[i]);
        if (curr->next == nullptr) throw null_ptr();
        curr = curr->next;
    }
    tail = curr;
    tail->next = nullptr;
}

template <typename T>
forward_list<T>::forward_list(const forward_list& other) noexcept: length(other.length) {
    if (other.head == nullptr) {
        head = tail = nullptr;
        return;
    }
    head = new node(other.head->value);
    if (head == nullptr) throw null_ptr();
    node* curr = head;
    node* other_current = other.head->next;
    while (other_current) {
        curr->next = new node(other_current->value);
        if (curr->next == nullptr) throw null_ptr();
        curr = curr->next;
        other_current = other_current->next;
    }
    tail = curr;
    tail->next = nullptr;
}

template <typename T>
forward_list<T>::~forward_list() {
    node* curr = head;
    while (curr) {
        node* next = curr->next;
        delete curr;
        curr = next;
    }
}

template <typename T>
T& forward_list<T>::operator[](unsigned index) {
    if (index >= length) throw index_out_of_range();
    node* curr = head;
    for (unsigned i = 0; i < index; ++i) curr = curr->next;
    return curr->value;
}

template <typename T>
const T& forward_list<T>::operator[](unsigned index) const {
    if (index >= length) throw index_out_of_range();
    node* curr = head;
    for (unsigned i = 0; i < index; ++i) curr = curr->next;
    return curr->value;
}

template<typename T>
forward_list<T>& forward_list<T>::operator=(forward_list other){
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(length, other.length);
    return *this;
}
template <typename T>
forward_list<T> forward_list<T>::operator+(const forward_list& right) {
    if (length == 0 && right.length == 0) return forward_list();
    forward_list<T> new_l(length + right.length);
    node* curr = new_l.head;
    node* old_current = head;
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
unsigned forward_list<T>::size() const { return length; }

template <typename T>
T forward_list<T>::get_first() const {
    if (length == 0) throw empty_container();
    return head->value;
}

template <typename T>
T forward_list<T>::get_last() const {
    if (length == 0) throw empty_container();
    return tail->value;
}

template <typename T>
auto forward_list<T>::begin() ->  forward_list<T>::forward_iterator { return forward_iterator(head); }

template <typename T>
auto forward_list<T>::end() -> forward_list<T>::forward_iterator { return forward_iterator(nullptr); }

template <typename T>
auto forward_list<T>::begin() const ->  forward_list<T>::const_forward_iterator { return const_forward_iterator(head); }

template <typename T>
auto forward_list<T>::end() const -> forward_list<T>::const_forward_iterator { return const_forward_iterator(nullptr); }


template <typename T>
forward_list<T> forward_list<T>::slice(unsigned start, unsigned end) {
    if (start > end || end > length) throw index_out_of_range();
    forward_list<T> result(end - start);
    node* cur = head;
    node* cur_r = result.head;
    for (unsigned i = 0; i < start; ++i) cur = cur->next;
    for (unsigned i = start; i < end; ++i) {
        cur_r->value = cur->value;
        cur_r = cur_r->next;
        cur = cur->next;
    }
    return result;
}