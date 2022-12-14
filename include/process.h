#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  enum class OrderingMethod { ByRAMAsc, ByRAMDes, ByCPUAsc, ByCPYDes };
  Process(int pid);
  Process(int pid, std::string user, std::string cmd, long uptime,
          std::string ram_, float cpuUtilization);
  void Refresh();
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  static OrderingMethod orderBy;
  static void OrderingBy(int& c);

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string cmd_;
  long upTime_;
  std::string ram_;
  float cpuUtilization_;
  long activeJiffies_;
};

#endif