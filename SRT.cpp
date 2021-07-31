#include <iostream>
#include <string>
#include <vector>
#include "Process.h"
#include "SRT.h"
#include <time.h>
#include <map>
#include <utility>
#include <algorithm>
#include "tgmath.h"


SRT::SRT(vector<Process> passedProcessList, int passedContextSwitch, float mainLambda, float someAlpha){
    processList = passedProcessList;
    contextSwitch = passedContextSwitch;
    lambda = mainLambda;
    alpha = someAlpha;
    totalTime = 0;
    CPUTime = 0;
    contextSwitchTracker = 0;
}


void SRT::SRTAlgorithm() {
    int contextSwitchTime = 0;
    int burstTime = 0;
    int cpu = 0; //0 when CPU is free and 1 when CPU is being used
    vector<Process> tempList = processList;
    vector<Process> finished; //Vector storing finished processes
    map<char, int> IOBlock;
    map<char, unsigned int> burstTracker; //Map keeping track of how many bursts have been completed
    map<char, int> blockList; //Map to keep track of the block a process is on
    map<char, unsigned int> tauTracker; // storing the Tau values
    map<char, unsigned int> IOTracker;
    map<char, unsigned int> procWaitTime;
    Process currentCPU = processList[0];


    //setting up the map
    for(unsigned int i = 0; i < processList.size(); i++){
        char name = processList[i].get_id();
        burstTracker[name] = 0;
        IOBlock[name] = 0;
        blockList[name] = 0;
        tauTracker[name] = 1/lambda;
        procWaitTime[name] = 0;
    }

    int time = 0;

    std::cout << "time 0ms: Simulator started for SRT [Q empty]" << std::endl;


    while (true) {
        if(cpu == 0){
            if(queueList.size() >=  1 && contextSwitchTime <= time){

                cpu = 1;
                contextSwitchTracker = 1;
                int burstInterval = burstTracker[queueList[0].get_id()];
                burstTime = (queueList[0].get_burst_list())[burstInterval];

                if(time < 1000){
                    std::cout << "time " << time << "ms: Process " << queueList[0].get_id() << " (tau " << tauTracker[queueList[0].get_id()] << "ms) started using the CPU for " << burstTime << "ms burst [Q ";
                    if (queueList.size() == 1){
                        std::cout << "empty]" << std::endl;
                    }
                    else{
                        for (unsigned int c = 1; c < queueList.size(); c++){
                            std::cout << queueList[c].get_id();
                        }
                        std::cout << "]" << std::endl;
                    }
                }

                burstTime = burstTime + time;
                currentCPU = queueList[0];
                burstTracker[queueList[0].get_id()] += 1;
                queueList.erase(queueList.begin());
            }
        }

        //Checking if a burst has finished
        if(time == burstTime && burstTime != 0){
            cpu = 0;
            burstTime = 0;
            int burstsLeft = currentCPU.get_burst_list().size() - burstTracker[currentCPU.get_id()];
            contextSwitchTime += time;
            
            if(burstTracker[currentCPU.get_id()] == currentCPU.get_burst_list().size()){
                std::cout << "time " << time << "ms: Process " << currentCPU.get_id() << " terminated [Q ";
                if(queueList.size() == 0){
                    std::cout << "empty]" << std::endl;
                }
                else{
                    for(unsigned int i = 1; i < queueList.size(); i++){
                        std::cout << queueList[i].get_id();
                    }
                    std::cout << "]" << std::endl;
                }
                if(queueList.size() > 0)
                    contextSwitchTracker += 1;
                contextSwitchTime = 0;
                if(queueList.size() > 0){
                    contextSwitchTime = contextSwitch + time;
                }
                else{
                    contextSwitchTime = (contextSwitch/2) + time;
                }

                for(unsigned int i = 0; i < processList.size(); i++){
                    if(processList[i].get_id() == currentCPU.get_id()){
                        finished.push_back(processList[i]);
                    }
                }
            }

            else{
                if(time < 1000){
                    std::cout << "time " << time <<"ms: Process " << currentCPU.get_id() << " (tau " << tauTracker[currentCPU.get_id()] << "ms) completed a CPU burst; " << burstsLeft << " bursts to go [Q ";
                    if(queueList.size() == 0){
                        std::cout << "empty]" << std::endl;
                    }
                    else{
                        for(unsigned int i = 0; i < queueList.size(); i++){
                            std::cout << queueList[i].get_id();
                        }
                        std::cout << "]" << std::endl;
                    }
                }
                if(queueList.size() > 0){
                    contextSwitchTracker++;
                }

                //Calculating Tau
                int tempTau = tauTracker[currentCPU.get_id()];
                tauTracker[currentCPU.get_id()] = ceil((1-alpha) * tauTracker[currentCPU.get_id()] + alpha * currentCPU.get_burst_list()[burstTracker[currentCPU.get_id()]-1]);
                if(time < 1000){
                    std::cout << "time " << time << "ms: Recalculated tau from " << tempTau << "ms to " << tauTracker[currentCPU.get_id()] << "ms for process " << currentCPU.get_id() << " [Q ";
                    if(queueList.size() == 0){
                        std::cout << "empty]" << std::endl;
                    }
                    else{
                        for(unsigned int i = 0; i < queueList.size(); i++){
                            std::cout << queueList[i].get_id();
                        }
                        cout << "]" << endl;
                    }
                }
                int IOInterval = blockList[currentCPU.get_id()];
                int IOTime = currentCPU.get_io_list()[IOInterval];
                IOTime += time;
                IOTime += (contextSwitch/2);

                if(queueList.size() > 0){
                    contextSwitchTime = contextSwitch + time;
                }
                else{
                    contextSwitchTime = (contextSwitch/2) + time;
                }

                blockList[currentCPU.get_id()] += 1;
                IOBlock[currentCPU.get_id()] = IOTime;
                if(time < 1000){
                    std::cout << "time " << time << "ms: Process " << currentCPU.get_id() << " switching out of CPU; will block on I/O until time " << IOTime << "ms [Q ";
                    if (queueList.size() == 0){
                        std::cout << "empty]" << std::endl;
                    }
                    else{
                        for (unsigned int i = 0; i < queueList.size(); i++){
                            std::cout << queueList[i].get_id();
                        }
                        std::cout << "]" << std::endl;
                    }
                }
            }
        }

        if(tempList.size() > 0){
            for(unsigned int i = 0; i < tempList.size(); i++){
                if(time == tempList[i].get_arrival_time()){
                    if(queueList.size() == 0){
                        contextSwitchTime = (contextSwitch/2) + time;
                    }

                    //Figuring out where to add new process
                    unsigned int tempIter = 0;
                    for(tempIter = 0; tempIter < queueList.size(); tempIter++){
                        if(tauTracker[queueList[tempIter].get_id()] > tauTracker[tempList[i].get_id()]){
                            break;
                        }
                        else if(tauTracker[queueList[tempIter].get_id()] == tauTracker[tempList[i].get_id()]){
                            if(burstTracker[queueList[tempIter].get_id()] > burstTracker[tempList[i].get_id()]){
                                break;
                            }
                            else if(burstTracker[queueList[tempIter].get_id()] == burstTracker[tempList[i].get_id()]){
                                if(IOTracker[queueList[tempIter].get_id()] > IOTracker[tempList[i].get_id()]){
                                    break;
                                }
                                else if(IOTracker[queueList[tempIter].get_id()] == IOTracker[tempList[i].get_id()]){
                                    if(queueList[tempIter].get_id() > tempList[i].get_id()){
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    queueList.insert(queueList.begin()+tempIter, tempList[i]);
                    procWaitTime[tempList[i].get_id()] = time;
                    if(time < 1000)
                        std::cout << "time " << time << "ms: Process " << tempList[i].get_id() << " (tau " << tauTracker[tempList[i].get_id()] << "ms) arrived; added to ready queue [Q ";
                    tempList.erase(tempList.begin()+i);

                    if(time < 1000){
                        for(unsigned int j = 0; j < queueList.size(); j++){
                            std::cout << queueList[j].get_id();
                        }
                        std::cout << "]" << std::endl;
                    }
                }
            }
        }

        for(unsigned int i = 0; i < processList.size(); i++){
            if(IOBlock[processList[i].get_id()] != 0 && IOBlock[processList[i].get_id()] == time){
                IOBlock[processList[i].get_id()] = 0;

                Process toqueue = processList[i];
                bool preempt = false;                
                if (tauTracker[processList[i].get_id()] < tauTracker[currentCPU.get_id()] && cpu == 1) {
                    preempt = true;
                    if (time < 1000) {
                        std::cout << "time " << time << "ms: Process " << processList[i].get_id() << " (tau " << tauTracker[processList[i].get_id()] << "ms) completed I/O; preempting " << currentCPU.get_id() << " [Q ";
                        for (unsigned int e = 0; e < queueList.size(); e++){
                            std::cout << queueList[e].get_id();
                        }
                        std::cout << "]" << std::endl;
                    }
                    toqueue = currentCPU;
                    currentCPU = processList[i];
                    contextSwitchTime = contextSwitch + time;


                }


                //Figuring out where to add new process
                unsigned int tempIter = 0;
                for(tempIter = 0; tempIter < queueList.size(); tempIter++){
                    if(tauTracker[queueList[tempIter].get_id()] > tauTracker[toqueue.get_id()]){
                        //std::cout << "Tau for process " << toqueue.get_id() << " is less than tau for process " << queueList[tempIter] << std::endl;
                        break;
                    }
                    else if(tauTracker[queueList[tempIter].get_id()] == tauTracker[toqueue.get_id()]){
                        if(burstTracker[queueList[tempIter].get_id()] < burstTracker[toqueue.get_id()]){
                            //std::cout << "Bursts for process " << queueList[tempIter] << " (" << burstTracker[queueList[tempIter]] << ") is less than bursts for process " << queueList[tempIter] << " (" << burstTracker[toqueue.get_id()] << ")" << std::endl;
                            break;
                        }
                        else if(burstTracker[queueList[tempIter].get_id()] == burstTracker[toqueue.get_id()]){
                            if(IOTracker[queueList[tempIter].get_id()] < IOTracker[toqueue.get_id()]){
                                //std::cout << "IOBursts for process " << toqueue.get_id() << " is less than iobursts for process " << queueList[tempIter] << std::endl;
                                break;
                            }
                            else if(IOTracker[queueList[tempIter].get_id()] == IOTracker[toqueue.get_id()]){
                                if(queueList[tempIter].get_id() < toqueue.get_id()){
                                    //std::cout << "Process " << toqueue.get_id() << " comes before process " << queueList[tempIter] << std::endl;
                                    break;
                                }
                            }
                        }
                    }
                }
                queueList.insert(queueList.begin()+tempIter, toqueue);
                if(time < 1000 && !preempt){
                    std::cout << "time " << time << "ms: Process " << processList[i].get_id() << " (tau " << tauTracker[processList[i].get_id()] << "ms) completed I/O; added to ready queue [Q ";
                    for (unsigned int e = 0; e < queueList.size(); e++){
                        std::cout << queueList[e].get_id();
                    }
                    std::cout << "]" << std::endl;
                }
                IOTracker[processList[i].get_id()] += 1;
                contextSwitchTime = 0;
                contextSwitchTime = (contextSwitch/2) + time;
            }
        }

        if(finished.size() == processList.size() && contextSwitchTime <= time){
            waitTimes = procWaitTime;
            cout << "time " << time << "ms: Simulator ended for SRT [Q empty]\n";
			cout << endl;
			break;
        }
        time++;
        totalTime++;
        if(cpu == 1){
            CPUTime++;
        }
    }

}

int SRT::getNumContextSwitches(){
    return contextSwitchTracker;
}

double SRT::getCPUUtilization(){
    //cout << "CPU time is RT" << CPUTime << " and totalTime is " << totalTime << endl;
    double output = round((CPUTime/totalTime)*100*1000)/1000;
    return output;
}

double SRT::getWaitTime(){
    double sum = 0.0;
    for( const auto &pair : waitTimes ){
        sum = sum + pair.second;
    }
    return round((sum/waitTimes.size())*1000)/1000;
}

