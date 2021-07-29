/* FCFS Algorithm */
#include <iostream>
#include <string>
#include <vector>
#include "Process.h"
#include "FCFS.h"
#include <time.h>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

bool sorter(Process one, Process two) { return one.get_arrival_time() < two.get_arrival_time(); }

FCFS::FCFS(vector<Process> a_process_list, int a_context_switch){
	//create the queue, but we could probably do this in main
	std::sort(a_process_list.begin(), a_process_list.end(), sorter);

	process_list = a_process_list;
	
	char queue = 'Q';
	queue_list.push_back(queue);
	context_switch = a_context_switch;
}

void FCFS::FCFS_algorithm(){
	//setting up parameters

	context_switch_tracker = 0;  //keeps track of context switches
	int context_switch_time = 0;  //keeps track of when current context switch time is over
	int burst_time = 0;		//keeps track of when current burst is over
	map<char, int> blocking; //keeps track of which processes are in io block
	int CPU = 0;		//zero (false) when cpu is free, one (true) when it is being run by a process
	vector<Process> temp_list = process_list;		//temp list of processes that still haven't arrived
	vector<Process> object_queue;					//temp list of processes in queue
	map<char, unsigned int> burst_tracker;					//map of objects to the bursts they have completed
	map<char, int> blocklist;						//map of objects to keep track of block a process is on
	vector<Process> finished; 		//keeps track of finished processes
	pair<Process, char> current_cpu (process_list[0], process_list[0].get_id());

	//set up map
	for (unsigned int y = 0; y < process_list.size(); y++){
		char name = process_list[y].get_id();
		burst_tracker[name] = 0;
		blocking[name] = 0;
		blocklist[name] = 0;
	}
	int time = 0;

	cout << "time 0ms: Simulator started for FCFS [Q empty]" << endl;
	while(1){
		//check if CPU is free and if it is start a burst

		if (CPU == 0 && queue_list.size() > 1 && context_switch_time <= time){

			//get that processes burst time
			int burst_interval = burst_tracker[queue_list[1]];
			burst_time = (object_queue[0].get_burst_list())[burst_interval];

			//print burst message
			cout << "time " << time << "ms: Process " << queue_list[1] << " started using the CPU for " << burst_time << "ms burst [Q ";
			if (queue_list.size() == 2){
				cout << "empty]" << endl;
			}
			else{
				for (unsigned int c = 1; c < queue_list.size(); c++){
					cout << queue_list[c];
					if (c != (queue_list.size()-1)){
						cout << " ";
					}
				}
				cout << "]" << endl;
			}


			burst_time += time;

			current_cpu.first = object_queue[0];
			current_cpu.second = object_queue[0].get_id();

			burst_tracker[queue_list[1]] += 1; //increment number of bursts process has done
			CPU = 1;



			//remove from queue
			queue_list.erase(queue_list.begin()+1);
			object_queue.erase(object_queue.begin());

		}

		//check if a process has arrived

		if(temp_list.size() != 0){

			for (unsigned int x = 0; x < temp_list.size(); x++){

				if (time == temp_list[x].get_arrival_time()){

					//set up context switch if first in queue
					if (queue_list.size() == 1){
						context_switch_time = (context_switch/2) + time;
						
					}

					queue_list.push_back(temp_list[x].get_id());
					object_queue.push_back(temp_list[x]);
					cout << "time " << time << "ms: Process " << temp_list[x].get_id() << " arrived; added to ready queue [Q ";
					temp_list.erase(temp_list.begin()+x);

					//print out arrival message
					//cout << "time " << time << "ms: Process " << temp_list[x].get_id() << " arrived; added to ready queue [Q ";
					for (unsigned int b = 1; b < queue_list.size(); b++){
						cout << queue_list[b];
						if (b != (queue_list.size()-1)){
							cout << " ";
						}
					}
					cout << "]" << endl;
				}
			}
		}

		//check if process burst has ended
		if(time == burst_time && burst_time != 0){
			CPU = 0;
			burst_time = 0;
			Process current_obj = current_cpu.first;
			char current_char = current_cpu.second;
			int bursts_left = current_obj.get_burst_list().size() - burst_tracker[current_char];
			context_switch_time += time;

			if (burst_tracker[current_char] == current_obj.get_burst_list().size()){
				//print termination message
				cout << "time " << time << "ms: Process " << current_char << " terminated [Q ";
				if (queue_list.size() == 1){
						cout << "empty]" << endl;
					}
				else{
					for (unsigned int d = 1; d < queue_list.size(); d++){
						cout << queue_list[d];
						if (d != (queue_list.size()-1)){
							cout << " ";
						}
					}
					cout << "]" << endl;
					context_switch_tracker += 1;
				}
				context_switch_time = 0;
				if (queue_list.size() > 1){
					context_switch_time = context_switch + time;
				}
				else{
					context_switch_time = (context_switch/2) + time;
				}


				for(unsigned int z = 0; z < process_list.size(); z++){
					//add to finished process list to keep track of when all processes are terminated
					if (process_list[z].get_id() == current_char){
						finished.push_back(process_list[z]);
						
					}
				}
			}
			else{
				//print burst completion message if process hasn't terminated
				cout << "time " << time <<"ms: Process " << current_char << " completed a CPU burst; " << bursts_left << " bursts to go [Q ";
					if (queue_list.size() == 1){
						cout << "empty]" << endl;
					}
					else{
						for (unsigned int d = 1; d < queue_list.size(); d++){
							cout << queue_list[d];
							if (d != (queue_list.size()-1)){
								cout << " ";
							}
						}
						cout << "]" << endl;
						context_switch_tracker += 1;
					}

				//set up I/0 blocking

				int io_interval = blocklist[current_char];
				int io_time = current_obj.get_io_list()[io_interval];
				io_time += time;
				io_time += (context_switch/2);

				if (queue_list.size() > 1){
					context_switch_time = context_switch + time;
				}
				else{
					context_switch_time = (context_switch/2) + time;
				}

				blocklist[current_char] += 1;
				blocking[current_char] = io_time;

				//print io block message
				cout << "time " << time << "ms: Process " << current_char << " switching out of CPU; will block on I/O until time " << io_time << "ms [Q ";
					if (queue_list.size() == 1){
						cout << "empty]" << endl;
					}
					else{
						for (unsigned int e = 1; e < queue_list.size(); e++){
							cout << queue_list[e];
							if (e != (queue_list.size()-1)){
								cout << " ";
							}
						}
						cout << "]" << endl;
					}			
			}

		}

		//check if io has ended
		for (unsigned int a = 0; a < process_list.size(); a++){
			if(blocking[process_list[a].get_id()] != 0 && blocking[process_list[a].get_id()] == time){
				//blocking[process_list[a].get_id()] == 0;

				//add back to the queue
				queue_list.push_back(process_list[a].get_id());
				object_queue.push_back(process_list[a]);

				//print end blocking message
				cout << "time " << time << "ms: Process " << process_list[a].get_id() << " completed I/O; added to ready queue [Q ";
				for (unsigned int e = 1; e < queue_list.size(); e++){
					cout << queue_list[e];
					if (e != (queue_list.size()-1)){
						cout << " ";
					}
				}
				cout << "]" << endl;
				context_switch_time = 0;
				context_switch_time = (context_switch/2) + time;

			}

		}


		//check if all processes ended
		if(finished.size() == process_list.size() && CPU == 0 && context_switch_time <= time){
			//print ending message
			cout << "time " << time << "ms: Simulator ended for FCFS [Q empty]\n" << endl;
			break;
		}
		time++;
		
	}


}




int FCFS::get_context_switch_num(){
		return context_switch_tracker;
}