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

		void FCFS_algorithm();
		void RR_algorithm();
		int get_context_switch_num();

private:
		vector<Process> process_list;
		int context_switch;
		vector<char> queue_list;
		int context_switch_tracker;
		int tslice;

};