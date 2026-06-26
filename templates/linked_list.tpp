#include "linked_list.hpp"
#include "errors.hpp"
#include <initializer_list>

template <typename T>
forward_list<T>::node::node(const T& value) : value(value), next(nullptr) {}

//forward_iterator
template <typename T>
T& forward_list<T>::iterator::operator*() const { return curr->value;}

template <typename T>
T* forward_list<T>::iterator::operator->() const { return &curr->value;}


template <typename T>
auto forward_list<T>::iterator::operator++() -> forward_list<T>::iterator& {
    if (curr) curr = curr->next;
    return *this;
}

template <typename T>
auto forward_list<T>::iterator::operator++(int) -> forward_list<T>::iterator {
    iterator tmp = *this;
    if (curr) curr = curr->next;
    return tmp;
}

template <typename T>
auto forward_list<T>::iterator::operator+(int right) -> forward_list<T>::iterator {
    iterator tmp = *this;
    for(int i = 0; i < right; i++){
        if (tmp.curr) tmp.curr = tmp.curr->next;
    }
    return tmp; 
}

template <typename T>
bool forward_list<T>::iterator::operator!=(const iterator& other) const {
    return curr != other.curr;
}

template <typename T>
bool forward_list<T>::iterator::operator==(const iterator& other) const {
    return curr == other.curr;
}

//const_forward_iterator
template <typename T>
const T& forward_list<T>::const_iterator::operator*() const { return curr->value;}

template <typename T>
const T* forward_list<T>::const_iterator::operator->() const { return &(curr->value);}


template <typename T>
auto forward_list<T>::const_iterator::operator++() -> forward_list<T>::const_iterator& {
    if (curr) curr = curr->next;
    return *this;
}

template <typename T>
auto forward_list<T>::const_iterator::operator++(int) -> forward_list<T>::const_iterator {
    const_iterator tmp = *this;
    if (curr) curr = curr->next;
    return tmp; 
}


template <typename T>
auto forward_list<T>::const_iterator::operator+(int right) -> forward_list<T>::const_iterator {
    const_iterator tmp = *this;
    for(int i = 0; i < right; i++){
        if (tmp.curr) tmp.curr = tmp.curr->next;
    }
    return tmp;
}


template <typename T>
bool forward_list<T>::const_iterator::operator!=(const const_iterator& other) const {
    return curr != other.curr;
}

template <typename T>
bool forward_list<T>::const_iterator::operator==(const const_iterator& other) const {
    return curr == other.curr;
}
//froward_list
template <typename T>
forward_list<T>::forward_list() : M_head(new node(T())), tail(nullptr), length(0){}

template <typename T>
forward_list<T>::forward_list(size_t initial_size): length(initial_size) {
    if (initial_size == 0) {
        M_head = new node(T());
        tail = nullptr;
        return;
    }
    M_head = new node(T());
    if (M_head == nullptr) throw null_ptr();
    node* curr = M_head;
    for (size_t i = 0; i < initial_size; i++) {
        curr->next = new node(T());
        if (curr->next == nullptr) throw null_ptr();
        curr = curr->next;
    }
    tail = curr;
    tail->next = nullptr;
}

template <typename T>
forward_list<T>::forward_list(const T* data, size_t initial_size): length(initial_size) {
    if (initial_size == 0) {
        M_head = new node(T());
        tail = nullptr;
        return;
    }
    if (data == nullptr) throw null_ptr();
    M_head = new node(T());
    if (M_head == nullptr) throw null_ptr();
    node* curr = M_head;
    for (size_t i = 0; i < initial_size; i++) {
        curr->next = new node(data[i]);
        if (curr->next == nullptr) throw null_ptr();
        curr = curr->next;
    }
    tail = curr;
    tail->next = nullptr;
}

