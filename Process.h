/* Process header file */

#include <string>

class Process{
public:
		Process(char a_id, int a_arrival, std::vector<int> a_burst_list, std::vector<int> a_io_list);

		//Accessors
		char get_id() const;
		int get_arrival_time() const;
		std::vector<int> get_burst_list() const;
		std::vector<int> get_io_list() const;



private:
		char id;
		int arrival;
		std::vector<int> burst_list;
		std::vector<int> io_list;
};