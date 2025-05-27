#ifndef FA23_CS20A_LlST_H
#define FA23_CS20A_LlST_H

#include<iostream>
#include<assert.h>
#include <codecvt>
// Linked DLList object that maintains both head and tail pointers
// and the count of the list.  Note that you have to keep the head,
// tail and count consistent with the intended state of the DLList 
// otherwise very bad things happen.

template<typename Item>
class DLList {
public:
    DLList();

    DLList(const DLList<Item> &other);

    DLList<Item> &operator=(const DLList<Item> &other);

    ~DLList();

    void print() const;

    bool empty() const;

    void add_front(const Item &itm);

    void add_rear(const Item &itm);

    void add(int idx, const Item &itm);

    // Note that the user must head ensure the list is not empty
    // prior to calling these functions.
    Item front() const;

    Item rear() const;

    Item peek(int idx) const;

    int size() const;

    int items(const Item &itm) const;

    int search(const Item &itm) const;

    bool remove_front();

    bool remove_rear();

    bool remove_index(int idx);

    int remove_item(const Item &itm);


    bool sub_list(const DLList<Item> &sub);

#ifndef MAKE_MEMBERS_PUBLIC

private:
#endif
    // Forward declare the nodes for our DLList.
    // Will be implemented along with list
    // member functions
    class Node;

    // We'll have both head and tail points for
    // Fast insertion/deletion from both ends.
    Node *head;
    Node *tail;

    // Keep track of number of nodes in the list
    int count;
};


/* DLList Implementation
//
//  Since DLList is a template class (which is not an actual
//  class yet, not until we actually instantiate the list)
//  we need to keep the implementation together with
//  the definition.  There are ways to simulate having
//  separate "implementation/definition" with templates,
//  but they aren't necessary and can be confusing.
*/

/* Node definition
//		Already implemented, nothing to do here but to use it.
*/
template<typename Item>
class DLList<Item>::Node {
public:
    Node() : next(nullptr), prev(nullptr) {
    }

    Node(Item i, Node *p, Node *n) : item(i), next(n), prev(p) {
    }

    Node *nxt() const { return next; }
    void nxt(Node *n) { next = n; }

    Node *prv() const { return prev; }
    void prv(Node *p) { prev = p; }

    Item itm() const { return item; }
    void itm(const Item &i) { item = i; }

private:
    Item item;
    Node *next;
    Node *prev;
};


/* DLList default constructor
//		Already implemented, nothing to do.
*/
template<typename Item>
DLList<Item>::DLList() : head(nullptr), tail(nullptr), count(0) {
    //default constructor
}


/* Copy constructor
*/
template<typename Item>
DLList<Item>::DLList(const DLList<Item> &other): head(nullptr), tail(nullptr), count(0) {
    //if the other list is empty, do nothing
    if (other.empty())
        return;
    //temporary node to iterate through the other list
    Node* other_current = other.head;
    //iterate through the other list
    //add each item to the rear of this list
    //to preserve order
    while (other_current != nullptr) {
        add_rear(other_current->itm());
        other_current =  other_current->nxt();
    }
}

/* Overloaded assignment operator
*/
template<typename Item>
DLList<Item> &DLList<Item>::operator=(const DLList<Item> &other) {
    //check for self assignment
    if (this == &other)
        return *this;
    //delete the nodes current list
    while(!empty())
       remove_front();
    //temporary node to iterate through the other list
    Node* other_current = other.head;
    //iterate through the other list
    //add each item to the rear of this list
    //to preserve order
    while (other_current != nullptr) {
        add_rear(other_current->itm());
        other_current = other_current->nxt();
    }
    return *this;
}


/* DLList destructor
*/
template<typename Item>
DLList<Item>::~DLList() {
    //delete all nodes in the list
    while(!empty())
        remove_front();

    //set to empty state
    head = nullptr;
    tail = nullptr;
    count = 0;

}

/* DLList print
*/

template<typename Item>
void DLList<Item>::print() const {
    //temporary node to iterate through the list
    Node* current = head;
    //iterate through the list
    //print each item
    //if current is not the tail, print a space
    while (current !=  nullptr) {
        std::cout << current -> itm();
        if (current != tail)
            std::cout << " ";
        current = current->nxt();
    }
}

/* DLList empty
*/
template<typename Item>
bool DLList<Item>::empty() const {
    //check if the list is empty
    //head and tail should be null and count should be 0
    return (head == nullptr) && (tail == nullptr) && (count == 0);
}


/* DLList add_front
*/
template<typename Item>
void DLList<Item>::add_front(const Item &itm) {
    // create and initialize a new node
    Node *newNode = new Node;
    newNode->itm(itm);
    //check if the list is empty
    //else link the new node to the head
    if (empty()) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->nxt(head);
        head->prv(newNode);
        head = newNode;
    }
    //increment the count
    count++;
}

/* DLList add_rear
*/
template<typename Item>
void DLList<Item>::add_rear(const Item &itm) {
    //if the list is empty
    //call add_front for the first item
    //else link the new node to the tail
    if (empty())
        add_front(itm);
    else {
        Node *newNode = new Node;
        newNode->itm(itm);
        newNode->prv(tail);
        tail->nxt(newNode);
        tail = newNode;
        count++;
    }
}

/* DLList add
*/
template<typename Item>
void DLList<Item>::add(int idx, const Item &itm) {
    //three cases:
    //idx < 0 - add to front
    //index > count - add to rear
    //index in between - traverse to the index and add
    if (idx <= 0) {
        add_front(itm);
    } else if (idx >= count) {
        add_rear(itm);
    } else {
        int index = 0;
        Node *current = head;
        //find the index
        while (index < idx) {
            current = current->nxt();
            index++;
        }
        //create a new node and link it to the current node
        Node *newNode = new Node(itm, current->prv(), current);
        current->prv()->nxt(newNode);
        current->prv(newNode);
        count++;
    }
}

