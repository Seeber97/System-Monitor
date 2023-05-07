#include <string>

#include "format.h"


using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::HMS(long time){
    string Time ;
    if(time < 10)
     Time = "0" + std::to_string(time); 
    else
     Time = std::to_string(time);

return Time;
    }

string Format::ElapsedTime(long seconds) { 
    long hour{0} , minute{0} , second ;
    string Hour , Minute , Second ,finalformat ;
    second = (seconds %3600) % 60 ;
    minute = (seconds %3600) / 60 ;
    hour = seconds / 60;

    Minute = HMS(minute);
    Hour = HMS(hour);
    Second = HMS(second);

    finalformat = Hour + ":" + Minute + ":" + Second ;
    return finalformat;
     }

