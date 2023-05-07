#include "processor.h"
#include"linux_parser.h"
  
// Return the aggregate CPU utilization
float Processor::Utilization() { 
    float cpu_utilization ;

    long cur_total = LinuxParser::Jiffies() , cur_idle = LinuxParser::IdleJiffies() ,
    cur_nonidle  = LinuxParser::ActiveJiffies();
    long prev_total = prev_Total() , prev_idle = prev_Idle() ;

    float Totaled = float(cur_total) - float(prev_total);
    float Idled = float(cur_idle) - float(prev_idle) ;

    // set the prev values
    prev_Total(cur_total) ;
    prev_Idle(cur_idle);
    prev_Nonidle(cur_nonidle);
    
    cpu_utilization = (Totaled - Idled) / Totaled ;
    return  cpu_utilization ;
 }


 //use accesor and mutator for previous values

long Processor::prev_Total() const{ return Total_ ;}
long Processor::prev_Idle()const{ return Idle_ ;}
long Processor::prev_Nonidle()const{ return NonIdle_ ;}

void Processor::prev_Total( long tot ){
 Total_ = tot ;
}
void Processor::prev_Idle( long idle ){
 Idle_ = idle ;
}
void Processor::prev_Nonidle( long nonidle ){
 NonIdle_ = nonidle;
}


