#include <vector>
#include <string>
#include "Process.h"

class FCFS{
public:
		FCFS(std::vector<Process> a_process_list);

		void FCFS_algorithm();

private:
		std::vector<Process> process_list;
		int time;
		std::vector<string> queue_list;

}