#include "linux_parser.h"

#include <unistd.h>

#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  for (auto &file : std::filesystem::directory_iterator(kProcDirectory)) {
    // Is this a directory?
    if (file.is_directory()) {
      // Is every character of the name a digit?
      string filename(file.path().stem().string());
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}
long LinuxParser::MemoryTotal() {
  long memTotal{0};
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream lineStream(line);
      string key, value;
      while (lineStream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = std::stol(value);
        }
      }
    }
  }
  return memTotal;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal{1.0}, memAvailable{0.0}, memFree{1.0}, Cached{0.0},
      Buffers{0.0}, SReclaimable{0}, Shmem{0.0};
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream lineStream(line);
      string key, value;
      while (lineStream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = stof(value);
        } else if (key == "MemAvailable:") {
          memAvailable = stof(value);
        } else if (key == "MemFree:") {
          memFree = stof(value);
        } else if (key == "Buffers:") {
          Buffers = stof(value);
        } else if (key == "Cached:") {
          Cached = stof(value);
        } else if (key == "SReclaimable:") {
          SReclaimable = stof(value);
        } else if (key == "Shmem:") {
          Shmem = stof(value);
        }
      }
    }
  }
  if (!memAvailable)
    memAvailable = memFree + Buffers + Cached + SReclaimable - Shmem;
  return (memTotal - memAvailable) / memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long upTime;
  string line;
  std::ifstream fileStream(kProcDirectory + kUptimeFilename);
  if (fileStream.is_open()) {
    if (getline(fileStream, line)) {
      std::istringstream lineStream(line);
      string value;
      lineStream >> value;
      upTime = std::stol(value);
    }
  }
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  long active{0};
  string line;
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatFilename);
  if (fileStream.is_open()) {
    if (getline(fileStream, line)) {
      std::istringstream lineStream(line);
      string value;
      for (int i = 1; i <= 13; i++) {
        lineStream >> value;
      }
      for (int i = 14; i <= 17; i++) {  // utime + stime +  cutime + cstime
        lineStream >> value;
        active += std::stol(value);
      }
      // active /= sysconf(_SC_CLK_TCK);
    }
  }
  return active;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long active{0};
  vector<string> jiffies = CpuUtilization();
  if (jiffies.size() >= 7) {
    active += std::stol(jiffies[CPUStates::kUser_]);
    active += std::stol(jiffies[CPUStates::kNice_]);
    active += std::stol(jiffies[CPUStates::kSystem_]);
    active += std::stol(jiffies[CPUStates::kIRQ_]);
    active += std::stol(jiffies[CPUStates::kSoftIRQ_]);
    active += std::stol(jiffies[CPUStates::kSteal_]);
    // user + nice + system + irq + softirq + steal
  }
  return active;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();
  long idle{0};
  if (jiffies.size() >= 4) {
    idle += std::stol(jiffies[CPUStates::kIdle_]);
    idle += std::stol(jiffies[CPUStates::kIOwait_]);  // idle + iowait
  }
  return idle;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> jiffies{};
  string line;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    while (getline(fileStream, line)) {
      string temp;
      std::istringstream lineStream(line);
      if (lineStream >> temp && temp == "cpu") {
        while (lineStream >> temp) {
          jiffies.emplace_back(temp);
        }
      }
    }
  }
  return jiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalProcesses{0};
  string line;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    while (getline(fileStream, line)) {
      std::istringstream lineStream(line);
      string key, value;
      while (lineStream >> key >> value && key == "processes") {
        totalProcesses = std::stoi(value);
      }
    }
  }
  return totalProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int runningProcesses{0};
  string line;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    while (getline(fileStream, line)) {
      std::istringstream lineStream(line);
      string key, value;
      while (lineStream >> key >> value && key == "procs_running") {
        runningProcesses = std::stoi(value);
      }
    }
  }
  return runningProcesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string cmd;
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (fileStream.is_open()) {
    getline(fileStream, cmd);
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  long ram;
  string line;
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (fileStream.is_open()) {
    while (getline(fileStream, line)) {
      std::istringstream lineStream(line);
      string key, value;
      if (lineStream >> key >> value && key == "VmSize:") {
        ram = stol(value);
        ram /= 1024;
      }
    }
  }
  return to_string(ram);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string uid;
  string line;
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (fileStream.is_open()) {
    while (getline(fileStream, line)) {
      std::istringstream lineStream(line);
      string key, value;
      if (lineStream >> key >> value && key == "Uid:") {
        uid = value;
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string user;
  string line;
  std::ifstream fileStream(kPasswordPath);
  if (fileStream.is_open()) {
    while (getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      string key, x, value;
      if (lineStream >> value >> x >> key && key == uid) {
        user = value;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long upTime;
  string line;
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatFilename);
  if (fileStream.is_open()) {
    if (getline(fileStream, line)) {
      std::istringstream lineStream(line);
      string value;
      for (int i = 0; i < 22; i++) {
        lineStream >> value;
      }
      upTime = std::stol(value);
      upTime /= sysconf(_SC_CLK_TCK);
    }
  }
  return UpTime() - upTime;
}
