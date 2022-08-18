#include "processor.h"
#include "linux_parser.h"


void Processor::Refresh(){
    long tempActiveJiffies = LinuxParser::ActiveJiffies();
    long tempTotalJiffies = LinuxParser::Jiffies();
    utilization_ = (tempActiveJiffies - activejiffies_) / (tempTotalJiffies - totalJiffies_ + 0.1);
    activejiffies_ = tempActiveJiffies;
    totalJiffies_ = tempTotalJiffies;
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    return utilization_; 
}