/*  DLList front
*/
template<typename Item>
Item DLList<Item>::front() const {
    // Force check that the head is pointing to a Node
    // Typical solution for deployement code is to throw
    // exceptions, but since we haven't covered that yet
    // we'll make due with assert, which is used for testing
    assert(head != nullptr);

    //return item in the head node
    return head->itm();
}

/* DLList rear
*/
template<typename Item>
Item DLList<Item>::rear() const {
    // Force check that the tail is pointing to a Node
    // Typical solution for deployement code is to throw
    // exceptions, since we haven't covered that yet
    // we'll make due with assert, which is used for testing
    assert(tail != nullptr);

    //return item in the tail node
    return tail->itm();
}

/* DLList peek
*/
template<typename Item>
Item DLList<Item>::peek(int idx) const {
    // Force index to be correct before getting the Item
    // Typical solution for deployement code is to throw
    // exceptions, since we haven't covered that yet
    // we'll make due with assert, which is used for testing
    assert(idx >= 0 && idx < count);
    int index = 0;
    Node *current = head;
    //traverse to the index
    while (index < idx) {
        current = current->nxt();
        index++;
    }
    //return the item in the node
    return current->itm();
}

/* DLList size
*/
template<typename Item>
int DLList<Item>::size() const {
    //return the count(number of nodes) in the list
    return count;
}

template<typename Item>
int DLList<Item>::items(const Item &itm) const {
    int count = 0;
    Node *current = head;
    //traverse through the list
    while (current != nullptr) {
        //check if the item in the node is equal to the item passed
        //if so, increment the count
        if (current->itm() == itm)
            count++;
        current = current->nxt();
    }
    //return the count(number of times the item is in the list)
    return count;
}

/* DLList search
*/
template<typename Item>
int DLList<Item>::search(const Item &itm) const {
    int index = 0;
    //flag to check if the item is found
    bool found = false;
    Node *current = head;
    //traverse through the list
    while (current != nullptr) {
        //if the item in the node is equal to the item passed
        //set the flag to true and break
        if (current->itm() == itm) {
            found = true;
            break;
        }
        //continue traversing
        current = current->nxt();
        index++;
    }
    //if the item is not found, return -42
    if (!found)
        return -42;
    //return the index of the item
    return index;
}

/* DLList remove_front
*/
template<typename Item>
bool DLList<Item>::remove_front() {
    //check if the list is empty
    if (empty())
        return false;
    //if the list has only one node
    //delete the head and set head and tail to null
    //delete the node and move the head to the next node
    if (size() == 1) {
        delete head;
        head = nullptr;
        tail = nullptr;
    } else {
        Node *garbage = head;
        head = head->nxt();
        head->prv(nullptr);
        delete garbage;
    }
    //decrement the count
    count--;
    return true;
}

/* DLList remove_rear
*/
template<typename Item>
bool DLList<Item>::remove_rear() {
    //check if the list is empty
    if (empty())
        return false;
    //if the list has only one node
    //call remove_front
    //else delete the tail and set tail to the previous node
    if (size() == 1) {
        return remove_front();
    } else {
        Node *garbage = tail;
        tail = tail->prv();
        tail->nxt(nullptr);
        delete garbage;
    }
    //decrement the count
    count--;

    return true;
}

/* DLList remove_index
*/
template<typename Item>
bool DLList<Item>::remove_index(int idx) {
    //if the index is out of bounds
    //or the list is empty, return false
    if (idx < 0 || idx >= size() || empty())
        return false;
    //if the index is 0, call remove_front
    //if the index is the last index, call remove_rear
    //else traverse to the index and delete the node
    if (idx == 0)
        return remove_front();
    else if (idx == size() - 1)
        return remove_rear();
    else {
        int index = 1;
        Node *current = head->nxt();
        while (index < idx) {
            current = current->nxt();
            index++;
        }
        //link the previous node to the next node
        current->prv()->nxt(current->nxt());
        current->nxt()->prv(current->prv());
        delete current;
    }
    //decrement the count
    count--;
    return true;
}

/* DLList remove_item
*/
template<typename Item>
int DLList<Item>::remove_item(const Item &itm) {
    //check if the item is in the list
    //if not found, search returns -42
    int index = search(itm);
    //if index >= 0, remove the item
    if (index >= 0)
        remove_index(index);
    return index;
}

/* DLList sub_list
*/
template<typename Item>
bool DLList<Item>::sub_list(const DLList<Item> &sub) {
    //an empty list is a sub list of any list
    if (sub.empty())
        return true;
    //check if the sub list is larger than the list
    if (empty() || sub.size() > size())
        return false;

    Node *current_node = head;
    Node *sub_current = sub.head;
    int count = 0;
    //traverse through the list
    while (current_node != nullptr) {
        //check if the item in the node is equal to the item in the sub list
        //if move to the next node in the sub list and increment the count
        //else reset the sub list to the head and set count to 0
        if (sub_current->itm() == current_node->itm()) {
            sub_current = sub_current->nxt();
            count++;
            //check if the count is equal to the size of the sub list
            if (count == sub.size())
                return true;
        } else {
            sub_current = sub.head;
            count = 0;
            //after resetting the sub list, check if the item in the node is equal to the item in the sub list
            //if so, don't move to the next node before accounting for the count
            if (sub_current->itm() == current_node->itm())
                continue;
        }
        current_node = current_node->nxt();
    }
    return false;
}

#endif


// _X_XMMXXI
