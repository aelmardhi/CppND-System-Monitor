#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

System::System() {
  kernel_ = LinuxParser::Kernel();
  os_ = LinuxParser::OperatingSystem();
  memoryTotal_ = LinuxParser::MemoryTotal();
}

void System::Refresh() {
  cpu_.Refresh();
  memoryUtilization_ = LinuxParser::MemoryUtilization();
  runningProcesses_ = LinuxParser::RunningProcesses();
  totalProcesses_ = LinuxParser::TotalProcesses();
  upTime_ = LinuxParser::UpTime();
  RefreshProcesses();
}

void System::RefreshProcesses() {
  vector<int> pids = LinuxParser::Pids();
  for (vector<Process>::iterator process = processes_.begin();
       process < processes_.end(); process++) {
    vector<int>::iterator pos = find(pids.begin(), pids.end(), process->Pid());
    if (pos == pids.end()) {
      processes_.erase(process);
    } else {
      pids.erase(pos);
    }
  }
  for (int pid : pids) {
    processes_.emplace_back(Process(pid));
  }
  for (Process& process : processes_) {
    process.Refresh();
  }
  sort(processes_.begin(), processes_.end());
}
// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }
float System::MemoryTotal() { return memoryTotal_ / 1024.0 / 1024.0; }
// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return memoryUtilization_; }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return os_; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return runningProcesses_; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return totalProcesses_; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return upTime_; }

void System::OrderingBy(int& c) { Process::OrderingBy(c); }