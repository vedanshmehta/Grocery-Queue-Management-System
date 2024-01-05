#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  // Delete all registers in the list
  if (head != nullptr) {
    delete head;
    head = nullptr;
  }
}

Register* RegisterList::get_head() { return head; }

int RegisterList::get_size() {
  // return number of registers
  return size;
}

Register* RegisterList::get_min_items_register() {
  // loop all registers to find the register with least number of items
  Register* current = head;
  double min = 2147483647;
  Register* minItemregister = nullptr;
  while (current != nullptr) {
    if (min > current->get_queue_list()->get_items()) {
      min = current->get_queue_list()->get_items();
      minItemregister = current;
    }
    current = current->get_next();
  }
  return minItemregister;
}

Register* RegisterList::get_free_register() {
  // return the register with no customers
  // if all registers are occupied, return nullptr
  Register* current = head;
  while (current != nullptr) {
    if (current->get_queue_list()->get_head() == nullptr) {
      return current;
    }
    current = current->get_next();
  }
  return nullptr;
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size
  if (head == nullptr) {
    head = newRegister;
    return;
  }

  Register* current = head;

  while (current->get_next() != nullptr) {
    current = current->get_next();
  }
  current->set_next(newRegister);
  size++;
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise
  Register* current = head;

  while (current != nullptr) {
    if (current->get_ID() == ID) {
      return true;
    }
    current = current->get_next();
  }
  return false;
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID

  // return the dequeued register
  // return nullptr if register was not found
  /*if (head == nullptr) {
    return nullptr;
  }

  Register* current = head;
  Register* previous = nullptr;
  while (current != nullptr) {
    if (current->get_ID() == ID) {
      if (previous == nullptr) {
        head = nullptr;
        return current;
      }
      previous->set_next(current->get_next());
      current->set_next(nullptr);
      return current;
    }
    previous = current;
    current = current->get_next();
  }

  return nullptr;
}*/
  Register* current = head;
  Register* next = current->get_next();
  if (current != nullptr) {
    if (current->get_ID() == ID) {
      head = next;
      current->set_next(nullptr);
      return current;
    }
  }
  Register* previous = head;
  current = previous->get_next();
  if (previous != NULL) {
    if (current->get_ID() == ID) {
      next = previous->get_next();
      current->set_next(nullptr);
      return current;
    }
  }
  return nullptr;
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr
  Register* current = head;
  // double min = expTimeElapsed;
  //  2147483647;
  double min = 2147483647;
  Register* MinDepartTimeRegister = nullptr;
  // if free register thing
  /* while (current != nullptr) {

     if (current->calculateDepartTime() != -1  // added (John)
         && min > current->calculateDepartTime()) {
       min = current->calculateDepartTime();
       MinDepartTimeRegister = current;
     }
     current = current->get_next();
   }
   return MinDepartTimeRegister;*/
  while (current != nullptr) {
    if (current->calculateDepartTime() == -1 ||
        current->calculateDepartTime() > expTimeElapsed) {
      current = current->get_next();
      continue;
    }
    if (current->calculateDepartTime() < min) {
      min = current->calculateDepartTime();
      MinDepartTimeRegister = current;
    }

    current = current->get_next();
  }
  return MinDepartTimeRegister;
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
