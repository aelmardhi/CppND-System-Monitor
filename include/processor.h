#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  void Refresh();
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long activejiffies_ {0};
  long totalJiffies_ {1};
  float utilization_{0.0};
};

#endif