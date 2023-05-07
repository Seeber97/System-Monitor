#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();     
 // mutator for private previous variable
  long prev_Total() const;
  long prev_Idle()const;
  long prev_Nonidle()const;
//accessor to get the prev values
  void prev_Total( long tot ) ;
  void prev_Idle( long idle ) ;
  void prev_Nonidle( long nonidle ) ;
  
 private:
  long Total_  , Idle_ , NonIdle_ ;
};

#endif