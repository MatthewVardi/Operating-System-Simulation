
/***************************************************************
Title: Components.h
Author: Matthew Vardi
Date Created: 4/3/18
Class: Spring 2018, Operating Systems Section 3
Professor: Professor Shostak
Description: Simulating an Operating System With Multi-level Feedback Queue,
             I/O Disks and Memory Management in C++
***************************************************************/


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque> 
#include <iomanip>

using namespace std;

//Process Class
class processBlock {
  public:
    int size_, id_, level_,quantum_;
    string IOfilename = "";
    //Print a process
    void printID() {

      if (id_ == 0) {
        cout << "CPU IS IDLE" << endl;
      } else {
        cout << "PID: " << id_ << " Level: "
        << level_ << " Time Slice: " << quantum_ << endl;
      }
      
    }
    //Print IO
    void printIO() {

      if (id_ == 0) {
        cout << "CPU IS IDLE" << endl;
      } else {
        cout << "PID: " << id_ << " Level: "
        << level_ << " Time Slice: " << quantum_ << " I/O FileName: " << IOfilename << endl;
      }
      
    }
    
    processBlock(int pageSize,int id) {
      id_ = id;
      size_ = pageSize;
      level_ = 0;
      quantum_ = 0;
    };
};

//Memory Management for memory table
class MemManegment {
 public:
  int frame;
  int page = 0;
  int PID = 0;
  int TimeStamp = 0;
  bool Busy = false;
  //Print Memory for memory table 
  void PrintMem() {
    cout << setw(5) << frame << setw(8) << page << setw(7) << PID << setw(7) << TimeStamp << endl;
  }



 private:
};

//Disk Class for I/O
class Disk {

  public:

    Disk() = default;

    bool isEmpty() { 
      return fcfs.empty();
    }

    processBlock getWorkingPID() { 
      return fcfs[0]; 
    }

    deque <processBlock> &getQueue() {
      return fcfs;
    }

  private:
    deque <processBlock> fcfs;
};

//Main OS Class
//Will handle most functions for our simulation 
class OS {
  public:

    //Constructor to initialize I/O disks and Memory Table
    OS(int NumDisks, int FrameNum) {
      DISKS.resize(NumDisks);
      MemTable.resize(FrameNum);
    }

    //Help Menu
    void help() {
      cout << "***************************** HELP MENU *****************************" << endl
      << "A - Create a new process with its own process block" << endl
      << "t - Terminate the process that currently is using the CPU" << endl
      << "d # file_name - The process that is using the CPU requests the hard disk number."<< endl
      << "                It wants to read or write file file_name." << endl
      << "D # - The hard disk # has finished the work for one process." << endl
      << "Q - Currently running process has spent a time quantum using the CPU" << endl
      << "m address - Process that is currently using the CPU" << endl
      << "            requests a memory operation for the logical address." << endl
      << "S r - Snapshot of the process that is in the CPU" << endl
      << "      and the processes that wait on each ready-queue level" << endl
      << "S i - Shows what processes are currently using the I/O devices" << endl
      << "      and the processes that are waiting to use them" << endl
      << "S m - Shows the state of Memory " << endl
      << "X - Exits the application immediately" << endl
      << "*********************************************************************" << endl
      << "Please enter one of the following commands:" << endl;
    }

    //Create a new process
    void newProcess(int pageSize, int id) {
      cout << "A new process is being created with ID = " << id << endl;
      processBlock newproc(pageSize,id);
      Queue[0].push_back(newproc);
    }

    //Return a specific process from a specific Queue
    processBlock getProcessFromQueue(int QueueNum, int ElementNum) {
      return Queue[QueueNum][ElementNum];
    }

    //Return the process in the CPU
    processBlock &getProcessFromCPU() {
      return CPU[0];
    }

    //Return the size of a given Level Queue
    int getQueueSize(int QueueNum) {
      return Queue[QueueNum].size();
    }
    
    //Return the size of the CPU
    int getCpuSize() {
      return CPU.size();
    }

    //Insert to CPU
    void InsertToCpu(processBlock proc) {
      CPU.push_back(proc);
    }

    //Return a given Queue
    deque <processBlock> &GetQueue(int QueueNum) {
      return Queue[QueueNum];
    }

    //Print what is in the CPU
    void PrintProcessInCpu() {
      CPU[0].printID();
    }

    //Increase the process in the CPU's time quantum by 1
    void TimeQuantum() {
      CPU[0].quantum_++;
    }

    //Return the CPU Queue
    deque <processBlock> &GetCPU() {
      return CPU;
    }

    //Print levels of all Multi level feedback queues 
    void PrintQueues() {
      for (int i = 0; i < 3; i++) {
        cout << "Level " << i << " Queue: " << endl;
        for(int j = 0; j < Queue[i].size(); j++) {
          getProcessFromQueue(i,j).printID();
        }
      }
    }


