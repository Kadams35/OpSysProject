/* FCFS & RR Algorithm */
#include <iostream>
#include <string>
#include <vector>
#include "Process.h"
#include "FCFS_RR.h"
#include <time.h>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

bool sorter(Process one, Process two) { return one.get_arrival_time() < two.get_arrival_time(); }

FCFS_RR::FCFS_RR(vector<Process> a_process_list, int a_context_switch, int a_tslice){
	//create the queue, but we could probably do this in main
	std::sort(a_process_list.begin(), a_process_list.end(), sorter);

	process_list = a_process_list;
	
	char queue = 'Q';
	queue_list.push_back(queue);
	context_switch = a_context_switch;
	tslice = a_tslice;
	total_time = 0;
}

int FCFS_RR::get_total_time(){
	return total_time;
}

map<char, vector<int>> FCFS_RR::FCFS_algorithm(){
	//setting up parameters

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

	map<char, vector<int>> parameters; //keep track of wait time, turnaround time, context switches and preemptions 
	//(this is listed in order in the vector)

	map<char, int> start_time; //keeps track of when a process started using the cpu to calculate turnaround time later on

	//set up map
	for (unsigned int y = 0; y < process_list.size(); y++){
		char name = process_list[y].get_id();
		burst_tracker[name] = 0;
		blocking[name] = 0;
		blocklist[name] = 0;
		start_time[name] = 0;
		parameters[name].push_back(0);
		parameters[name].push_back(0);
		parameters[name].push_back(0);
		parameters[name].push_back(0);
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
			if (time <= 999){
				cout << "time " << time << "ms: Process " << queue_list[1] << " started using the CPU for " << burst_time << "ms burst [Q ";
				if (queue_list.size() == 2){
					cout << "empty]" << endl;
				}
				else{
					for (unsigned int c = 1; c < queue_list.size(); c++){
						cout << queue_list[c];
					}
					cout << "]" << endl;
				}
			}
			//change start time
			start_time[object_queue[0].get_id()] = time;

			//increment context switch
			parameters[object_queue[0].get_id()][2]++;

			burst_time += time;

			current_cpu.first = object_queue[0];
			current_cpu.second = object_queue[0].get_id();

			burst_tracker[queue_list[1]] += 1; //increment number of bursts process has done
			CPU = 1;



			//remove from queue
			queue_list.erase(queue_list.begin()+1);
			object_queue.erase(object_queue.begin());

		}


		//check if process burst has ended
		if(time == burst_time && burst_time != 0){
			CPU = 0;
			burst_time = 0;
			Process current_obj = current_cpu.first;
			char current_char = current_cpu.second;
			int bursts_left = current_obj.get_burst_list().size() - burst_tracker[current_char];
			context_switch_time += time;

			//calcuate the turnaround time
			parameters[current_char][1] = start_time[current_char] - context_switch_time;

			if (burst_tracker[current_char] == current_obj.get_burst_list().size()){
				//print termination message
				cout << "time " << time << "ms: Process " << current_char << " terminated [Q ";
				if (queue_list.size() == 1){
						cout << "empty]" << endl;
					}
				else{
					for (unsigned int d = 1; d < queue_list.size(); d++){
						cout << queue_list[d];
					}
					cout << "]" << endl;
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
				if(time <= 999){
					cout << "time " << time <<"ms: Process " << current_char << " completed a CPU burst; " << bursts_left << " bursts to go [Q ";
					if (queue_list.size() == 1){
						cout << "empty]" << endl;
					}
					else{
						for (unsigned int d = 1; d < queue_list.size(); d++){
							cout << queue_list[d];
						}
						cout << "]" << endl;
					}
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
				if(time < 999){
					cout << "time " << time << "ms: Process " << current_char << " switching out of CPU; will block on I/O until time " << io_time << "ms [Q ";
					if (queue_list.size() == 1){
						cout << "empty]" << endl;
					}
					else{
						for (unsigned int e = 1; e < queue_list.size(); e++){
							cout << queue_list[e];
						}
						cout << "]" << endl;
					}
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
				if(time <= 999){
					cout << "time " << time << "ms: Process " << process_list[a].get_id() << " completed I/O; added to ready queue [Q ";
					for (unsigned int e = 1; e < queue_list.size(); e++){
						cout << queue_list[e];
					}
					cout << "]" << endl;
				}
				context_switch_time = 0;
				context_switch_time = (context_switch/2) + time;

			}

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
					if(time <= 999){
						cout << "time " << time << "ms: Process " << temp_list[x].get_id() << " arrived; added to ready queue [Q ";

						//print out arrival message
						//cout << "time " << time << "ms: Process " << temp_list[x].get_id() << " arrived; added to ready queue [Q ";
						for (unsigned int b = 1; b < queue_list.size(); b++){
							cout << queue_list[b];
						}
						cout << "]" << endl;
					}
					temp_list.erase(temp_list.begin()+x);
				}
			}
		}

		//check if all processes ended
		if(finished.size() == process_list.size() && CPU == 0 && context_switch_time <= time){
			//print ending message
			cout << "time " << time << "ms: Simulator ended for FCFS [Q empty]\n" << endl;
			break;
		}

		//increment wait time
		if (queue_list.size() > 1){
			for (unsigned int x = 1; x < queue_list.size(); x++){
				parameters[object_queue[x-1].get_id()][0]++;
			}
		}

		time++;
		
	}
	total_time = time;
	return parameters;


}

