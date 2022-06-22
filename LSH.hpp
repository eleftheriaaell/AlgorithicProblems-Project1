#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <time.h>
#include "data.hpp"

using namespace std;

class LSH{ // LSH CLASS

    private:
        int k; 
        int w;
        int tableSize;
        int current_ID;//ID FOR THE CURRENT QUERY POINT 
        vector<float> *v_array;//ARRAY WITH K V FOR EACH Hi FUNCTION
        struct bucketList{//HASH TABLE NODE
            long ID;
            data* p;
        };

        vector<bucketList> *LSH_hashTable;//HASH TABLE
        int gp(bucketList* node);//g(p)
    
    public:
        LSH();
        LSH(int kk, int window, int tablesize);
        ~LSH();

        int current_q_bucket;//G BUCKET FOR CURRENT QUERY
        void hashPush(data* p);
        int hi(data* p, int w, int i);
        void v_calculator(int d);
        float inner_product(data* p, int i);
        long modulo(int rh, long M);
        int gp_cluster(data* node);
        vector<data> kNNsearch(vector<int> q, int d, int N);
        vector<data*> range_search(vector<int> q, int d, int minR, int maxR);
        void print();
        

};