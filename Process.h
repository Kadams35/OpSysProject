/* Process header file */

#include <string>

class Process{
public:
		Process(std::string a_id, int a_arrival, int a_context_switch, std::vector<int> a_burst_list);

		//Accessors
		std::string get_id() const;
		int get_arrival_time() const;
		int get_context_switch_time() const;
		std::vector<int> get_burst_time() const;


private:
		std::string id;
		int arrival;
		int context_switch;
		std::vector<int> burst_list;
};
