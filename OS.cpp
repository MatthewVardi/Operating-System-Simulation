
/***************************************************************
Title: OS.cpp
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
#include "Components.h"
#include <deque>
#include <stdlib.h>
#include <cstdlib>
#include <iomanip>


using namespace std;


int main() {



  long ram; // This is in bytes
  int frameSize;
  int numDisks;
  int idNum = 0;
  int TimeStamp = 1;
  

  cout << "How much RAM memory is there on our simulated computer?" << endl;
  cin >> ram;
  cout << "What is the size of a page/frame?" << endl; // These start from 0
  cin >> frameSize;
  cout << "How many hard disks does our simulated computer have?" << endl; // There start from 0
  cin >> numDisks;

  int NumOfFrames = (ram/frameSize);
  
  string input;
  bool running = true;
  OS SIM(numDisks,NumOfFrames);
  SIM.InitializeMemTable();
  SIM.help();

  
  while (running) {
    

    if (SIM.getCpuSize() == 0) {
      if (SIM.getQueueSize(0) != 0) {
        SIM.InsertToCpu(SIM.GetQueue(0).front());
        SIM.GetQueue(0).pop_front();        
      } else if (SIM.getQueueSize(1) != 0) {
        SIM.InsertToCpu(SIM.GetQueue(1).front());
        SIM.GetQueue(1).pop_front();
      } else if (SIM.getQueueSize(2) != 0) {
        SIM.InsertToCpu(SIM.GetQueue(2).front());
        SIM.GetQueue(2).pop_front();
      } else {

      }
    } else if (SIM.getCpuSize() != 0) { // this condition is for pre-emption, it means something is in the CPU
       if (SIM.getProcessFromCPU().level_ == 0) {
        // do nothing because there is no pre-emption here
       } else if (SIM.getProcessFromCPU().level_ == 1 && SIM.getQueueSize(0) != 0) {
        //This case is when the process in the CPU is from level 1 and 
        //the level 0 queue is not empty --> PREEMPTION

        SIM.GetQueue(1).push_front(SIM.getProcessFromCPU());
        SIM.GetCPU().pop_front();
        SIM.InsertToCpu(SIM.GetQueue(0).front());
        SIM.GetQueue(0).pop_front();

       } else if (SIM.getProcessFromCPU().level_ == 2) {
        //This case is when the process in the CPU is from level 2 and 
        //now we check if level 0 is not empty, if its not then we take from there
        //otherwise check if level 1 is not empty, if its not then we take from there
        //We start with level 0 because it is of higher priority. 

        if (SIM.getQueueSize(0) != 0) {

          SIM.GetQueue(2).push_front(SIM.getProcessFromCPU());
          SIM.GetCPU().pop_front();
          SIM.InsertToCpu(SIM.GetQueue(0).front());
          SIM.GetQueue(0).pop_front();

        } else if (SIM.getQueueSize(1) != 0) {
          SIM.GetQueue(2).push_front(SIM.getProcessFromCPU());
          SIM.GetCPU().pop_front();
          SIM.InsertToCpu(SIM.GetQueue(1).front());
          SIM.GetQueue(1).pop_front();

        }
       }
    }

    getline(cin,input);
    
    if (input == "A") { // New Process Requested

        idNum++;
        SIM.newProcess(frameSize,idNum);
        processBlock temp(frameSize,idNum);
        SIM.InsertNewProcessToMemTable(temp,TimeStamp);

    } else if (input == "X") { // Terminate Program

        running = false;

    } else if (input[0] == 'd') {  // Disk Request from process in CPU

      cout << "IO Request" << endl;

      if(input[1] == ' ') {
        
        string delimiter = "' '";
        string HDN = input.substr(2,input.find(delimiter));
        string tempstring = input.substr(2,input.size());
        int HardDiskNum = atoi(HDN.c_str());
        if (HardDiskNum < numDisks) {
          cout  << "HardDiskNum:" << HardDiskNum << endl;

          string File_Name = tempstring.substr(2,tempstring.size()); 
          if(File_Name[0] == ' ') {
            File_Name = File_Name.substr(1,File_Name.size());
          }
          cout << "File name is:" << File_Name << endl;
        

          SIM.getProcessFromCPU().IOfilename = File_Name;
          SIM.GetDisk(HardDiskNum).getQueue().push_back(SIM.getProcessFromCPU()); // put in IO queue first then take out of CPU 
          SIM.GetCPU().pop_front();

          cout << "currently using disk # " << HardDiskNum << " is process: " << SIM.GetDisk(HardDiskNum).getWorkingPID().id_ << endl;
        } else {
          cout << "NOT A VALID DISK NUMBER REQUEST" << endl;
        }
        
        
      } else {
        cout << "IMPROPER USE OF 'd' COMMAND" << endl;
      }
    } else if (input[0] == 'S' && input [2] == 'r') { // Print CPU and all Multi level queues 

      cout << "You Selected 'S r' " << endl;
      cout << "Current in the CPU:" << endl;
      if (!SIM.GetCPU().empty()) {
        
        SIM.getProcessFromCPU().printID();
        cout << endl;

      } else {
        cout << "CPU IS EMPTY" << endl;
      }
      cout << "Printing all processes" << endl;
      SIM.PrintQueues(); 

    } else if (input[0] == 'S' && input [2] == 'i') { // Print I/O Info

        cout  << "Printing Hard Disk Information: " << endl;
        SIM.PrintIO();

    } else if (input[0] == 'S' && input [2] == 'm') { // Print Memory Table

        cout << "Printing Memory Table" << endl;
        cout << endl;
        SIM.PrintMemTable();

    } else if (input[0] == 'Q') { // +1 to time slice for Process in CPU 

        cout << "You Selected 'Q' " << endl;
        cout << "Adding One Time Quantum to Process in CPU" << endl;
        SIM.TimeQuantum();
        
        if (SIM.getProcessFromCPU().level_ == 0 && SIM.getProcessFromCPU().quantum_ == 1 ) {

          SIM.getProcessFromCPU().level_ = 1; // set the processes level to level 1
          SIM.getProcessFromCPU().quantum_ = 0; // reset his quantum 
          SIM.GetQueue(1).push_back(SIM.getProcessFromCPU());
          SIM.GetCPU().pop_front();

        } else if (SIM.getProcessFromCPU().level_ == 1 && SIM.getProcessFromCPU().quantum_ == 2 ) {

          SIM.getProcessFromCPU().level_ = 2; // set the processes level to level 2
          SIM.getProcessFromCPU().quantum_ = 0; // reset his quantum 
          SIM.GetQueue(2).push_back(SIM.getProcessFromCPU());
          SIM.GetCPU().pop_front();

        } else {
        // Level 2 process should stay in CPU forever until termination or I/O
      }
    } else if (input[0] == 't') { // Terminate process in CPU

      cout << "Terminating current process in the CPU" << endl;
      int tempPIDvar = SIM.GetCPU()[0].id_;
      SIM.DeleteProcessFromMemTable(tempPIDvar);

      SIM.GetCPU().clear();

    } else if (input[0] == 'D') { // Eject process from Disk number

      string DN = input.substr(2,input.size()); // grab the disk number
      int newDN = atoi(DN.c_str());
      if (newDN < numDisks && newDN >= 0 ) {
        if (!SIM.GetDisk(newDN).getQueue().empty()) {
        
        int LevelToGoTo = SIM.GetDisk(newDN).getQueue()[0].level_; // grab the level of the process in I/O
        int ID = SIM.GetDisk(newDN).getQueue()[0].id_;
        //cout << LevelToGoTo << endl;

        //this line under here is what i added to reset quantum when finished disk work
        SIM.GetDisk(newDN).getQueue()[0].quantum_ = 0;
        SIM.GetQueue(LevelToGoTo).push_back(SIM.GetDisk(newDN).getQueue()[0]);
        SIM.GetDisk(newDN).getQueue().pop_front();

        cout << "Finished I/O Work on Disk # " << newDN << " and sending process " << ID
        << " back to the level " << LevelToGoTo << " Queue" << endl;  
        } else {
          cout << "Disk # " << newDN << " Does not have a process to eject" << endl;
        }
      } else {
        cout << "Disk Number Requested is Invalid" << endl;
      }

    } else if (input[0] == 'm') { // Memory Request from process in CPU
      if (input[1] == ' ') {
        string PA = input.substr(2,input.size()-2);
        int PageAddress =  atoi(PA.c_str());
        cout << "Page address requested is " << PageAddress << endl;
        if (PageAddress < ram) {
          int PageNum = (PageAddress/frameSize);
          cout << "Page Number is " << PageNum << endl;


          SIM.MemRequestFromProcessInCPU(PageNum,TimeStamp);
        } else {
          cout << "Not a valid Page Address to request" << endl;
        }


      } else { // Not a valid command 
        cout << "Invalid Input" << endl;
      }
    } else {
      // Do Nothing and run the loop again
    }

  }

  return 0;
}
