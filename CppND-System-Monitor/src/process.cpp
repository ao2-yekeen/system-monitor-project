#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(const int pidint) : pid_(pidint){};
// TODO: Return this process's ID
int Process::Pid() { 
  return pid_;;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  auto result =
      ((float)LinuxParser::ActiveJiffies(pid_) / LinuxParser::ClkTPS());

  cpu_utilization_ = result / UpTime();
  return this->cpu_utilization_;
}

// TODO: Return the command that generated this process
string Process::Command() {
  return LinuxParser::Command(pid_);;
}

// TODO: Return this process's memory utilization
string Process::Ram() { LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  return (cpu_utilization_ > a.cpu_utilization_);
}