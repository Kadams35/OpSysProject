#include <iostream>
#include <string>
#include <vector>
#include "Process.h"
#include "SJF.h"
#include <time.h>
#include <map>
#include <utility>
#include <algorithm>
#include "tgmath.h"

using namespace std;

SJF::SJF(vector<Process> passedProcessList, int passedContextSwitch, float mainLambda, float someAlpha){
    processList = passedProcessList;
    contextSwitch = passedContextSwitch;
    lambda = mainLambda;
    alpha = someAlpha;
    totalTime = 0.0;
    CPUTime = 0.0;
    contextSwitchTracker = 0;
}

void SJF::SJFAlgorithm(){
    int contextSwitchTime = 0;
    int burstTime = 0;
    map<char, int> IOBlock;
    int cpu = 0; //0 when CPU is free and 1 when CPU is being used
    vector<Process> tempList = processList;
    vector<Process> objectQueue;
    map<char, unsigned int> burstTracker; //Map keeping track of how many bursts have been completed
    map<char, int> blockList; //Map to keep track of the block a process is on
    pair<Process, char> currentCPU (processList[0], processList[0].get_id());
    vector<Process> finished; //Vector storing finished processes
    map<char, unsigned int> tauTracker; // storing the Tau values
    map<char, unsigned int> IOTracker;
    map<char, unsigned int> procWaitTime;


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

    cout << "time 0ms: Simulator started for SJF [Q empty]" << endl;
    while(1){
        if(cpu == 0){
            if(queueList.size() >=  1 && contextSwitchTime <= time){
                cpu = 1;
                contextSwitchTracker = 1;
                int burstInterval = burstTracker[queueList[0]];
                std::cout << "Current time: " << time << std::endl;
                cout << "ContextSwitch/2: " << contextSwitch/2 << endl;
                cout << "procWaitTime[queueList[0]]: "  << procWaitTime[queueList[0]] << endl;
                waitTimes.push_back(time - (contextSwitch/2) - procWaitTime[queueList[0]]);
                burstTime = (objectQueue[0].get_burst_list())[burstInterval];
                if(time < 1000){
                    cout << "time " << time << "ms: Process " << queueList[0] << " (tau " << tauTracker[objectQueue[0].get_id()] << "ms) started using the CPU for " << burstTime << "ms burst [Q ";
                    if (queueList.size() == 1){
                        cout << "empty]" << endl;
                    }
                    else{
                        for (unsigned int c = 1; c < queueList.size(); c++){
                            cout << queueList[c];
                        }
                        cout << "]" << endl;
                    }
                }
                burstTime = burstTime + time;
                currentCPU.first = objectQueue[0];
                currentCPU.second = queueList[0];
                burstTracker[queueList[0]] += 1;
                queueList.erase(queueList.begin());
                objectQueue.erase(objectQueue.begin());
            }
        }

        //Checking if a burst has finished
        if(time == burstTime && burstTime != 0){
            cpu = 0;
            burstTime = 0;
            int burstsLeft = currentCPU.first.get_burst_list().size() - burstTracker[currentCPU.second];
            contextSwitchTime += time;
            
            if(burstTracker[currentCPU.second] == currentCPU.first.get_burst_list().size()){
                cout << "time " << time << "ms: Process " << currentCPU.second << " terminated [Q ";
                if(queueList.size() == 0){
                    cout << "empty]" << endl;
                }
                else{
                    for(unsigned int i = 1; i < queueList.size(); i++){
                        cout << queueList[i];
                    }
                    cout << "]" << endl;
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
                    if(processList[i].get_id() == currentCPU.second){
                        finished.push_back(processList[i]);
                    }
                }
            }

            else{
                if(time < 1000){
                    cout << "time " << time <<"ms: Process " << currentCPU.second << " (tau " << tauTracker[currentCPU.second] << "ms) completed a CPU burst; " << burstsLeft << " bursts to go [Q ";
                    if(queueList.size() == 0){
                        cout << "empty]" << endl;
                    }
                    else{
                        for(unsigned int i = 0; i < queueList.size(); i++){
                            cout << queueList[i];
                        }
                        cout << "]" << endl;
                    }
                }
                if(queueList.size() > 0){
                    contextSwitchTracker++;
                }

                //Calculating Tau
                int tempTau = tauTracker[currentCPU.second];
                tauTracker[currentCPU.second] = ceil((1-alpha) * tauTracker[currentCPU.second] + alpha * currentCPU.first.get_burst_list()[burstTracker[currentCPU.second]-1]);
                if(time < 1000){
                    cout << "time " << time << "ms: Recalculated tau from " << tempTau << "ms to " << tauTracker[currentCPU.second] << "ms for process " << currentCPU.second << " [Q ";
                    if(queueList.size() == 0){
                        cout << "empty]" << endl;
                    }
                    else{
                        for(unsigned int i = 0; i < queueList.size(); i++){
                            cout << queueList[i];
                        }
                        cout << "]" << endl;
                    }
                }
                int IOInterval = blockList[currentCPU.second];
                int IOTime = currentCPU.first.get_io_list()[IOInterval];
                IOTime += time;
                IOTime += (contextSwitch/2);

                if(queueList.size() > 0){
                    contextSwitchTime = contextSwitch + time;
                }
                else{
                    contextSwitchTime = (contextSwitch/2) + time;
                }

                blockList[currentCPU.second] += 1;
                IOBlock[currentCPU.second] = IOTime;
                if(time < 1000){
                    cout << "time " << time << "ms: Process " << currentCPU.second << " switching out of CPU; will block on I/O until time " << IOTime << "ms [Q ";
                    if (queueList.size() == 0){
                        cout << "empty]" << endl;
                    }
                    else{
                        for (unsigned int i = 0; i < queueList.size(); i++){
                            cout << queueList[i];
                        }
                        cout << "]" << endl;
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
                        if(tauTracker[queueList[tempIter]] > tauTracker[tempList[i].get_id()]){
                            break;
                        }
                        else if(tauTracker[queueList[tempIter]] == tauTracker[tempList[i].get_id()]){
                            if(burstTracker[queueList[tempIter]] > burstTracker[tempList[i].get_id()]){
                                break;
                            }
                            else if(burstTracker[queueList[tempIter]] == burstTracker[tempList[i].get_id()]){
                                if(IOTracker[queueList[tempIter]] > IOTracker[tempList[i].get_id()]){
                                    break;
                                }
                                else if(IOTracker[queueList[tempIter]] == IOTracker[tempList[i].get_id()]){
                                    if(queueList[tempIter] > tempList[i].get_id()){
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    queueList.insert(queueList.begin()+tempIter, tempList[i].get_id());
                    objectQueue.insert(objectQueue.begin()+tempIter, tempList[i]);
                    procWaitTime[tempList[i].get_id()] = time;
                    if(time < 1000)
                        cout << "time " << time << "ms: Process " << tempList[i].get_id() << " (tau " << tauTracker[tempList[i].get_id()] << "ms) arrived; added to ready queue [Q ";
                    tempList.erase(tempList.begin()+i);
                    if(time < 1000){
                        for(unsigned int j = 0; j < queueList.size(); j++){
                            cout << queueList[j];
                        }
                        cout << "]" << endl;
                    }
                }
            }
        }

        for(unsigned int i = 0; i < processList.size(); i++){
            if(IOBlock[processList[i].get_id()] != 0 && IOBlock[processList[i].get_id()] == time){
                IOBlock[processList[i].get_id()] = 0; 
                //Figuring out where to add new process
                unsigned int tempIter = 0;
                for(tempIter = 0; tempIter < queueList.size(); tempIter++){
                    if(tauTracker[queueList[tempIter]] > tauTracker[processList[i].get_id()]){
                        //std::cout << "Tau for process " << processList[i].get_id() << " is less than tau for process " << queueList[tempIter] << std::endl;
                        break;
                    }
                    else if(tauTracker[queueList[tempIter]] == tauTracker[processList[i].get_id()]){
                        if(burstTracker[queueList[tempIter]] < burstTracker[processList[i].get_id()]){
                            //std::cout << "Bursts for process " << queueList[tempIter] << " (" << burstTracker[queueList[tempIter]] << ") is less than bursts for process " << queueList[tempIter] << " (" << burstTracker[processList[i].get_id()] << ")" << std::endl;
                            break;
                        }
                        else if(burstTracker[queueList[tempIter]] == burstTracker[processList[i].get_id()]){
                            if(IOTracker[queueList[tempIter]] < IOTracker[processList[i].get_id()]){
                                //std::cout << "IOBursts for process " << processList[i].get_id() << " is less than iobursts for process " << queueList[tempIter] << std::endl;
                                break;
                            }
                            else if(IOTracker[queueList[tempIter]] == IOTracker[processList[i].get_id()]){
                                if(queueList[tempIter] < processList[i].get_id()){
                                    //std::cout << "Process " << processList[i].get_id() << " comes before process " << queueList[tempIter] << std::endl;
                                    break;
                                }
                            }
                        }
                    }
                }
                queueList.insert(queueList.begin()+tempIter, processList[i].get_id());
                objectQueue.insert(objectQueue.begin()+tempIter, processList[i]);
                procWaitTime[processList[i].get_id()] = time;
                if(time < 1000){
                    cout << "time " << time << "ms: Process " << processList[i].get_id() << " (tau " << tauTracker[processList[i].get_id()] << "ms) completed I/O; added to ready queue [Q ";
                    for (unsigned int e = 0; e < queueList.size(); e++){
                        cout << queueList[e];
                    }
                    cout << "]" << endl;
                }
                IOTracker[processList[i].get_id()] += 1;
                contextSwitchTime = 0;
                contextSwitchTime = (contextSwitch/2) + time;
            }
        }

        if(finished.size() == processList.size() && contextSwitchTime <= time){
            cout << "time " << time << "ms: Simulator ended for SJF [Q empty]\n";
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

int SJF::getNumContextSwitches(){
    return contextSwitchTracker;
}

double SJF::getCPUUtilization(){
    //cout << "CPU time is " << CPUTime << " and totalTime is " << totalTime << endl;
    double output = round((CPUTime/totalTime)*100*1000)/1000;
    return output;
}

double SJF::getAvgWaitTime(){
    double sum = 0.0;
    for(unsigned int i = 0; i < waitTimes.size(); i++){
        std::cout << waitTimes[i] << std::endl;
        sum = sum + waitTimes[i];
    }
    return round((sum/waitTimes.size())*1000)/1000;
}

