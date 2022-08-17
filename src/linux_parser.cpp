#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float memTotal {1.0},memAvailable{0.0}, memFree {1.0}, Cached {0.0}, Buffers{0.0}, SReclaimable{0}, Shmem{0.0};
  string line;
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
  if(stream.is_open()){
    while( getline(stream,line)){
      std::istringstream lineStream(line);
      string key, value;
      while(lineStream >> key >> value){
        if(key == "MemTotal:"){
          memTotal =  stof(value);
        }else if(key == "MemAvailable:"){
          memAvailable =  stof(value);
        }else if(key == "MemFree:"){
          memFree =  stof(value);
        }else if(key == "Buffers:"){
          Buffers =  stof(value);
        }else if(key == "Cached:"){
          Cached =  stof(value);
        }else if(key == "SReclaimable:"){
          SReclaimable =  stof(value);
        }else if(key == "Shmem:"){
          Shmem =  stof(value);
        }
      }
    }
  }
  if(!memAvailable)
    memAvailable = memFree + Buffers + Cached +   SReclaimable - Shmem;
  return (memTotal - memAvailable) / memTotal;
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long upTime;
  string line;
  std::ifstream fileStream(kProcDirectory+kUptimeFilename);
  if(fileStream.is_open()){
    if (getline(fileStream, line))
    {
      std::istringstream lineStream (line);
      string value;
      lineStream >> value;
      upTime = std::stol(value);
    } 
  }
  return upTime;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<long> jiffies {};
  string line;
  std::ifstream fileStream(kProcDirectory+kStatusFilename);
  if(fileStream.is_open()){
    while (getline(fileStream,line))
    {
      string temp;
      std::istringstream lineStream(line);
      if(lineStream >> temp && temp == "cpu"){
        while(lineStream >> temp){
          jiffies.emplace_back(std::stol(temp));
        }
      }
    }
    
  }
  if(jiffies.size() >= 10){
    return jiffies[0] +jiffies[1] +jiffies[2] +jiffies[5] +jiffies[6] +jiffies[7] ; //user + nice + system + irq + softirq + steal
  }
  return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalProcesses{0};
  string line;
  std::ifstream fileStream(kProcDirectory+kStatFilename);
  if(fileStream.is_open()){
    while(getline(fileStream, line)){
      std::istringstream lineStream(line);
      string key, value;
      while (lineStream >> key>> value && key=="processes")
      {
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
  std::ifstream fileStream(kProcDirectory+kStatFilename);
  if(fileStream.is_open()){
    while(getline(fileStream, line)){
      std::istringstream lineStream(line);
      string key, value;
      while (lineStream >> key>> value && key=="procs_running")
      {
        runningProcesses = std::stoi(value);
      }
      
    }
  }
  return runningProcesses; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
