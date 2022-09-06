#include "processor.h"
#include <string>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::vector;

Processor::Processor(): prevTotalCpuTime(0.0),prevIdleCpuTime(0.0){};

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<long> cpuValue{};
    vector<string> cpuString = LinuxParser::CpuUtilization();
    for (int it = 0; it < (int)cpuString.size(); it++){
        try {cpuValue.push_back(std::stol(cpuString[it]));}
        catch (const std::invalid_argument& arg){cpuValue.push_back((long)0);}
    }

    float totalCPUTime = cpuValue[LinuxParser::kUser_] +  cpuValue[LinuxParser::kNice_] +
                         cpuValue[LinuxParser::kSystem_] + cpuValue[LinuxParser::kIdle_] +
                         cpuValue[LinuxParser::kIOwait_] + cpuValue[LinuxParser::kIRQ_] +
                         cpuValue[LinuxParser::kSoftIRQ_] + cpuValue[LinuxParser::kSteal_];
    
    float idleCpuTime = cpuValue[LinuxParser::kIdle_] + cpuValue[LinuxParser::kIOwait_];
    float diffIdle = idleCpuTime - prevIdleCpuTime;
    float diffTotal = totalCPUTime - prevTotalCpuTime;
    float diffUsage = (diffTotal - diffIdle)/diffTotal;
    prevIdleCpuTime = idleCpuTime;
    prevTotalCpuTime = totalCPUTime;
    return diffUsage;
 }