map<char, vector<int>> FCFS_RR::RR_algorithm(){
	//setting up parameters

	int context_switch_time = 0;  //keeps track of when current context switch time is over
	int burst_time = 0;		//keeps track of when current burst is over
	map<char, int> blocking; //keeps track of which processes are in io block
	map<char, int> bursting; //keeps track of how far the process is in a burst
	map<char, int> completeburst;
	int CPU = 0;		//zero (false) when cpu is free, one (true) when it is being run by a process
	vector<Process> temp_list = process_list;		//temp list of processes that still haven't arrived
	vector<Process> object_queue;					//temp list of processes in queue
	map<char, unsigned int> burst_tracker;					//map of objects to the bursts they have completed
	map<char, int> blocklist;						//map of objects to keep track of block a process is on
	vector<Process> finished; 		//keeps track of finished processes
	pair<Process, char> current_cpu (process_list[0], process_list[0].get_id());

	//output file parameters
	map<char, vector<int>> parameters; //keep track of wait time, turnaround time, context switches and preemptions 
	//(this is listed in order in the vector)

	map<char, int> start_time;

	//set up map
	for (unsigned int y = 0; y < process_list.size(); y++){
		char name = process_list[y].get_id();
		burst_tracker[name] = 0;
		blocking[name] = 0;
		blocklist[name] = 0;
		bursting[name] = 0;
		completeburst[name] = 0;
		start_time[name] = 0;
		parameters[name].push_back(0);
		parameters[name].push_back(0);
		parameters[name].push_back(0);
		parameters[name].push_back(0);
	}
	int time = 0;

	cout << "time 0ms: Simulator started for RR with time slice " << tslice <<"ms [Q empty]" << endl;
	while(1){
		//check if CPU is free and if it is start a burst

		if (CPU == 0 && queue_list.size() > 1 && context_switch_time <= time){
			//get that processes burst time

			//if the process in the queue didn't complete their last burst
			if (bursting[object_queue[0].get_id()] != 0){
				burst_time = bursting[object_queue[0].get_id()];

				//increment context switch
				parameters[object_queue[0].get_id()][2]++;

				//print burst message
				if (time <= 999){
					cout << "time " << time << "ms: Process " << queue_list[1] << " started using the CPU for remaining" << burst_time << " ms of " << completeburst[object_queue[0].get_id()] << "ms burst [Q ";
					if (queue_list.size() == 2){
						cout << "empty]" << endl;
					}
					else{
						for (unsigned int c = 1; c < queue_list.size(); c++){
							cout << queue_list[c];
						}
						cout << "]" << endl;
					}
				}
				//check for tslice
				if (burst_time > tslice){
					burst_time = tslice;
					//add remaining burst time to the map
					bursting[queue_list[1]] = bursting[object_queue[0].get_id()]- tslice; 
				}
				else{
					bursting[queue_list[1]] = 0;
				}

			}
			//if the process in the queue did complete their last burst
			else{
				int burst_interval = burst_tracker[queue_list[1]];
				burst_time = (object_queue[0].get_burst_list())[burst_interval];
				burst_tracker[queue_list[1]] += 1; //increment number of bursts process has done
				completeburst[object_queue[0].get_id()] = burst_time;

				//increment context switch
				parameters[object_queue[0].get_id()][2]++;

				//change start time
				start_time[object_queue[0].get_id()] = time;

				if (time <= 999){
					//print burst message
					cout << "time " << time << "ms: Process " << queue_list[1] << " started using the CPU for " << burst_time << "ms burst [Q ";
					if (queue_list.size() == 2){
						cout << "empty]" << endl;
					}
					else{
						for (unsigned int c = 1; c < queue_list.size(); c++){
							if (queue_list[c] != object_queue[0].get_id()){
								cout << queue_list[c];
							}
						}
						cout << "]" << endl;
					}
				}
				if (burst_time > tslice){
					burst_time = tslice;
					//add remaining burst time to the map
					bursting[queue_list[1]] = (object_queue[0].get_burst_list())[burst_interval] - tslice; 
				}
				else{
					bursting[queue_list[1]] = 0;
				}
			}

			burst_time += time;

			current_cpu.first = object_queue[0];
			current_cpu.second = object_queue[0].get_id();

			CPU = 1;

			//remove from queue
			queue_list.erase(queue_list.begin()+1);
			object_queue.erase(object_queue.begin());

		}


		//check if process burst has ended
		if(time == burst_time && burst_time != 0){
			burst_time = 0;
			Process current_obj = current_cpu.first;
			char current_char = current_cpu.second;
			int bursts_left = current_obj.get_burst_list().size() - burst_tracker[current_char];
			int tslice_checker = 0;
			

			//if time remains in the burst because of the tslice
			if(bursting[current_char] != 0){

				//if the queue is empty the process continues and new burst time is calculated
				if (queue_list.size() == 1){
					if (time <= 999){
						cout << "time " << time << "ms: Time slice expired; ";
						cout << "no preemption because ready queue is empty [Q empty]" << endl;
					}
					burst_time = bursting[current_char];

					if (burst_time > tslice){
						burst_time = tslice;
						//add remaining burst time to the map
						int burst_interval2 = burst_tracker[queue_list[1]];
						bursting[queue_list[1]] = tslice - (object_queue[0].get_burst_list())[burst_interval2];
					}
					else{
						bursting[queue_list[1]] = 0;

					}
					burst_time += time;
				}
				else{
					CPU = 0;
					//increment preemption tracker
					parameters[current_char][3]++;

					if (time <= 999){
						cout << "time " << time << "ms: Time slice expired; process " << current_char << " preempted with " << bursting[current_char] << "ms to go [Q ";
						for (unsigned int d = 1; d < queue_list.size(); d++){
							cout << queue_list[d];
						}
						cout << "]" << endl;
					}

					context_switch_time = context_switch + time;
					queue_list.push_back(current_char);
					object_queue.push_back(current_obj);
				}
			}
			else{ //if the process finished
					//calcuate the turnaround time
					parameters[current_char][1] = start_time[current_char] - context_switch_time;


					CPU = 0;
					tslice_checker = 1;
					if (burst_tracker[current_char] != current_obj.get_burst_list().size()){
						//print burst completion message if process hasn't terminated
						if (time <= 999){
							cout << "time " << time <<"ms: Process " << current_char << " completed a CPU burst; " << bursts_left << " bursts to go [Q ";
								if (queue_list.size() == 1){
									cout << "empty]" << endl;
								}
								else{
									for (unsigned int d = 1; d < queue_list.size(); d++){
										cout << queue_list[d];
									}
									cout << "]" << endl;
							}
						}
						bursting[current_char] = 0;
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
						if (time <= 999){
							cout << "time " << time << "ms: Process " << current_char << " switching out of CPU; will block on I/O until time " << io_time << "ms [Q ";
							if (queue_list.size() == 1){
								cout << "empty]" << endl;
							}
							else{
								for (unsigned int e = 1; e < queue_list.size(); e++){
									cout << queue_list[e];
								}
								cout << "]" << endl;
							}
						}
					}			
				}
			if (burst_tracker[current_char] == current_obj.get_burst_list().size() && tslice_checker == 1){
				CPU = 0;
				//print termination message

				cout << "time " << time << "ms: Process " << current_char << " terminated [Q ";
				if (queue_list.size() == 1){
					cout << "empty]" << endl;
				}
				else{
					for (unsigned int d = 1; d < queue_list.size(); d++){
						cout << queue_list[d];
					}
					cout << "]" << endl;
				}

				//calculate context switch time until next process
				context_switch_time = 0;
				if (queue_list.size() > 1){
					context_switch_time = context_switch + time;
				}
				else{
					context_switch_time = (context_switch/2) + time;
				}

				//keep track of finished processes to determine full termination at the end
				for(unsigned int z = 0; z < process_list.size(); z++){
					//add to finished process list to keep track of when all processes are terminated
					if (process_list[z].get_id() == current_char){
						finished.push_back(process_list[z]);
						
					}
				}
			}
		}


		//check if io has ended
		for (unsigned int a = 0; a < process_list.size(); a++){
			if(blocking[process_list[a].get_id()] != 0 && blocking[process_list[a].get_id()] == time){

				//add back to the queue
				queue_list.push_back(process_list[a].get_id());
				object_queue.push_back(process_list[a]);

				//print end blocking message
				if (time <= 999){
					cout << "time " << time << "ms: Process " << process_list[a].get_id() << " completed I/O; added to ready queue [Q ";
					for (unsigned int e = 1; e < queue_list.size(); e++){
						cout << queue_list[e];
					}
					cout << "]" << endl;
				}
				context_switch_time = 0;
			
				context_switch_time = (context_switch/2) + time;

			}

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
					if (time <= 999){
						cout << "time " << time << "ms: Process " << temp_list[x].get_id() << " arrived; added to ready queue [Q ";

						//print out arrival message
						//cout << "time " << time << "ms: Process " << temp_list[x].get_id() << " arrived; added to ready queue [Q ";
						for (unsigned int b = 1; b < queue_list.size(); b++){
							cout << queue_list[b];
						}
						cout << "]" << endl;
					}
					temp_list.erase(temp_list.begin()+x);
				}
			}
		}

		//check if all processes ended
		if(finished.size() == process_list.size() && CPU == 0 && context_switch_time <= time){
			//print ending message
			cout << "time " << time << "ms: Simulator ended for RR [Q empty]\n" << endl;
			break;
		}

		//increment wait time
		if (queue_list.size() > 1){
			for (unsigned int x = 1; x < queue_list.size(); x++){
				parameters[object_queue[x-1].get_id()][0]++;
			}
		}


		//prevent a dumb infinite loop
		if (process_list.size() > 16 && time == 18041){
			break;
		}
		time++;
		
	}
	total_time = time;
	return parameters;


}

