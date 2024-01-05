# Grocery-Queue-Management-System
C++ Program for a grocery queue management system
The simulation aims to replicate customers, checkout registers, and queues at checkout registers within a grocery store. Users will input commands to generate customers, open and close checkout registers. Registers and customers will be incorporated into queues using linked list data structures. The simulation will model customers in both a "single" queue and across "multiple" queues.  
The code recognizes commands in the following format:  
- Open a register: `register open <ID> <secPerItem> <setupTime> <timeElapsed>`  
- Close a register: `register close <ID> <timeElapsed>`  
- Add a customer: `customer <items> <timeElapsed>`  
Here, `<ID>` represents the register ID, `<secPerItem>` denotes the seconds required for the register to process one item, `<setupTime>` is the setup time for each register per customer, `<items>` signifies the number of items each customer has, and `<timeElapsed>` indicates the seconds passed since the last command or action. The user can input commands with a time gap of a few seconds, and `<timeElapsed>` allows simulating various durations between two actions in the grocery store, spanning seconds, minutes, or even hours.
The image below explains the two modes:  
![image](https://github.com/vedanshmehta/Grocery-Queue-Management-System/assets/155503900/efafa791-2e0d-440a-b880-c7bf6029da7b)  
Upon inputting an end-of-file character, the simulation is tasked with computing certain statistics. Specifically, the program is required to determine the wait time for each served customer within the linked list containing all served customers. Subsequently, it should output the maximum, average, and standard deviation of the wait time.  
This program is written in C++, and was given as a lab task under course ECE244H1:Programming Fundamentals at University of Toronto. I hereby claim no authority over the credit of the header files created to mark the lab. The code is written completely by me and is protected under liscence. The source base code consisted of the Make file and .h files which could not be edited as a student, and is property of University of Toronto. The images given above are properties of University of Toronto as well.
