#include <dirent.h>

#include <unistd.h>

#include <sstream>

#include <string>

#include <vector>

using namespace std;

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
       replace(line.begin(), line.end(), ' ', '_');
       replace(line.begin(), line.end(), '=', ' ');
       replace(line.begin(), line.end(), '"', ' ');
       istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector < int > LinuxParser::Pids() {
  vector < int > pids;
  DIR * directory = opendir(kProcDirectory.c_str());
  struct dirent * file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file -> d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file -> d_name);
      if (all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float MemTotal;
  float MemFree;
  float Memory_utilize = 1;

  ifstream myfile("/proc/meminfo");
  if (myfile) {
    string line, os, kernel;
    while (getline(myfile, line)) {
      istringstream linestream(line);
      while (linestream >> os >> kernel) {
        if (os == "MemTotal:") {
          MemTotal = stof(kernel);
        }
        if (os == "MemFree:") {
          MemFree = stof(kernel);
     
          Memory_utilize = MemTotal + MemFree;
        }
      }
    }
    
  }
  return Memory_utilize;
}

    // TODO: Read and return the system uptime
long LinuxParser::UpTime() {
      long Uptime;

      ifstream myfile("/proc/uptime");
      if (myfile) {
        string line, os, kernel;
        while (getline(myfile, line)) {
          istringstream linestream(line);
          while (linestream >> os >> kernel) {
            Uptime = stol(os, nullptr, 10);
          }
        }
      }
      return Uptime;
    }

    // TODO: Read and return the number of jiffies for the system
    long LinuxParser::Jiffies() {
      long total = 0; //initalize my total 
      vector < string > cpuUtilization = LinuxParser::CpuUtilization(); //initialize cpuUtilization
      for (int i = kUser_; i <= kSteal_; i++) { //use a for loop 
        total += stol(cpuUtilization[i]);
      }

      return total; // return                                                                        
    }

    // TODO: Read and return the number of active jiffies for a PID
    // REMOVE: [[maybe_unused]] once you define the function
    long LinuxParser::ActiveJiffies(int pid) {
      long total = 0;
      string value, line; // initialize variables 
      ifstream stream(kProcDirectory + to_string(pid) + kStatFilename); // read files 
      if (stream.is_open()) { // check if file is open
        getline(stream, line); //read file line by line
        istringstream linestream(line);
        int i = 0; //initialize the variable
        while (linestream >> value) { //
          if (i >= 13 && i <= 16) { //
            total += stol(value); //totals 
          }
          i++; //increment i
        }
      }
      return total; //return total 
    }

    // TODO: Read and return the number of active jiffies for the system
    long LinuxParser::ActiveJiffies() {
      return LinuxParser::Jiffies() - LinuxParser::IdleJiffies(); //
    }

    // TODO: Read and return the number of idle jiffies for the system
    long LinuxParser::IdleJiffies() {
      long total = 0; //initialize variables
      vector < string > cpuUtilization = LinuxParser::CpuUtilization(); //initialize cpuUtilization
      for (int i = kIdle_; i <= kIOwait_; i++) {
        total += stol(cpuUtilization[i]); //convert string to long int and add it to total
      }
      return total; //return total 
    }

    // TODO: Read and return CPU utilization
    vector < string > LinuxParser::CpuUtilization() {
      string value, line;
      vector < string > cpu_Values; //initalize variables
      ifstream stream(kProcDirectory + kStatFilename);
      if (stream.is_open()) { //check if file is open
        getline(stream, line); //read file line by line
        istringstream linestream(line);
        while (linestream >> value) {
          if (value != "cpu") {
            cpu_Values.push_back(value); //
          }
        }
      }
      return cpu_Values;
    }

    // TODO: Read and return the total number of processes
    int LinuxParser::TotalProcesses() {
      string line;
      string key;
      string value;
      int totalProcesses = 0; //initalize variables
      ifstream stream(kProcDirectory + kStatFilename); //read file 
      if (stream.is_open()) { //check if file is open
        while (getline(stream, line)) {
          istringstream linestream(line); //initialize linestream
          while (linestream >> key) {
            if (key == "processes") {
              linestream >> totalProcesses; // get totalprocesses
              return totalProcesses;
            }
          }
        }
      }
      return totalProcesses;
    }

    // TODO: Read and return the number of running processes
    int LinuxParser::RunningProcesses() {
      string line;
      string key;
      string value;
      int runningProcesses = 0; //initialize variables
      ifstream stream(kProcDirectory + kStatFilename); //initalize stream
      if (stream.is_open()) { //check if stream is open 
        while (getline(stream, line)) {
          istringstream linestream(line);
          while (linestream >> key) {
            if (key == "procs_running") {
              linestream >> runningProcesses;
              return runningProcesses; //
            }
          }
        }
      }
      return runningProcesses;
    }

    // TODO: Read and return the command associated with a process
    string LinuxParser::Command(int pid) {
      ifstream myfile("/proc/" + to_string(pid) + "/cmdline");
      if (myfile) {
        string line, value;
        while (getline(myfile, line)) {
          istringstream linestream(line);
          while (linestream >> value) {
            return value;
          }
        }
      }
       return " ";
    }

        // TODO: Read and return the memory used by a process
        string LinuxParser::Ram(int pid) {
          ifstream myfile(kProcDirectory + to_string(pid) + "/status");
 
          if (myfile) {
            string line, os, kernel;
            while (getline(myfile, line)) {
              istringstream linestream(line);
              while (linestream >> os >> kernel) {

                if (os == "VmSize:") {
                  return kernel;
                }
              }
            }
          }
          return "";
        }

    // TODO: Read and return the user ID associated with a process
    string LinuxParser::Uid(int pid) {
        
     ifstream myfile(kProcDirectory + to_string(pid) + "/status");
 		if (myfile) {
            string line, os, kernel;
            while (getline(myfile, line)) {
              istringstream linestream(line);
              while (linestream >> os >> kernel) {

                if (os == "Uid:") {
                  return kernel;
                }
              }
            }
          }
          return "";
        }


// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
   string uid = LinuxParser::Uid(pid);
  string line, key;
  string username, dummy, user;
   ifstream filestream("/etc/passwd");
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
       replace(line.begin(), line.end(), ':', ' ');
        istringstream linestream(line);
      while (linestream >> username >> dummy >> user) {
        if (user == uid) {
          return username;
        }
      }
    }
  }
  return " ";  
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
 long starttime{0};
 long number;
 string line, word;
 vector<long> data;

  ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    getline(stream, line);
    replace(line.begin(), line.end(), '(', '_');
    replace(line.begin(), line.end(), ')', '_');
    replace(line.begin(), line.end(), '=', ' ');
    replace(line.begin(), line.end(), '"', ' ');
    istringstream linestream(line);
  
    for (int i=1; i<4; i++) { 
        linestream >> word;
        data.push_back(0);
    }
    for (int i=4; i<26; i++) { 
        linestream >> number;
        data.push_back(number);
    }
  
    starttime = data[22-1]; 
  }

  return starttime;
}
