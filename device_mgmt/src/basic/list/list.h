#ifndef SRC_BASIC_LIST_H_
#define SRC_BASIC_LIST_H_

#pragma once

#include "list_node.h"
#include <string>

template <typename T> 
class List {
private:
    ListNode<T> *header_;
    ListNode<T> *trailer_;

protected:

    int size_ = 0;
    void init();
    int clear();

public:
    List() { init(); }
    ~List() = default;
    int size() const { 
        return size_; 
    }

    bool empty() { 
        return (size_ == 0); 
    }

    ListNode<T> *first() const {
        return header_ -> succ_;
    }

    ListNode<T> *last() const {
        return trailer_ -> pred_;
    }

    T &front()const {
        return  header_ -> succ_ -> data_;
    }

    T &back() const {
        return trailer_-> pred_ -> data_;
    }


    ListNode<T> *operator[] ( int r ) const;

    ListNode<T> *insertAsFirst(T const &e);
    ListNode<T> *push_back(T const &e);

    ListNode<T> *push(T *e);

    ListNode<T> *insertAfter(ListNode<T> *p, T const &e);
    ListNode<T> *insertBefore(ListNode<T> *p, T const &e);

    T &remove(ListNode<T> *p);

    ListNode<T> *search_size(int n, ListNode<T> *p) const;
    ListNode<T> *search_addr(int n, ListNode<T> *p) const;

    bool check(std::string);

    void sort(int flag);

    int find(std::string) const;
    int find_type(std::string name) const;

};

#endif // SRC_BASIC_LIST_H_