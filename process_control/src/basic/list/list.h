#ifndef SRC_LIST_LIST_H_
#define SRC_LIST_LIST_H_

#pragma once

#include "list_node.h"


template <typename T> 
class List {
private:
    int size_ = 0;
    ListNode<T> *header_;
    ListNode<T> *trailer_;

protected:
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

     ListNode<T> *operator[] ( int r ) const;

    ListNode<T> *insertAsFirst(T const &e);
    ListNode<T> *insertAsLast(T const &e);

    ListNode<T> *insertAfter(ListNode<T> *p, T const &e);
    ListNode<T> *insertBefore(ListNode<T> *p, T const &e);

    void remove(ListNode<T> *p);

    ListNode<T> *search_size(int n, ListNode<T> *p) const;
    ListNode<T> *search_addr(int n, ListNode<T> *p) const;

    int find(std::string s) const;

    void sort(int flag);



};

#endif // SRC_LIST_LIST_H_