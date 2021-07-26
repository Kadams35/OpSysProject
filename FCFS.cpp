/* FCFS Algorithm */
#include <iostream>
#include <string>
#include <vector>
#include "Process.h"


FCFS::FCFS(std::vector<Process> a_process_list){
	process_list = a_process_list;
	queue_list.push_back("Q");
	queue_list.push_back("empty");
}

void FCFS::FCFS_algorithm(){
	Process first = process_list[i];
	queue_list.erase(1);
	queue_list.push_back(first.get_id());

	std::cout << "time " << first.get_arrival_time() << "ms: Process " << first.get_id() << "arrived; added to the ready queue [";
	for (int x = 0; x < queue_list.size(); x++){
		std::cout << queue_list[x];
		if (x != queue_list.size()-1){
			std::cout << " ";
		}
	}
	std::cout << "\n";

	





}