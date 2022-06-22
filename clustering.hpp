#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include "LSH.hpp"
#include "hypercube.hpp"

using namespace std;

class Clustering{

    private:
        int K;
        float current_max_dist;
        struct clusterList{
            float Di;
            data* p;
        };
        list<clusterList> *cluster_Table;

        struct probability{
            float P;
            data r_point;       //for index r when chosen randomly, this is the antistixo point
        };
        vector<probability> P_array;

    public:
        Clustering();
        Clustering(int K_medians);
        ~Clustering();
        void init_Push(data* p);
        void Di_first_centroid(int d);
        void k_means(int d, int n, vector<data*> dataList);
        void table_Push(vector<data*> dataList, int t);
        int binary_search(vector<probability> P_array, int n, float target);
        void Lloyds(int d, vector<data*> dataList);
        data* new_centroid(int i, int d);
        void reverse_range_search_LSH(LSH* objLSH, int L, int R, vector<data*> dataList);
        static bool cond(const clusterList& value);
        void table_Push_reverse(vector<data*> dataList, int t);
        void reverse_range_search_HYPER(HyperCube objHYPER, int R, vector<data*> dataList, int M, int probes);
        void silhouttes(ofstream& outfile);
        void output(ofstream& outfile, int d);
        void optional(ofstream& outfile, int d);
        void print();


};