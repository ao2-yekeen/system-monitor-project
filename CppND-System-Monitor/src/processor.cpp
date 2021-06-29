#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
     vector<long> CPU_utiliz_1 = LinuxParser::CpuUtilization(); //get the cpu utilization
    long Total_jiff_1 = LinuxParser::Jiffies(CPU_utiliz_1);  //get the total jiffies
    long Active_jiff_1 = LinuxParser::ActiveJiffies(CPU_utiliz_1); // get the active jiffies
 
    vector<long> CPU_utiliz_2 = LinuxParser::CpuUtilization();
    long Total_jiff_2 = LinuxParser::Jiffies(CPU_utiliz_2);
    long Active_jiff_2 = LinuxParser::ActiveJiffies(CPU_utiliz_2);
 
    float Delta_total = Total_jiff_2 - Total_jiff_1;   //calculate the difference between total jiffie 2 and total jiffie 1 
    float Delta_active = Active_jiff_2 - Active_jiff_1;//calculate the difference between active jiffie 2 and active jiffie 1 
    float cpu_util = Delta_active / Delta_total ;
 
    return cpu_util;
}