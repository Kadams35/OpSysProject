
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include <tgmath.h>
//#include <cassert>


double next_exp(double lambda, int upper_bound){
    double r = drand48();
    double x = -log(r)/lambda;
    if(x > upper_bound){
        next_exp(lambda, upper_bound);
    }
    else{
        return x;
    }
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

    srand48(seed); //Sets the seed (this needs to be done before each method to reset the randomization)
    
    //wtf is going on man
    return EXIT_SUCCESS;
}