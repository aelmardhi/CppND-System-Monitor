#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long tempActiveJiffies = LinuxParser::ActiveJiffies();
    long tempTotalJiffies = LinuxParser::Jiffies();
    float utiliztion = (tempActiveJiffies - activejiffies_) / (tempTotalJiffies - totalJiffies_ + 0.0);
    activejiffies_ = tempActiveJiffies;
    totalJiffies_ = tempTotalJiffies;
    return utiliztion; 
     }