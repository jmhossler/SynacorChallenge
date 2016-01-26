#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include "stack.h"

Node::Node(uint16_t d) {
  data = d;
  next = NULL;
}

Node::Node(uint16_t d, Node *n) {
  data = d;
  next = n;
}

Node::~Node() {

}

void Node::setNext(Node *n) {
  next = n;
}

uint16_t Node::getData() {
  return data;
}

Node *Node::getNext() {
  return next;
}

Stack::Stack() {
  head = NULL;
}

void Stack::push(uint16_t data) {
  head = new Node(data,head);
}

uint16_t Stack::pop() {
  if(this->isEmpty()) {
    return 0;
    std::cerr << "Stack is empty\n";
    exit(1);
  }
  uint16_t val = head->getData();
  Node *temp = head;
  head = head->getNext();
  delete temp;
  return val;
}

uint16_t Stack::peek() {
  if(this->isEmpty()) {
    std::cerr << "Stack is empty\n";
    exit(1);
  }
  return head->getData();
}

bool Stack::isEmpty() {
  if(head == NULL) {
    return true;
  }
  return false;
}
