#include "data.hpp"
#include <iostream>
using namespace std;

bool comp(data const &a, data const &b){// COMPARE POINTS BY DISTANCES 
    return a.current_distance < b.current_distance;
}

float distance(vector<int> x, vector<int> y, int d){//CALCULATE DISTANCE
  
    float dist = 0.0;
    int z;
    float n = 2.0;                  //for Eucledian distance

    for(int i = 0; i < d; i++){
        z = abs(x[i] - y[i]);
        dist = dist + (pow(z, n));
        
    }
    
    dist = pow(dist, (1.0/n));
    return dist;
}

bool compare_distance(data const& a, data const& b){//COMPARE POINTS BY DISTANCES
    return a.current_distance < b.current_distance;
}

bool compare_ID_pointer(data* const& a, data* const& b){// COMPAIR POINT BY ID
    return a->ID < b->ID;
}