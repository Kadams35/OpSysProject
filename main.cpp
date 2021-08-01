#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include <tgmath.h>
#include <vector>
#include <stdlib.h>
//#include <cassert>
#include "Process.h"
#include "SJF.h"
#include "FCFS_RR.h"
#include "SRT.h"


double next_exp(double lambda, int upper_bound){
    double r = drand48();
    double x = -log(r)/lambda;
    if(x > upper_bound){
        x = next_exp(lambda, upper_bound);
    }
    return x;
}



std::vector<Process> mainlist(int n, int seed, double lambda, int upper_bound) {
    srand48(seed); //Sets the seed (this needs to be done before each method to reset the randomization)
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::vector<Process> Main_list;

    for (int i = 0; i < n; i++) {
        char id = alphabet[i];
        int arrival = next_exp(lambda, upper_bound);
        int num_cpu_bursts = ceil(drand48() * 100);

        std::vector<int> cpu_bursts;
        std::vector<int> io_bursts;
        for (int j = 0; j < num_cpu_bursts; j++) {
            if (j == num_cpu_bursts - 1) {
                int temp = ceil(next_exp(lambda, upper_bound));
                int temp1 = 0;
                cpu_bursts.push_back(temp);
                io_bursts.push_back(temp1);
            }
            else {
                int temp = ceil(next_exp(lambda, upper_bound));
                int temp1 = ceil(next_exp(lambda, upper_bound))*10;
                cpu_bursts.push_back(temp);
                io_bursts.push_back(temp1);

            }
        }

        Process temp = Process(id, arrival, cpu_bursts, io_bursts);
        if(num_cpu_bursts == 1){
            cout << "Process " << temp.get_id() << " (arrival time " << arrival << " ms) " << num_cpu_bursts << " CPU burst (tau " << ceil(1/lambda) << "ms)" << endl; 
        }
        else{
            cout << "Process " << temp.get_id() << " (arrival time " << arrival << " ms) " << num_cpu_bursts << " CPU bursts (tau " << ceil(1/lambda) << "ms)" << endl; 
        }
        Main_list.push_back(temp);
    }
    cout << "\n";
    return Main_list;
}

//Function to calculate the burst average given the list of processes
double calculateBurstAverage(std::vector<Process> mainList){
    double sum = 0.0;
    int totalBursts = 0;
    for(unsigned int i = 0; i < mainList.size(); i++){
        for(unsigned int j = 0; j < mainList[i].get_burst_list().size(); j++){
            sum = sum + mainList[i].get_burst_list()[j];
            totalBursts++;
        }
    }
    return round(sum/totalBursts*1000)/1000;
}



