#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>


#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

//  read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//  Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string memline;
  string line;
  float memtotal , memfree , val , Lastval ;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
   while( std::getline(stream ,line)){
     std::istringstream linestream(line);
     while(linestream >> memline >> val){
     if(memline == "MemTotal:"){ memtotal = val ;} 
     if (memline == "MemFree:"){memfree = val ;}
   }
  }
   Lastval = (memtotal - memfree) / memtotal ;
   }
   return Lastval ;
}

//  Read and return the system uptime
long LinuxParser::UpTime() { 
  string line , upline;
  long up_val ;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream , line);
    std::istringstream linestream(line);
    linestream >> upline ;
    up_val = stol(upline);
    }
  return up_val;
   }

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return  LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); 
  }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  long utime , stime , cutime , cstime , Totaltime  ;
  string line ,value ;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if(stream.is_open()){
    std::getline(stream , line);
    std::istringstream streamline(line);
    for(int i=1 ; i<=18 ; i++){
      streamline >> value ;
      if(i== 15){ utime = stof(value) ;}
      if(i== 16){ stime = stof(value) ;}
      if(i== 17){ cutime = stof(value) ;}
      if(i== 18){ cstime = stof(value) ;}
    }
      Totaltime = utime+ stime + cutime + cstime ;
 }
 return Totaltime ;
}

//  Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  auto jifies = CpuUtilization();
  return stol(jifies[CPUStates::kUser_]) + stol(jifies[CPUStates::kNice_]) + stol(jifies[CPUStates::kSystem_])
  + stol(jifies[CPUStates::kIRQ_])+ stol(jifies[CPUStates::kSoftIRQ_])+ stol(jifies[CPUStates::kSteal_]) ;
   }

//  Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto jifies =CpuUtilization();
  return stol(jifies[CPUStates::kIOwait_]) + stol(jifies[CPUStates::kIdle_]); 
  }

//  Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuinfo ;
  string line , cpu , val ;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()){
    std::getline(stream ,line);
    std::istringstream linesteam(line);
    linesteam >> cpu ;
    while(linesteam >> val){
      cpuinfo.push_back(val);
    }
  }
  return cpuinfo ;
   }

//  Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int value , totalproc ;
  string line , totline;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open())
{
  std::istringstream linestream(line);
  while(linestream >> totline >> value){
    if(totline == "Processes"){ 
      totalproc = value;
      break;
    }
  }
}  
 return totalproc;
  }

//  Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string value , runningproc ;
  string line , runline;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open())
{
  std::istringstream linestream(line);
  while(linestream >> runline >> value){
    if(runline == "Procs_running"){ 
      runningproc = value;
      break;
    }
  }
}  
   int runproc = 0;
   if(runningproc != "") { runproc = stoi(runningproc) ;}
 return runproc;
 }

//  Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string command;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream , command);
  }
  return command; 
  }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string ram , line  , ramline;
  long ramval;
  std::ifstream stream(kProcDirectory+ std::to_string(pid)+kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream , line)){
      std::istringstream streamline(line);
      streamline >> ramline;
      if(ramline == "VmSize:"){
        streamline >> ramval ;
         break;
      }
    }
  }
   ramval/=1000 ;
   ram = to_string(ramval);
   return  ram ;
    }

//  Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string uid , line , uidline ;
  std::ifstream stream(kProcDirectory+ to_string(pid) +kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream ,line)){
      std::istringstream streamline(line);
      streamline >> uidline ;
      if(uidline == "Uid:"){
        streamline >> uid ;
        break;
      }
    }
  }
  return uid; 
  }

//  Read and return the user associated with a process

string LinuxParser::User(int pid) {
  string upid = Uid(pid);
  string line , uname , x, userpid , username;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream ,line)){
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream streamline(line) ;
    streamline >> uname >> x >> userpid ;
    if(userpid == upid){
      username = uname ;
      break ;
    }
  }
    }
    return username;
}

//  Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  long uptime , starttime ;
  string line ,value ;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if(stream.is_open()){
    std::getline(stream , line);
    std::istringstream streamline(line);
    for(int i=0 ; i<22 ; i++){
      if(i == 21){
        streamline >> value ;
        starttime = stol(value);
    }
  } 
  }
  uptime = LinuxParser::UpTime() - (starttime / sysconf(_SC_CLK_TCK));
   return uptime ;
}

// process's CPU utilization
float LinuxParser::process_cpuutilization(int pid) { 
  float utime , stime , cutime , cstime , Totaltime ;
  string line ,value ;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if(stream.is_open()){
    std::getline(stream , line);
    std::istringstream streamline(line);
    for(int i=1 ; i<=18 ; i++){
      streamline >> value ;
      if(i== 15){ utime = stof(value) ;}
      if(i== 16){ stime = stof(value) ;}
      if(i== 17){ cutime = stof(value) ;}
      if(i== 18){ cstime = stof(value) ;}
    }
     Totaltime = utime+ stime + cutime + cstime ;
     Totaltime = Totaltime /sysconf(_SC_CLK_TCK);
     float uptime = static_cast<long>( LinuxParser::UpTime(pid));
     Totaltime = Totaltime / uptime ;
  }
  return Totaltime;
}
