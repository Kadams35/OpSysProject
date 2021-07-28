#include <iostream>
#include <string>
#include <vector>
#include "Process.h"
#include <time.h>
#include <map>

using namespace std;

class SRT{
    public:
        SRT(vector<Process> passedProcessList, int passedContextSwitch);
        void SRTAlgorithm();
    private:
        vector<Process> processList;
        int contextSwitch;
        int time; //Keeps track of how many milliseconds have passed in the simulation
        vector<char> queueList;
};

SRT::SRT(vector<Process> passedProcessList, int passedContextSwitch){
    processList = passedProcessList;
    contextSwitch = passedContextSwitch;
}

void SRT::SRTAlgorithm(){
    int contextSwitchTracker = 0; //Tracking if a context switch is currently happening
    int contextSwitchTime = 0;
    int burstTime = 0;
    map<char, int> IOBlock;
    int cpu = 0; //0 when CPU is free and 1 when CPU is being used
    vector<Process> tempList = processList;
    vector<Process> objectQueue;
    map<char, unsigned int> burstTracker; //Map keeping track of how many bursts have been completed
    map<char, int> blockList; //Map to keep track of the block a process is on
    Process currentCpu = Process();
    vector<Process> finished; //Vector storing finished processes


    //setting up the map
    for(unsigned int i = 0; i < processList.size(); i++){
        char name = processList[i].get_id();
        burstTracker[name] = 0;
        IOBlock[name] = 0;
        blockList[name] = 0;
    }

    while(1){
        if(cpu == 0){
            if(queueList.size() >=  1){
                cpu = 1;
                contextSwitchTracker = 1;
                int burstInterval = burstTracker[queueList[0]];
                burstTime = (objectQueue[0].get_burst_list())[burstInterval];
                burstTime = burstTime + time;
                currentCpu = objectQueue[0];
                burstTracker[queueList[0]] += 1;
                queueList.erase(queueList.begin());
            }
        }
    }

}