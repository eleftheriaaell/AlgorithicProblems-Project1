#include "LSH.hpp"
#include <fstream>
#include <sstream>

int main(int argc, char *argv[]){
    
    ifstream infile(argv[2]);
    ofstream outfile;
    ifstream infileq(argv[4]);

    int k;
    int L;
    int N;
    int R;

    if(argc == 15){
        outfile.open(argv[10]);        //if taken in right position with other given argvs
        k = atoi(argv[6]);
        L = atoi(argv[8]);
        N = atoi(argv[12]);
        R = atoi(argv[14]);
    }
    else{
        outfile.open(argv[6]);      //if taken without other argvs is placed in 6th position
        k = 4;
        L = 5;
        N = 1;
        R = 3;
    }

    int w = 400;
    
    ifstream countfile(argv[2]);
    int file_lines = count(istreambuf_iterator<char>(countfile), istreambuf_iterator<char>(), '\n');
    int tableSize = file_lines/16;
    
    struct timeval start, end;

    string line;
    int element;
    vector<data*> dataList;//LIST WITH ALL THE POINTS DATA

    LSH* obj = new LSH[L];

    for(int i = 0; i < L; i++)//LSH CONSTRUCTORS
        obj[i] = LSH(k, w, tableSize);   

    ////////////////////// filling the hashtables ///////////////////////
    int flag = 0;
    while(getline(infile, line)){//FOR EACH INPUT POINT
        data* p = new data();
        stringstream ss(line);
        
        ss >> element;
        p->ID = element;//POINT ID
        while( ss >> element )//SPLIT THE LINE AND GET THE ELEMENTS OF VECTORS
            p->p_data.push_back(element);
        
        p->d = p->p_data.size();
        
        dataList.push_back(p);//PUSH POINT IN DATALIST

        if(flag == 0){//CALCULATE ALL THE V FOR EACH H FUNCTION ONLY ONCE
            for(int i = 0; i < L; i++)
                obj[i].v_calculator(p->d);
            flag = 1;
        }
       
        for(int i = 0; i < L; i++){//FOR EACH HASH TABLE PUSH THE POINT
            obj[i].hashPush(dataList[dataList.size() - 1]);
        }
    }

    //////////////////////////////////////////////////////////////////

    string lineq;
    int elementq;
    
    while(getline(infileq, lineq)){//FOR EACH QUERY POINT
        vector<int> q_data;
        stringstream ss(lineq);
        
        ss >> elementq;
        int q_id = elementq;//QUERY ID
        while( ss >> elementq )//SPLIT THE LINE AND GET ALL THE ELEMENTS
            q_data.push_back(elementq);


        int d = q_data.size();
  
        //////////////////////////// finding the kNN /////////////////////
        
        vector<vector<data>> knn_arr;
        data kNNarr[N*L];
        
        gettimeofday(&start, NULL);
        ios_base::sync_with_stdio(false);

        for(int i = 0; i < L; i++)// FROM EACH HASH TABLE TAKE N NEAREST NEIGHBORS
            knn_arr.push_back(obj[i].kNNsearch(q_data, d, N));
        
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

        float distances[dataList.size()];//ARRAY FOR TRUE DISTANCE
        for(int i = 0; i < dataList.size(); i++)//FOR EACH POINT GET TRUE DISTANCE
            distances[i] = distance(dataList[i]->p_data, q_data, dataList[i]->d);

        gettimeofday(&end, NULL);

        double exec_time_2;
  
        exec_time_2 = (end.tv_sec - start.tv_sec) * 1e6;
        exec_time_2 = (exec_time_2 + (end.tv_usec - start.tv_usec)) * 1e-6;

        int n = sizeof(distances) / sizeof(distances[0]);
        sort(distances, distances + n);
        
        int prev = kNNarr[0].ID;
       
        outfile << "Query: " << q_id << endl << "Nearest neighbor-1: " << kNNarr[0].ID << endl;
        outfile << "distanceLSH: " << kNNarr[0].current_distance << endl;
        outfile << "distanceTrue: " << distances[0] << endl << endl;

        int cnt = 1;
        int j;
        for(j = 1; j < knn_size; j++){
            if(cnt == N)
                break;

            if(kNNarr[j].ID == prev)//IF THE POINT ALREADY EXIST IN N NEAREST NEIGHBOR ARRAY CONTINUE
                continue;

            prev = kNNarr[j].ID;

            outfile << "Nearest neighbor-" << cnt + 1 << ": " << kNNarr[j].ID << endl;
            outfile << "distanceLSH: " << kNNarr[j].current_distance << endl;
            outfile << "distanceTrue: " << distances[cnt] << endl << endl;

            cnt++;             
        }

        outfile << "tLSH: " << exec_time << endl;
        outfile << "tTrue: " <<exec_time_2 << endl << endl;

        //////////////////////////////// range-search /////////////////////////////////

       /* vector<vector<data*>> R_arr;//RANGE_POINTS ARRAY
        vector<data*> Rarr;
        
        for(int i = 0; i < L; i++)//FROM EACH HASH TABLE TAKE ALL THE POINTS WITH DISTANCE R
            R_arr.push_back(obj[i].range_search(q_data, d, 0, R));
        
        
        for(auto k: R_arr){
            for(auto l: k){
                Rarr.push_back(l);
            }
        }

        if(Rarr.size() == 0)
            continue;
        
        sort(Rarr.begin(), Rarr.end(), compare_ID_pointer);
        
        prev = Rarr.front()->ID;
        
        outfile << "R-near neighbors: " << endl << Rarr.front()->ID << endl;

        for(auto v: Rarr){
            
            if(v->ID == prev)// IF THE POINT ALREADY EXIST IN RANGE_POINTS ARRAY CONTINUE
                continue;

            prev = v->ID;
            outfile << v->ID << endl;
        }*/

        outfile << endl;
    }

    outfile.close(); 
        
    return 0;
}