#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:
void DepartureUpdate(string);

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join

// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);

// Global variables
RegisterList *registerList;  // holding the list of registers
QueueList *doneList;         // holding the list of customers served
QueueList *singleQueue;      // holding customers in a single virtual queue
double expTimeElapsed;  // time elapsed since the beginning of the simulation

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  std::cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      std::cout << "Invalid operation" << endl;
    }
    std::cout << "> ";  // Prompt for input
    getline(cin, line);
  }

  std::cout << "Finished at time " << expTimeElapsed << endl;
  std::cout << "Statistics:" << endl;
  double max = 0;  //-2147483647
  double TotalWaitTime = 0.0;
  int count = 0;
  // Maximum Wait Time
  Customer *current = doneList->get_head();
  while (current != nullptr) {
    if (max < (current->get_departureTime() - current->get_arrivalTime())) {
      max = (current->get_departureTime() - current->get_arrivalTime());
    }
    current = current->get_next();
  }
  std::cout << "Maximum wait time: " << max << endl;

  // Average Wait Time
  current = doneList->get_head();
  while (current != nullptr) {
    TotalWaitTime +=
        (current->get_departureTime() - current->get_arrivalTime());
    count++;
    current = current->get_next();
  }
  double AvgWaitTime = (double)(TotalWaitTime / count);
  std::cout << "Average wait time: " << AvgWaitTime << endl;

  // Standard deviation
  current = doneList->get_head();
  double SumofDifferences = 0.0;
  while (current != nullptr) {
    SumofDifferences =
        SumofDifferences +
        pow((current->get_departureTime() - current->get_arrivalTime()) -
                AvgWaitTime,
            2);
    current = current->get_next();
  }
  double StdDev = (double)sqrt(SumofDifferences / count);
  std::cout << "Standard Deviation of wait time: " << StdDev << endl;
  // doneList->print();
  //  You have to make sure all dynamically allocated memory is freed
  //  before return 0
  //~registerList();
  /*Register* t = registerList->get_head();

  if(t != NULL){
    while(t != NULL){
      t->get_queue_list()->empty_queue();
      Register* n = t->get_next();

      delete t;
      t = n;
    }
  }
    delete registerList;
singleQueue->empty_queue();
  delete singleQueue;
        doneList->empty_queue();
  delete doneList;
  /*if (singleQueue->empty_queue()) {
    //~singleQueue();
    delete singleQueue;
  } else {
    // ~singleQueue();
    delete singleQueue;
  }
  if (doneList->empty_queue()) {
    // ~doneList();
    delete doneList;
  } else {
    //  ~doneList();
    delete doneList;
  }*/
  delete registerList;
  delete doneList;
  delete singleQueue;
  return 0;
}

string getMode() {
  string mode;
  std::cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  std::cout << "Enter \"single\" if you want to simulate a single queue or "
               "\"multiple\" to "
               "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    std::cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    std::cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    std::cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    std::cout << "Error: too many arguments." << endl;
    return;
  }
  // Depending on the mode of the simulation (single or multiple),
  // add the customer to the single queue or to the register with
  // fewest items
  expTimeElapsed += timeElapsed;
  DepartureUpdate(mode);
  Customer *newCustomer = new Customer(expTimeElapsed, items);
  if (mode == "single") {
    singleQueue->enqueue(newCustomer);
    std::cout << "A customer entered" << endl;
    if (registerList->get_free_register() == nullptr) {
      std::cout << "No free registers" << endl;
    }
  } else if (mode == "multiple") {
    int ID = registerList->get_min_items_register()->get_ID();
    registerList->get_min_items_register()->get_queue_list()->enqueue(
        newCustomer);
    std::cout << "A customer entered" << endl;
    std::cout << "Queued a customer with quickest register " << ID << endl;
  }
  DepartureUpdate(mode);
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    std::cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    std::cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    std::cout << "Error: too many arguments" << endl;
    return;
  }

  // Check if the register is already open
  // If it's open, print an error message
  // Otherwise, open the register
  // If we were simulating a single queue,
  // and there were customers in line, then
  // assign a customer to the new register
  if (registerList->foundRegister(ID)) {
    std::cout << "Error: register " << ID << " is already open" << endl;
    return;
  }
  expTimeElapsed += timeElapsed;
  DepartureUpdate(mode);
  Register *newRegister =
      new Register(ID, secPerItem, setupTime, expTimeElapsed);
  registerList->enqueue(newRegister);
  std::cout << "Opened register " << ID
            << endl;  // Should this happen before or after DepartureUpdate?
                      // registerList->print();
  DepartureUpdate(mode);
  /* if (mode == "single" && singleQueue->get_head() != nullptr) {
     newRegister->get_queue_list()->enqueue(singleQueue->dequeue());  // *** ??
     std::cout << "Queued a customer with free register " << ID << endl;
   }*/
  // else if (mode == "multiple") {
  // COME BACK LATER
  // }
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    std::cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    std::cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  // If it is open dequeue it and free it's memory
  // Otherwise, print an error message
  expTimeElapsed += timeElapsed;
  DepartureUpdate(mode);
  if (registerList->foundRegister(ID)) {
    delete registerList->dequeue(ID);
    std::cout << "Closed register " << ID << endl;
  } else {
    std::cout << "Error: register " << ID << " is not open" << endl;
    return;
  }
  DepartureUpdate(mode);
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}

