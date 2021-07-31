#include <iostream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

class SRT{
    public:
        SRT(vector<Process> passedProcessList, int passedContextSwitch, float mainLambda, float someAlpha);
        void SRTAlgorithm();
        int getNumContextSwitches();
        double getCPUUtilization();
    private:
        vector<Process> processList;
        float lambda;
        float alpha;
        int contextSwitch;
        int contextSwitchTracker;
        double totalTime;
        double CPUTime;
        vector<Process> queueList;
};