    //Returns how many I/O Queues there are
    int getDiskSize() {
      return DISKS.size();
    }

    //Returns the specific I/O Queue # passed
    Disk &GetDisk(int DiskNum) {
      return DISKS[DiskNum];
    }

    //Print I/O
    void PrintIO() {
      for (int i = 0; i < DISKS.size(); i++) {
        if (DISKS[i].getQueue().empty()) {
          cout << "DISK # " << i << " IS EMPTY" << endl;
        } else {
          cout << "DISK # " << i << " IS BEING USED BY PROCESS # " << DISKS[i].getQueue()[0].id_ <<
          " USING FILE " << DISKS[i].getQueue()[0].IOfilename << endl;
          if(DISKS[i].getQueue().size() > 1) {
            cout << "DISK # " << i << " HAS " << DISKS[i].getQueue().size() - 1 << " PROCESSES WAITING IN ITS QUEUE" << endl;
            cout << "THOSE PROCESSES ARE: " << endl;
            for (int j = 1; j < DISKS[i].getQueue().size(); j++) {
              DISKS[i].getQueue()[j].printIO();
            }
          }
        }
      }
    }

    //Returns how many frames there are in the table
    int getMemTableSize() {
      return MemTable.size();
    }

    //Initalize the frames on the memory table
    void InitializeMemTable() {
      for (int i = 0; i < MemTable.size(); i++) {
        MemTable[i].frame = i;
      }
    }

    //Print the memory table 
    void PrintMemTable() {
      cout << " Frame # " << " Page # " << " PID " << " TimeStamp " << endl;
      cout << "---------" << "--------" << "-----" << "-----------" << endl;
      for( int i = 0; i < MemTable.size(); i++) {
        MemTable[i].PrintMem();
      }
    }

    //Insert a new process to the memory table when its created 
    void InsertNewProcessToMemTable(processBlock proc, int &TS) {
      
          int i = FindEmptyIndex();
          MemTable[i].Busy = true;
          MemTable[i].page = 0;
          MemTable[i].PID = proc.id_;
          MemTable[i].TimeStamp = TS++;
    }

    //Returns the index of the entry to wipe and replace 
    int FindEmptyIndex() {
      for ( int i = 0; i < MemTable.size(); i++ ) {
        if (!MemTable[i].Busy) {
          //cout << " i am returning index " << i << " as empty" << endl;
          return i;
        }
      }

      // if we reach here then it means they were all busy and we need to eliminate the row with the
      // lowest time stamp by setting his Busy bool to false;

      //First, find the table entry with lowest Time Stamp
      int LRU = 5000;
      for ( int j = 0; j < MemTable.size(); j++) {
        if (MemTable[j].TimeStamp < LRU) {
          LRU = MemTable[j].TimeStamp;
        }
      }

      //Second, set his busy flag to false and return the index of the table entry we will be over-writing 
      for ( int u = 0; u < MemTable.size(); u++) {
        if (MemTable[u].TimeStamp == LRU) {
          MemTable[u].Busy = false;
          //cout << " i am returning index " << u << " as the LRU to swap with new process" << endl; 
          return u;
        }
      }
    }

    //Delete all entries in memory table given a PID
    void DeleteProcessFromMemTable(int id) {
      for ( int i = 0; i < MemTable.size(); i++) {
        if (MemTable[i].PID == id) {
          MemTable[i].PID = 0;
          MemTable[i].TimeStamp = 0;
          MemTable[i].page = 0;
          MemTable[i].Busy = false;
        }
      }
    }
    
    //This function will either create a new entry in the table
    //Or it will update the time stamp if the requested entry existed 
    void MemRequestFromProcessInCPU(int RequestedPageNum, int &TS) {
      int PIDfromCPU = CPU[0].id_;

      for(int i = 0; i < MemTable.size(); i++) {
        if (!(MemTable[i].PID == PIDfromCPU) && !(MemTable[i].page == RequestedPageNum)) {
          int t = FindEmptyIndex();
          MemTable[t].Busy = true;
          MemTable[t].page = RequestedPageNum;
          MemTable[t].PID = CPU[0].id_;
          MemTable[t].TimeStamp = TS++;
          return;
        } else if (MemTable[i].PID == PIDfromCPU && MemTable[i].page == RequestedPageNum) {
          MemTable[i].TimeStamp = TS++;
          return;
        }
      }

    }






  private:
    //Multilevel Feedback Queue
    deque <processBlock> Queue[3];
    //CPU
    deque <processBlock> CPU;
    //I/O Disks
    vector <Disk> DISKS;
    //Memory Table
    vector <MemManegment> MemTable;

};


