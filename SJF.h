#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <map>
#include <tuple>

using namespace std;

class SJF{
    public:
        SJF(vector<Process> passedProcessList, int passedContextSwitch, float mainLambda);
        void SJFAlgorithm();
    private:
        vector<Process> processList;
        float lambda;
        int contextSwitch;
        vector<char> queueList;
};