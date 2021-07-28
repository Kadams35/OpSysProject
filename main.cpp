
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


double next_exp(double lambda, int upper_bound){
    double r = drand48();
    double x = -log(r)/lambda;
    if(x > upper_bound){
        next_exp(lambda, upper_bound);
    }
    else{
        return x;
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
        int num_cpu_bursts = ceil(next_exp(lambda, upper_bound) * 100);

        std::vector<int> cpu_bursts;
        std::vector<int> io_bursts;
        for (int j = 0; j < num_cpu_bursts; j++) {
            int temp, temp1;
            if (j == num_cpu_bursts - 1) {
                int temp = ceil(next_exp(lambda, upper_bound));
                int temp1 = 0;
            }
            else {
                int temp = ceil(next_exp(lambda, upper_bound));
                int temp1 = ceil(next_exp(lambda, upper_bound) * 10);
            }
            cpu_bursts.push_back(temp);
            io_bursts.push_back(temp1);
        }
        Main_list.push_back(Process(id, arrival, cpu_bursts, io_bursts));
    }
    return Main_list;
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
    //send to fcfsfsfscsdssfscdcd

    return EXIT_SUCCESS;
}