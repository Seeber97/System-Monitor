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
    long hour{0} , minute{0}  ,second_over_hour ;
    string Hour , Minute , Second ,finalformat ;
    hour = seconds / 3600;
    second_over_hour = seconds - (hour*3600);
    minute = second_over_hour / 60 ;
    seconds = second_over_hour % 60;

    Minute = HMS(minute);
    Hour = HMS(hour);
    Second = HMS(seconds);

    finalformat = Hour + ":" + Minute + ":" + Second ;
    return finalformat;
     }

