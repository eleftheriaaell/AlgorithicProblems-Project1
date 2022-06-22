#pragma once

#include <vector>
#include <random>
#include <chrono>
#include <bits/stdc++.h>
#include <sys/time.h>

using namespace std;

struct data{//POINTS NODE
    vector<int> p_data;//VECTOR P
    int ID;
    int d;//DIMENSION
    float current_distance = 0.0;//DISTANCE BETWEEN POINT AND CURRENT QUERY POINT
    int cluster_counter = 0;
};

bool comp(data const &a, data const &b);

float distance(vector<int> x, vector<int> y, int d);

bool compare_distance(data const& a, data const& b);

bool compare_ID_pointer(data* const& a, data* const& b);