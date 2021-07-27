/* Process header file */

#include <string>
using namespace std;

class Process{
public:
		Process(string a_id, int a_arrival, int a_context_switch, vector<int> a_burst_list, vector<int> a_io_list);

		//Accessors
		string get_id() const;
		int get_arrival_time() const;
		int get_context_switch_time() const;
		vector<int> get_burst_list() const;
		vector<int> get_io_list() const;


private:
		string id;
		int arrival;
		int context_switch;
		vector<int> burst_list;
		vector<int> io_list;
};
