/* Process Functions */
#include <string>
#include <vector>
#include "Process.h"



Process::Process(std::string a_id, int a_arrival, int a_context_switch, int a_burst){
	id = a_id;
	arrival = a_arrival;
	context_switch = a_context_switch;
	burst_list = a_burst_list;
}


std::string Process::get_id() const{
	return id;
}


int Process::get_arrival_time() const{
	return arrival;
}


int Process::get_context_switch_time() const{
	return context_switch;
}


std::vector<int> Process::get_burst_list() const;{
	return burst_list;
}
