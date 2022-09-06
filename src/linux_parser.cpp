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

//  Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);

  string line, key, value;
  float memTotal, memFree;

  if (filestream.is_open()){
    while (std::getline(filestream,line)){
      std::remove(line.begin(), line.end(), ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key=="MemTotal") {memTotal=std::stof(value);}
        else if (key=="memFree"){
          memTotal=std::stof(value);
          break;}
      }
    }
  }
  return ((memTotal-memFree)/memTotal); }

// Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream filestream(kProcDirectory+kUptimeFilename);

  string line, wholeTime; 

  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      while (linestream>>wholeTime){
          try{ return std::stol(wholeTime);}
          catch(const std::invalid_argument & arg){return 0;}
      }
    }
 }
 return 0;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

//  Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  std::ifstream filestream(kProcDirectory+kStatFilename);
  vector<string> cpu{};
  string line, key;
  string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >> iowait 
                >> irq >>  softirq >> steal >> guest >> guest_nice){ 
            if (key == "cpu"){
              cpu.push_back(user);
              cpu.push_back(nice);
              cpu.push_back(system);
              cpu.push_back(idle);
              cpu.push_back(iowait);
              cpu.push_back(irq);
              cpu.push_back(softirq);
              cpu.push_back(steal);
              cpu.push_back(guest);
              cpu.push_back(guest_nice);
              return cpu;
            }
          }
       }
    }
  
  return {}; 
  }
vector<float> LinuxParser::CpuUtilization(int pid) { 
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  vector<float> cpu{};
  string line, value;
  float time = 0.0;
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      for (int i=1; i<=kStarttime_;i++){
        linestream >> value;
        if (i == kUtime_ || i == kStime_ || i == kCutime_ || i== kCstime_|| i== kStime_){
          time = std::stof(value) / sysconf(_SC_CLK_TCK);
          cpu.push_back(time);
        }
      }
    }
  }
  
  return cpu; 
  }

//  Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::ifstream filestream(kProcDirectory+kStatFilename);
  string line, key, value;
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "processes"){
          try {return std::stol(value);}
          catch (const std::invalid_argument & arg){return 0;}}
      }
    }
  }  
  return 0; 
}

//  Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream filestream(kProcDirectory+kStatFilename);
  string line, key, value;
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "procs_running"){
          try {return std::stol(value);}
          catch (const std::invalid_argument & arg){return 0;}}
      }
    }
  }  
  return 0; }

// Read and return the command associated with a process

string LinuxParser::Command(int pid) { 
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  string value = "";
  if (filestream.is_open()){
    std::getline(filestream, value);
    return value;
  }
  return value; }

//  Read and return the memory used by a process

string LinuxParser::Ram(int pid) { 
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  string line, key, value;
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::replace(line.begin(), line.end(), ':',' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "VmSize"){
          return value;}
      }
    }
  }  
  return value; }

// Read and return the user ID associated with a process

string LinuxParser::Uid(int pid) { 
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  string line, key, value;
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::replace(line.begin(), line.end(), ':',' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "Uid"){
          return value;}
      }
    }
  }    
  
  return string(); }

// Read and return the user associated with a process

string LinuxParser::User(int pid) { 
  std::ifstream filestream(kPasswordPath);

  string line; 
  string key;
  string value = "";
  string other;
  string uid = Uid(pid);

  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::replace(line.begin(),line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream>>value>>other>>key){
        if (key==uid){return value;}
      }
    }
  return value;
 }
}


// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  std::ifstream filestream(kProcDirectory+"/"+std::to_string(pid)+kStatFilename);

  string line; 
  string value;

  long uptime = 0;

  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      for (int i =1; i<=kStarttime_; i++){
        linestream >> value;
        if (i==kStarttime_){
          try{
            uptime = std::stol(value)/sysconf(_SC_CLK_TCK); // convert the clock ticks to seconds 
            return uptime;}
          catch(const std::invalid_argument & arg){return 0;}
        }
      }
    }
  return uptime;
  }
  return uptime; }
