#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  
  Processor(); // constructor

 private:
  float prevTotalCpuTime;
  float prevIdleCpuTime;
};

#endif