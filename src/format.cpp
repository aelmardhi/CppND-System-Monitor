#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    string elapsedTime;
    elapsedTime = ":" + 
            ((seconds%60<10)? "0"+(std::to_string( seconds%60) ):
            (std::to_string( seconds%60)))
            + elapsedTime;
    seconds /= 60;
    elapsedTime = ":" + 
            ((seconds%60<10)? "0"+(std::to_string( seconds%60) ):
            (std::to_string( seconds%60)))
            + elapsedTime;
    seconds /= 60;
    elapsedTime = std::to_string( seconds) + elapsedTime;
    
    return elapsedTime;
}