#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {
  if (head != nullptr) {
    delete head;
    head = nullptr;
  }
}

Customer* QueueList::get_head() { return head; }

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head
  if (head == nullptr) {
    head = customer;
    return;
  }

  Customer* current = head;

  while (current->get_next() != nullptr) {
    current = current->get_next();
  }
  current->set_next(customer);
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue
  // and return a pointer to it
  if (head == nullptr) {
    return nullptr;
  }

  Customer* previousHead = head;
  head = previousHead->get_next();
  previousHead->set_next(nullptr);
  return previousHead;
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has
  int sum = 0;
  Customer* current = head;
  while (current != nullptr) {
    sum += current->get_numOfItems();
    current = current->get_next();
  }
  return sum;
}

bool QueueList::empty_queue() {
  // if the queue is empty, return false
  // if the queue is not empty, delete all customers
  // and return true
  if (head == nullptr) {
    return false;
  }
  // ~Customer();
  delete head;
  head = nullptr;
  return true;
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
