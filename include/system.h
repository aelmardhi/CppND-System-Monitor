#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  void Refresh();
  void RefreshProcesses();
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryTotal();
  float MemoryUtilization();      // TODO: See src/system.cpp
  long UpTime();                  // TODO: See src/system.cpp
  int TotalProcesses();           // TODO: See src/system.cpp
  int RunningProcesses();         // TODO: See src/system.cpp
  std::string Kernel();           // TODO: See src/system.cpp
  std::string OperatingSystem();  // TODO: See src/system.cpp
  void OrderingBy(int& c);

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::string kernel_;
  std::string os_;
  long memoryTotal_;
  float memoryUtilization_;
  int runningProcesses_;
  int totalProcesses_;
  long int upTime_;
};

#endif