void DepartureUpdate(string mode) {
  int ID;
  double departTime;
  // What might be easier to read: use
  if (mode == "single") {
    while (singleQueue->get_head() != nullptr &&
           registerList->get_free_register() != nullptr) {
      Register *reg = registerList->get_free_register();
      ID = reg->get_ID();
      if (mode == "single") {
        Customer *temp = singleQueue->get_head();
        if (temp == nullptr) {
          reg = registerList->get_free_register();
          continue;
        }
        if (registerList->get_free_register() != nullptr) {
          // If there is a free register remaining after you add a
          // customer to a different free register, this
          // expression will evaluate as false and what is in
          // else{} will be executed
          reg->get_queue_list()->enqueue(
              singleQueue
                  ->dequeue());  // This is not going to work because now
                                 // there is a new minDepartTimeRegister! You
                                 // just departed the customer above!  // Need
                                 // std::cout?? Also if you do this here, make
                                 // sure not to do it *again* for the same
                                 // customer elsewhere (ie. ***)
          std::cout << "Queued a customer with free register " << ID << endl;
        }
      }
      // reg = registerList->get_free_register();
      DepartureUpdate(mode);
    }
  }
  // for all of this do if (registerList->head() != nullptr)
  Register *r = registerList->get_head();
  if (registerList->get_head() != nullptr) {
    Register *reg =
        registerList->calculateMinDepartTimeRegister(expTimeElapsed);
    /*if (reg == nullptr ) {
      if (mode == "single") {
        int ID_new=registerList->get_head()->get_ID();
        registerList->get_head()->get_queue_list()->enqueue(singleQueue->dequeue());
        std::cout<<"Queued a customer with free register "<<ID_new<<endl;
      }
      //see for multiple;
    }*/
    QueueList *q = nullptr;
    Customer *c = nullptr;
    bool ETA;
    if (reg) {
      q = reg->get_queue_list();

      ETA = expTimeElapsed >= reg->calculateDepartTime();

      if (reg->get_queue_list()) {
        c = reg->get_queue_list()->get_head();
      }
    }
    bool AHHHH = false;

    while (reg != nullptr && reg->get_queue_list() != nullptr &&
           reg->get_queue_list()->get_head() !=
               nullptr &&  // How will this behave when you
                           // have >1 registers? (might be ok)
           expTimeElapsed >=
               reg->calculateDepartTime()  // for this condition: what if you
                                           // want to add customers to a
                                           // register from a single queue but
                                           // no customers need to depart?
    ) {
      ID = reg->get_ID();
      Customer *departingCust = reg->get_queue_list()->get_head();
      reg->departCustomer(doneList);
      //->get_departureTime();
      //  departTime = reg->get_queue_list()->get_head()->get_departureTime();
      departTime = departingCust->get_departureTime();  // added- John

      std::cout << "Departed a customer at register ID " << ID << " at "
                << departTime << endl;
      if (mode == "single") {
        Customer *temp = singleQueue->get_head();
        if (temp == nullptr) {
          // reg = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
          DepartureUpdate(mode);
          continue;
        }
        // int ID=
        // registerList->calculateMinDepartTimeRegister(expTimeElapsed)->get_ID();

        if (registerList->get_free_register() != nullptr) {
          // If there is a free register remaining after you add a
          // customer to a different free register, this
          // expression will evaluate as false and what is in
          // else{} will be executed
          reg->get_queue_list()->enqueue(
              singleQueue
                  ->dequeue());  // This is not going to work because now
                                 // there is a new minDepartTimeRegister! You
                                 // just departed the customer above!  // Need
                                 // std::cout?? Also if you do this here, make
                                 // sure not to do it *again* for the same
                                 // customer elsewhere (ie. ***)
          std::cout << "Queued a customer with free register " << ID << endl;
        }
        /*else {
          std::cout << "No free registers"
               << endl;  // If there are no free registers, you should not
                         // enqueue as you did above.
        }*/
        // continue;
      }
      // reg = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
      DepartureUpdate(mode);
    }
  }

  return;
}