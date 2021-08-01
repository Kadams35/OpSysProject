#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <map>
#include <tuple>

using namespace std;


class FCFS_RR{
public:
		FCFS_RR(vector<Process> a_process_list, int a_context_switch, int a_tslice);

		map<char, vector<int> > FCFS_algorithm();
		map<char, vector<int> > RR_algorithm();
		int get_total_time();

private:
		vector<Process> process_list;
		int context_switch;
		vector<char> queue_list;
		int tslice;
		int total_time;

};
