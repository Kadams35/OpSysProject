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

using namespace std;

bool sorter(Process one, Process two){
    int smallSize = 0;
    if(one.get_burst_list().size() > two.get_burst_list().size()){
        smallSize = two.get_burst_list().size();
    }
    else{
        smallSize = one.get_burst_list().size();
    }
    for(int i = 0; i < smallSize; i++){
        return one.get_burst_list()[i] < two.get_burst_list()[i];
    }
    return one.get_burst_list()[0] < two.get_burst_list()[0];
}

SRT::SRT(vector<Process> passedProcessList, int passedContextSwitch, int mainLambda){
    processList = passedProcessList;
    std::sort(processList.begin(), processList.end(), sorter);
    contextSwitch = passedContextSwitch;
    lambda = mainLambda;
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
    pair<Process, char> currentCPU (processList[0], processList[0].get_id());
    vector<Process> finished; //Vector storing finished processes
    map<char, unsigned int> tauTracker; // storing the Tau values


    //setting up the map
    for(unsigned int i = 0; i < processList.size(); i++){
        char name = processList[i].get_id();
        burstTracker[name] = 0;
        IOBlock[name] = 0;
        blockList[name] = 0;
        tauTracker[name] = 1/lambda;
    }

    int time = 0;

    cout << "time 0ms: Simulator started for SRT [Q empty]" << endl;
    while(1){
        if(cpu == 0){
            if(queueList.size() >=  1){
                cpu = 1;
                contextSwitchTracker = 1;
                int burstInterval = burstTracker[queueList[0]];
                burstTime = (objectQueue[0].get_burst_list())[burstInterval];
                cout << "time " << time << "ms: Process " << queueList[1] << "(tau " << tauTracker[objectQueue[0].get_id()] << "ms) started using the CPU for " << burstTime << "ms burst [Q ";
                if (queueList.size() == 1){
                    cout << "empty]" << endl;
                }
                else{
                    for (unsigned int c = 0; c < queueList.size(); c++){
                        cout << queueList[c];
                        if (c != (queueList.size()-1)){
                            cout << " ";
                        }
                    }
                    cout << "]" << endl;
                }
                burstTime = burstTime + time;
                currentCPU.first = objectQueue[0];
                currentCPU.second = objectQueue[0].get_id();
                burstTracker[queueList[0]] += 1;
                queueList.erase(queueList.begin());
            }
        }

        if(tempList.size() > 0){
            for(unsigned int i = 0; i < tempList.size(); i++){
                if(time == tempList[i].get_arrival_time()){
                    queueList.push_back(tempList[i].get_id());
                    objectQueue.push_back(tempList[i]);
                    tempList.erase(tempList.begin()+i);

                    cout << "time " << time << "ms: Process " << tempList[i].get_id() << " (tau " << tauTracker[tempList[i].get_id()] << "ms) arrived; added to ready queue [Q ";
                    for(unsigned int j = 0; j < queueList.size(); j++){
                        cout << queueList[j];
                        if(j != (queueList.size()-1)){
                            cout << " ";
                        }
                        cout << endl;
                    }
                }
            }
        }

        if(time == burstTime && burstTime != 0){
            cpu = 0;
            burstTime = 0;
            int burstsLeft = currentCPU.first.get_burst_list().size() - burstTracker[currentCPU.second];
            contextSwitchTime += time;
            
            if(burstTracker[currentCPU.second] == currentCPU.first.get_burst_list().size()){
                cout << "time " << time << "ms: Process " << currentCPU.second << " terminated [Q ";
                if(queueList.size() == 1){
                    cout << "empty]" << endl;
                }
                else{
                    for(unsigned int i = 1; i < queueList.size(); i++){
                        cout << queueList[i];
                        if(i != (queueList.size()-1)){
                            cout << " ";
                        }
                    }
                    cout << endl;
                    contextSwitchTracker += 1;
                }

                for(unsigned int i = 0; i < processList.size(); i++){
                    if(processList[i].get_id() == currentCPU.second){
                        finished.push_back(processList[i]);
                    }
                }
            }

            else{
                cout << "time " << time <<"ms: Process " << currentCPU.second << " (tau " << tauTracker[currentCPU.second] << "ms) completed a CPU burst; " << burstsLeft << " bursts to go [Q ";
                if(queueList.size() == 1){
                    cout << "empty]" << endl;
                }
                else{
                    for(unsigned int i = 0; i < queueList.size(); i++){
                        cout << queueList[i];
						if (i != (queueList.size()-1)){
							cout << " ";
						}
                    }
                    cout << endl;
                    contextSwitchTracker++;
                }

                //Calculating Tau
                int tempTau = tauTracker[currentCPU.second];
                tauTracker[currentCPU.second] = ceil(0.5 * tauTracker[currentCPU.second] + 0.5 * currentCPU.first.get_burst_list()[burstTracker[currentCPU.second]-1]);
                cout << "time " << time << "ms: Recalculated tau from " << tempTau << "ms to " << tauTracker[currentCPU.second] << "ms for process " << currentCPU.second << " [Q ";
                if(queueList.size() == 0){
                    cout << "empty]" << endl;
                }
                else{
                    for(unsigned int i = 0; i < queueList.size(); i++){
                        cout << queueList[i];
						if (i != (queueList.size()-1)){
							cout << " ";
						}
                    }
                    cout << endl;
                }
                
                int IOInterval = blockList[currentCPU.second];
                int IOTime = currentCPU.first.get_io_list()[IOInterval];
                IOTime += time;

                blockList[currentCPU.second] += 1;
                IOBlock[currentCPU.second] = IOTime;

                cout << "time " << time << "ms: Process " << currentCPU.second << " switching out of CPU; will block on I/O until time " << IOTime << "ms [Q ";
                if (queueList.size() == 0){
                    cout << "empty]" << endl;
                }
                else{
                    for (unsigned int i = 1; i < queueList.size(); i++){
                        cout << queueList[i];
                        if (i != (queueList.size()-1)){
                            cout << " ";
                        }
                    }
                    cout << endl;
                }
            }
        }

        for(unsigned int i = 0; i < processList.size(); i++){
            if(IOBlock[processList[i].get_id()] != 0 && IOBlock[processList[i].get_id()] == time){
                IOBlock[processList[i].get_id()] = 0; 
                queueList.push_back(tempList[i].get_id());
                objectQueue.push_back(tempList[i]);
                cout << "time " << time << "ms: Process " << processList[i].get_id() << " (tau " << tauTracker[tempList[i].get_id()] << "ms) completed I/O; added to ready queue [Q ";
				for (unsigned int e = 0; e < queueList.size(); e++){
					cout << queueList[e];
					if (e != (queueList.size()-1)){
						cout << " ";
					}
				}
				cout << endl;
            }
        }

        if(finished.size() == processList.size()){
            cout << "time " << time << "ms: Simulator ended for FCFS [Q empty]\n";
			cout << endl;
			break;
        }
        time++;
    }

}