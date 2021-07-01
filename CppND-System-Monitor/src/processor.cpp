#include "processor.h"
#include "linux_parser.h"
#include <vector>
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
 return static_cast<float>(LinuxParser::Jiffies()) / static_cast<float>(LinuxParser::ActiveJiffies())/100;
}