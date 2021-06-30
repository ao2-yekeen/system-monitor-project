#include <string>

#include "format.h"

using namespace std;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long int seconds) {
  long int minutes = 0;
  long int hours = 0;
  
  std::string output = to_string(hours) + ":" + to_string(minutes) + ":" + to_string(seconds);
  
  while (seconds >= 60){
    seconds -= 60;
    minutes +=1;
    output = to_string(hours) + ":" + to_string(minutes) + ":" + to_string(seconds);
  while(minutes >=60){
     minutes -=60;
     hours+=1;
     output = to_string(hours) + ":" + to_string(minutes) + ":" + to_string(seconds);
     }
  }
  
  return output;
}
