#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long hours, minutes, sec;
    string time;

    hours = seconds/3600;
    if (hours<10){
        time = "0" + std::to_string(hours) + ":";
    }
    else{
        time =  std::to_string(hours) + ":";
    }

    minutes = (seconds/60)%60;
    if (minutes<10) {
        time += "0";
    }
    time += std::to_string(minutes) + ":";

    sec = seconds%60;
    if (sec<10) {
    time += "0";
    }
    time += std::to_string(sec) + ":";
    
    return time; 
}