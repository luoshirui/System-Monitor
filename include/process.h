#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization() const;                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

  //  Declare any necessary private members
 private:
  int processId_;
  std::string user_;
  std::string command_;
  float cpuUsage_;
  std::string ram_;
  long uptime_;
  enum ProcessCPUStates{
      kUtime_ = 0,
      kStime_,
      kCutime_,
      kCstime_,
      kStarttime_
  };
  void calculateCpuUsage();
  void determineUser();
  void determineCommand();
  void determineRam();
  void determineUptime();
};

#endif