#include "list.h"
#include "list_node.h"

template <typename T>
class Queue : public List<T> {
public:
    void enqueue(T const &e) {push_back(e);}
    ListNode<T> *first() {return (List<T>::first());}
    void dequeue() {List<T>::remove(first());}
    T &front() {return first() -> data_;}
};