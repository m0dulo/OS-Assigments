#include "list.h"
#include "list_node.h"
#include <iostream>

template <typename T>
class Queue : public List<T> {
public:
    void enqueue(T const &e) {Queue::push_back(e);}
    ListNode<T> *first() {return (List<T>::first());}
    ListNode<T> *last() {return (List<T>::last());}
    void dequeue() {List<T>::remove(List<T>::first());}
    T &front() {return first() -> data_;}
    int size() {return (Queue::size_);}
    void init(int size) {
        for (size_t i = 0; i < size; i++) {
            Queue::push_back(-1);
        }
    }
    bool is_top() {
        bool flag = true;
        for (int i = 0; i < Queue::size_; i++) {
            flag = flag && ((*this)[i] -> data_ != -1);
        }
        return flag;
    }
    T trailer() {return last() -> data_;}
};