#ifndef STACK_JH_2015
#define STACK_JH_2015
#include <stdint.h>

// Node class, only used by Stack class
class Node {
  private:
    uint16_t data;
    Node *next;
  public:
    Node(uint16_t d);
    Node(uint16_t d, Node *n);
    ~Node();
    void setNext(Node *n);
    uint16_t getData();
    Node *getNext();
};

// Stack class, only allows accessing through standard stack operations
class Stack {
  private:
    Node *head;
  public:
    Stack();
    void push(uint16_t data);
    uint16_t pop();
    uint16_t peek();
    bool isEmpty();
};

#endif