int main(int argc, char* argv[]) {
    if (argc != 8) {
        std::cerr << "Invalid number of arguments. Simulation requires 7." << std::endl;
        return EXIT_FAILURE;
    }

    int n = std::stoi(argv[1]); //cast our first argument to int n number of processes
    int seed = std::stoi(argv[2]); //second argument is the seed for random number generator
    double lambda = std::stod(argv[3]); //lambda value for interarrival times
    int upper_bound = std::stoi(argv[4]); //upper_bound for valid pseudo-random numbers
    int t_cs = std::stoi(argv[5]); //time in milliseconds for a context switch
    float alpha = std::stof(argv[6]); //alpha variable for exponential averaging //this might not be a float..
    int t_slice = std::stoi(argv[7]); //for RR algorithm, time slice value measured in milliseconds

    
    std::vector<Process> Main_list = mainlist(n, seed, lambda, upper_bound);
    
    //get total burst time and num of bursts
    int num_bursts = Main_list[0].get_burst_list().size();
    float total_burst_time = 0;
    for (unsigned int a = 0; a < Main_list.size(); a++){
        std::vector<int> burst_list = Main_list[a].get_burst_list();
        for (unsigned int b = 0; b < burst_list.size(); b++){
            total_burst_time += burst_list[b];
        }
    }


    //send to fcfsfsfscsdssfscdcd
    //Storing the burst average
    double averageCPUBurst = calculateBurstAverage(Main_list);
    //cout << "Average CPU burst: " << averageCPUBurst << endl;

    //FCFS Algorithm
    FCFS_RR test = FCFS_RR(Main_list, t_cs, t_slice);
    std::map<char, vector<int> > results1 = test.FCFS_algorithm();
    //calculate metrics
    float wait_time1 = 0;
    float turnaround_time1 = 0;
    int context_switch1 = 0;

    int time1 = test.get_total_time();
    float CPU1 = (total_burst_time/time1)*100;

    for(unsigned int x = 0; x < Main_list.size(); x++){
        char name = Main_list[x].get_id();
        wait_time1 += (results1[name][0]/num_bursts);
        turnaround_time1 += (results1[name][1]/num_bursts);
        context_switch1 += results1[name][2];
    }
    wait_time1 = wait_time1/Main_list.size();
    turnaround_time1 = turnaround_time1/Main_list.size();
  
    //SJF Algorithm
    SJF test2 = SJF(Main_list, t_cs, lambda, alpha);
    test2.SJFAlgorithm();

    SRT test3 = SRT(Main_list, t_cs, lambda, alpha);
    test3.SRTAlgorithm();
  
    FCFS_RR test4 = FCFS_RR(Main_list, t_cs, t_slice);
    std::map<char, vector<int> > results4 = test4.RR_algorithm();
     //calculate metrics
    float wait_time4 = 0;
    float turnaround_time4 = 0;
    int context_switch4 = 0;
    int preemption4 = 0;
    int time4 = test.get_total_time();
    float CPU4 = (total_burst_time/time4)*100;

    for(unsigned int x = 0; x < Main_list.size(); x++){
        char name = Main_list[x].get_id();
        wait_time4 += (results4[name][0]/num_bursts);
        turnaround_time4 += (results4[name][1]/num_bursts);
        context_switch4 += results4[name][2];
        preemption4 += results4[name][3];
    }
    wait_time4 = wait_time4/Main_list.size();
    turnaround_time4 = turnaround_time4/Main_list.size();

    

    std::ofstream out ("simout.txt");

    out << "Algorithm FCFS" << std::endl;
    out << "-- average CPU burst time: " << averageCPUBurst << " ms" << std::endl;
    out << "-- average wait time: " << wait_time1 << " ms" << std::endl;
    out << "-- average turnaround time: " << turnaround_time1 << " ms" << std::endl;
    out << "-- total number of context switches: " << context_switch1 << std::endl;
    out << "-- total number of preemptions: 0" << std::endl;
    out << "-- CPU utilization: " << CPU1 << "%" << std::endl;
    out << "Algorithm SJF" << std::endl;
    out << "-- average CPU burst time: " << averageCPUBurst << " ms" << std::endl;
    out << "-- average wait time: " << test2.getAvgWaitTime() << " ms" << std::endl;
    out << "-- average turnaround time: " << test2.getAvgWaitTime()+averageCPUBurst+t_cs << " ms" << std::endl;
    out << "-- total number of context switches: " << test2.getNumContextSwitches() << std::endl;
    out << "-- total number of preemptions: 0" << std::endl;
    out << "-- CPU utilization: " << test2.getCPUUtilization() << "%" << std::endl;
    out << "Algorithm SRT" << std::endl;
    out << "-- average CPU burst time: " << averageCPUBurst << " ms" << std::endl;
    out << "-- average wait time: " << test3.getWaitTime() << " ms" << std::endl;
    out << "-- average turnaround time: " << "0" << " ms" << std::endl;
    out << "-- total number of context switches: " << test3.getNumContextSwitches() << std::endl;
    out << "-- total number of preemptions: " << "0" << std::endl;
    out << "-- CPU utilization: " << test3.getCPUUtilization() << "%" << std::endl;
    out << "Algorithm RR" << std::endl;
    out << "-- average CPU burst time: " << averageCPUBurst << " ms" << std::endl;
    out << "-- average wait time: " << wait_time4 << " ms" << std::endl;
    out << "-- average turnaround time: " << turnaround_time4 << " ms" << std::endl;
    out << "-- total number of context switches: " << context_switch4 << std::endl;
    out << "-- total number of preemptions: " << preemption4 << std::endl;
    out << "-- CPU utilization: " << CPU4 << "%" << std::endl;

    out.close();



    return EXIT_SUCCESS;
}
