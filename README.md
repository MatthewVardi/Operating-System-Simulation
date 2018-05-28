<strong>Operating Systems Simulation in C++</strong>

Features:
 1. Multi-level Feedback Queue
 2. I/O Disk Management
 3. Memory Management 

Class: Spring 2018, Operating Systems Section 3

Professor: Professor Shostak

To compile on terminal type:
 g++ -std=c++11 OS.cpp


To run the executable type:
 ./a.out

List of commands:

       A - Create a new process with its own process block

       t - Terminate the process that currently is using the CPU.

       d # file_name - The process that is using the CPU requests the hard disk numbee it wants to read or write file file_name.

       D # - The hard disk # has finished the work for one process.

       Q - Currently running process has spent a time quantum using the CPU.

       m address - Process that is currently using the CPU requests a memory operation for the logical address.

       S r - Snapshot of the process that is in the CPU and the processes that wait on each ready-queue level.

       S i - Shows what processes are currently using the I/O devices and the processes that are waiting to use them.

       S m - Shows the state of Memory.

       X - Exits the application immediately.


Note: When printing memory table, entries with all 0's refers to an empty / unoccupied row


