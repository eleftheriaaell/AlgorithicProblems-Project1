#include "hypercube.hpp"
#include <fstream>
#include <sstream>
#include <time.h>

int main(int argc, char *argv[]){

    ifstream infile(argv[2]);
    ofstream outfile;
    ifstream infileq(argv[4]);

    int k;
    int N;
    int R;
    int M;
    int probes;

    if(argc == 17){
        outfile.open(argv[12]);        //if taken in right position with other given argvs
        k = atoi(argv[6]);
        N = atoi(argv[14]);
        R = atoi(argv[16]);
        M = atoi(argv[8]);
        probes = atoi(argv[10]);
    }
    else{
        outfile.open(argv[6]);      //if taken without other argvs is placed in 6th position
        k = 14;
        N = 1;
        R = 10000;
        M = 10;
        probes = 2;
    }

    int w = 400;

    struct timeval start, end;

    string line;
    int element;
    vector<data*> dataList;

    HyperCube obj = HyperCube(k, w);   

    srand(time(NULL));

    ////////////////////// filling the hashtables ///////////////////////
    int flag = 0;
    while(getline(infile, line)){//FOR EACH POINT
        data* p = new data();
        stringstream ss(line);
        
        ss >> element;
        p->ID = element;//POINT ID
        while( ss >> element )//SPLIT THE LINE AND GET THE ELEMENTS OF VECTORS
            p->p_data.push_back(element);
        
        p->d = p->p_data.size();
        
        dataList.push_back(p);//PUSH POINT IN DATALIST

        
        if(flag == 0){//CALCULATE VECTOR V FOR EACH H FUNCTION ONLY ONCE
            obj.v_calculator(p->d);
            flag = 1;
        }
        
        obj.hashPush(dataList[dataList.size() - 1]);//PUSH POINT IN HASH TABLE
    }

    //////////////////////////////////////////////////////////////////

    string lineq;
    int elementq;
    
    while(getline(infileq, lineq)){// FOR EACH QUERY POINT
        vector<int> q_data;
        stringstream ss(lineq);
        
        ss >> elementq;//QUERY POINT ID
        int q_id = elementq;
        while( ss >> elementq )//SPLIT THE LINE AND GET THE ELEMENTS OF VECTOR
            q_data.push_back(elementq);


        int d = q_data.size();

        //////////////////////////// finding the kNN /////////////////////
        
        vector<vector<data>> knn_arr;
        data kNNarr[N];//N NEAREST NEIGHBORS ARRAY

        gettimeofday(&start, NULL);
        ios_base::sync_with_stdio(false);

        knn_arr.push_back(obj.kNNsearch(q_data, d, N, M, probes));//GET N NEAREST NEIGHBORS

        gettimeofday(&end, NULL);

        double exec_time;
  
        exec_time = (end.tv_sec - start.tv_sec) * 1e6;
        exec_time = (exec_time + (end.tv_usec - start.tv_usec)) * 1e-6;
          
        int knn_size = 0;
        for(auto k: knn_arr){
            for(auto l: k){
                kNNarr[knn_size] = l;
                knn_size++;
            }
        }

        if(knn_size == 0)
            continue;

        sort(&kNNarr[0], &kNNarr[knn_size - 1], compare_distance);

        gettimeofday(&start, NULL);
        ios_base::sync_with_stdio(false);

        float distances[dataList.size()];
        for(int i = 0; i < dataList.size(); i++)//FOR EACH POINT FIND TRUE DISTANCE
            distances[i] = distance(dataList[i]->p_data, q_data, dataList[i]->d);

        gettimeofday(&end, NULL);

        double exec_time_2;
  
        exec_time_2 = (end.tv_sec - start.tv_sec) * 1e6;
        exec_time_2 = (exec_time_2 + (end.tv_usec - start.tv_usec)) * 1e-6;

        int n = sizeof(distances) / sizeof(distances[0]);
        sort(distances, distances + n);
        
        int prev = kNNarr[0].ID;
        
        outfile << "Query: " << q_id << endl << "Nearest neighbor-1: " << kNNarr[0].ID << endl;
        outfile << "distanceHypercube: " << kNNarr[0].current_distance << endl;
        outfile << "distanceTrue: " << distances[0] << endl << endl;

        int cnt = 1;
        int j;
        for(j = 1; j < knn_size; j++){
            if(cnt == N)
                break;

            if(kNNarr[j].ID == prev)
                continue;

            prev = kNNarr[j].ID;
            
            int x = j + 1;
            outfile << "Nearest neighbor-" << x << ": " << kNNarr[j].ID << endl;
            outfile << "distanceHypercube: " << kNNarr[j].current_distance << endl;
            outfile << "distanceTrue: " << distances[j] << endl << endl;

            cnt++;           
                
        }

        outfile << "tHypercube: " << exec_time << endl;
        outfile << "tTrue: " <<exec_time_2 << endl << endl;        

        //////////////////////////////// range-search /////////////////////////////////

        vector<data*> Rarr;// RANGE_POINTS ARRAY
       
        Rarr = obj.range_search(q_data, d, 0, R, M, probes);//GET POINTS WITH DISTANCE <R

        if(Rarr.size() == 0)
            continue;   
        
        sort(Rarr.begin(), Rarr.end(), compare_ID_pointer);
        
        prev = Rarr.front()->ID;
        
        outfile << "R-near neighbors: " << endl << Rarr.front()->ID << endl;

        for(auto v: Rarr){
            
            if(v->ID == prev)
               continue;

            prev = v->ID;
            outfile << v->ID << endl;

        }

        outfile << endl;
       
        
    }

    outfile.close();
}