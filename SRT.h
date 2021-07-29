#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <map>
#include <tuple>

using namespace std;

class SRT{
    public:
        SRT(vector<Process> passedProcessList, int passedContextSwitch, int mainLambda);
        void SRTAlgorithm();
    private:
        vector<Process> processList;
        int lambda;
        int contextSwitch;
        vector<char> queueList;
};