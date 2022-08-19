#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  user_ = LinuxParser::User(pid);
  cmd_ = LinuxParser::Command(pid);
}

Process::Process(int pid, string user, string cmd, long upTime, string ram,
                 float cpuUtilization)
    : pid_(pid),
      user_(user),
      cmd_(cmd),
      upTime_(upTime),
      ram_(ram),
      cpuUtilization_(cpuUtilization) {}

void Process::Refresh() {
  ram_ = LinuxParser::Ram(Pid());
  long tempUpTime = LinuxParser::UpTime(Pid());
  long tempActiveJiffies = LinuxParser::ActiveJiffies(Pid());
  if (tempUpTime == upTime_) return;
  cpuUtilization_ = (tempActiveJiffies - activeJiffies_) /
                    (sysconf(_SC_CLK_TCK) + 0.0) / (0.0 + tempUpTime - upTime_);
  activeJiffies_ = tempActiveJiffies;
  upTime_ = tempUpTime;
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUtilization_; }

// TODO: Return the command that generated this process
string Process::Command() { return cmd_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return upTime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return cpuUtilization_ < a.cpuUtilization_;
}