/* Process Functions */
#include <string>
#include <vector>
#include "Process.h"

using namespace std;

Process::Process(char a_id, int a_arrival, std::vector<int> a_burst_list, std::vector<int> a_io_list){
	id = a_id;
	arrival = a_arrival;
	burst_list = a_burst_list;
	io_list = a_io_list;
}


char Process::get_id() const{
	return id;
}


int Process::get_arrival_time() const{
	return arrival;
}


std::vector<int> Process::get_burst_list() const{
	return burst_list;
}

std::vector<int> Process::get_io_list() const{
	return io_list;
}
;