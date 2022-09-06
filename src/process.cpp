#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : processId_(pid){
    calculateCpuUsage();
    determineCommand();
    determineRam();
    determineUptime();
    determineUser();
}
//  Return this process's ID
int Process::Pid() { return processId_; }

//  Return this process's CPU utilization
float Process::CpuUtilization() const { return cpuUsage_; }

//  Return the command that generated this process
string Process::Command() { return command_; }

//  Return this process's memory utilization
string Process::Ram() { return ram_; }

//  Return the user (name) that generated this process
string Process::User() { return user_; }

//  Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

void Process::calculateCpuUsage(){
    long uptime = LinuxParser::UpTime();
    vector<float> val = LinuxParser::CpuUtilization(Pid());
    if (val.size() == 5){
        float totaltime = val[kUtime_]+val[kStime_]+val[kCutime_]+val[kCstime_];
        float seconds = uptime -val[kStarttime_];
        cpuUsage_ = totaltime / seconds;
    }
    else {cpuUsage_ = 0;}
}

void Process::determineUser() {user_ = LinuxParser::User(Pid());}
void Process::determineCommand() {command_ = LinuxParser::Command(Pid());}
void Process::determineRam(){
    string value = LinuxParser::Ram(Pid());
    try{
        long conv = std::stol(value)/1000;
        ram_ = std::to_string(conv);
    }
    catch(const std::invalid_argument& arg) {
        ram_ = "0";
    }
}
void Process::determineUptime() {
    uptime_= LinuxParser::UpTime(Pid());
}