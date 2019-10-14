#ifndef SRC_BASIC_LIST_NODE_H_
#define SRC_BASIC_LIST_NODE_H_

#pragma once


template <typename T>
class ListNode {
public:
    T data_;
    ListNode<T> *pred_;
    ListNode<T> *succ_;
    
    ListNode() {};
    ListNode(T e, ListNode<T> *p, ListNode<T> *s)
            : data_(e), pred_(p), succ_(s) {}

    ListNode<T> *insertAsSucc(T const &e) {
        ListNode<T> *x = new ListNode(e, this, succ_);
        succ_ -> pred_ = x;
        succ_ = x;
        return x;
    } 

    ListNode<T> *insertAsPred(T const &e) {
        ListNode<T> *x = new ListNode(e, pred_, this);
        pred_ -> succ_ = x;
        pred_ = x;
        return x;
    }
};

#endif // SRC_BASIC_LIST_NODE_H_