template <typename T>
forward_list<T>::forward_list(const forward_list& other): length(other.length) {
    if (other.M_head == nullptr) {
        tail = nullptr;
        return;
    }
    M_head = new node(other.M_head->value);
    if (M_head == nullptr) throw null_ptr();
    node* curr = M_head;
    node* other_current = other.M_head->next;
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
forward_list<T>::forward_list(forward_list&& move){
    length = std::move(move.length);
    M_head = std::move(move.M_head);
    tail = std::move(move.tail);
}

template <typename T>
forward_list<T>::forward_list(const std::initializer_list<T> initial_l): length(initial_l.size()) {
    M_head = new node(T());
    node* curr = M_head;
    for (const T& val : initial_l){
        curr->next = new node(val);
        curr = curr->next;
    }
    tail = curr;
    tail->next = nullptr;
}

template <typename T>
forward_list<T>::~forward_list() {
    node* curr = M_head;
    while (curr) {
        node* next = curr->next;
        delete curr;
        curr = next;
    }
}

template <typename T>
T& forward_list<T>::operator[](size_t index) {
    if (index >= length) throw index_out_of_range();
    node* curr = M_head;
    for (size_t i = 0; i <= index; i++) curr = curr->next;
    return curr->value;
}

template <typename T>
const T& forward_list<T>::operator[](size_t index) const {
    if (index >= length) throw index_out_of_range();
    node* curr = M_head;
    for (size_t i = 0; i <= index; i++) curr = curr->next;
    return curr->value;
}

template<typename T>
forward_list<T>& forward_list<T>::operator=(forward_list other){
    std::swap(M_head, other.M_head);
    std::swap(tail, other.tail);
    std::swap(length, other.length);
    return *this;
}
template <typename T>
forward_list<T> forward_list<T>::operator+(const forward_list& right) {
    if (length == 0 && right.length == 0) return forward_list();
    forward_list<T> new_l(length + right.length);
    node* curr = new_l.M_head->next;
    node* old_current = M_head->next;
    while (old_current) {
        curr->value = old_current->value;
        curr = curr->next;
        old_current = old_current->next;
    }
    old_current = right.M_head->next;
    while (old_current) {
        curr->value = old_current->value;
        curr = curr->next;
        old_current = old_current->next;
    }
    return new_l;
}

template <typename T>
size_t forward_list<T>::size() const { return length; }

template <typename T>
T forward_list<T>::front() const {
    if (length == 0) throw empty_container();
    return M_head->next->value;
}

template <typename T>
T forward_list<T>::back() const {
    if (length == 0) throw empty_container();
    return tail->value;
}

template <typename T>
auto forward_list<T>::begin() ->  forward_list<T>::iterator { return iterator(M_head->next); }

template <typename T>
auto forward_list<T>::before_begin() ->  forward_list<T>::iterator { return iterator(M_head); }

template <typename T>
auto forward_list<T>::end() -> forward_list<T>::iterator { return iterator(nullptr); }

template <typename T>
auto forward_list<T>::before_end() -> forward_list<T>::iterator { return iterator(tail); }

template <typename T>
auto forward_list<T>::begin() const ->  forward_list<T>::const_iterator { return const_iterator(M_head->next); }

template <typename T>
auto forward_list<T>::end() const -> forward_list<T>::const_iterator { return const_iterator(nullptr); }


template <typename T>
forward_list<T> forward_list<T>::slice(size_t start, size_t end) {
    if (start > end || end > length) throw index_out_of_range();
    forward_list<T> result(end - start);
    node* cur = M_head->next;
    node* cur_r = result.M_head->next;
    for (size_t i = 0; i < start; i++) cur = cur->next;
    for (size_t i = start; i < end; i++) {
        cur_r->value = cur->value;
        cur_r = cur_r->next;
        cur = cur->next;
    }
    return result;
}

template <typename T>
forward_list<T>* forward_list<T>::insert_after(const_iterator place, const T& value){
    node* new_node = new node(value);
    if (length == 0) {
    M_head->next = new node(value);
    tail = M_head->next;
    length = 1;
    return this;
    }
    if (place == iterator(tail)){
        tail->next = new_node; 
        tail = new_node;
        length++;
        return this;
    }else if(place == before_begin()){
        new_node->next = M_head->next;
        M_head->next = new_node;
        length++;
        return this;
    }
    iterator current = begin();
    node* curr_node = M_head->next;
    while (current != place && current != end()){
        current++;
        curr_node = curr_node->next;
    }
    if (current == end()){
        delete new_node;
        throw index_out_of_range();
    }
    new_node->next = curr_node->next;
    curr_node->next = new_node;
    length++;
    return this;
}