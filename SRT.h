#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <map>
#include <tuple>

using namespace std;

class SRT{
    public:
        SRT(vector<Process> passedProcessList, int passedContextSwitch, float mainLambda);
        void SRTAlgorithm();
    private:
        vector<Process> processList;
        float lambda;
        int contextSwitch;
        vector<char> queueList;
};