#include "clustering.hpp"
#include <fstream>
#include <sstream>

int main(int argc, char *argv[]){
    
    ifstream infile(argv[2]);
    ifstream configfile(argv[4]);
    ofstream outfile(argv[6]);

    int K;
    int k_LSH;
    int k_HYPER;
    int L;
    int M;
    int probes; 

    string method;
    bool option;
    if(argc == 11){             //with optional included in command
        option = 1;
        method = argv[10];
    }
    else{
        option = 0;
        method = argv[8];
    }

    int w = 400;
    int R = 10000;
    
    ifstream countfile(argv[2]);
    int file_lines = count(istreambuf_iterator<char>(countfile), istreambuf_iterator<char>(), '\n');
    int tableSize = file_lines/16;

    struct timeval start, end; 

    /////////////////////////// LSH and HYPER //////////////////////////  
    
    string line2;
    string val;
    int value;

    int i = 0;
    while(getline(configfile, line2)){//GET VALUES FROM CONFIGURATION FILE 
        stringstream ss(line2);
        
        ss >> val;
        ss >> value;

        if(i == 0)
            K = value;
        else if(i == 1)
            L = value;
        else if(i == 2)
            k_LSH = value;
        else if(i == 3)
            M = value;
        else if(i == 4)
            k_HYPER = value;
        else
            probes = value;

        i++;
        
    }

    LSH* objLSH = new LSH[L];//L OBJECTS FOR LSH

    for(int i = 0; i < L; i++)
        objLSH[i] = LSH(k_LSH, w, tableSize);

    HyperCube objHYPER = HyperCube(k_HYPER, w);//HYPERCUBE OBJECT

    srand(time(NULL));

    ////////////////////// CLUSTERING ///////////////////////////////

    string line;
    int element;
    vector<data*> dataList;

    Clustering obj = Clustering(K);  //CLUSTERING OBJECT

    ////////////////////// filling the tables ///////////////////////
    int flag = 0;
    while(getline(infile, line)){//FOR EACH POINT
        data* p = new data();
        stringstream ss(line);
         
        ss >> element;//POINT ID
        p->ID = element;
        while( ss >> element )//SPLIT THE LINE AND GET THE ELEMENTS OF VECTORS
            p->p_data.push_back(element);
           
        p->d = p->p_data.size();
            
        dataList.push_back(p);//PUSH THE POINT DATA IN DATALIST

        obj.init_Push(p);//INITIALIZE CLUSTER

        
        if(flag == 0){
            for(int i = 0; i < L; i++)
                objLSH[i].v_calculator(p->d);
            objHYPER.v_calculator(p->d);
            flag = 1;
        }

        for(int i = 0; i < L; i++)//PUSH THE POINT IN HASTABLE FOR LSH
            objLSH[i].hashPush(dataList[dataList.size() - 1]);
            
        objHYPER.hashPush(dataList[dataList.size() - 1]);//PUSH POINT IN HASHTABLE FOR HYPERCUBE

    }
    
    obj.k_means(dataList.front()->d, dataList.size(), dataList);//KMEANS

    if (method == "Classic"){//CLASSIC MATHOD(LLOYDS)
        gettimeofday(&start, NULL);
        ios_base::sync_with_stdio(false);

        obj.Lloyds(dataList.front()->d, dataList);

        gettimeofday(&end, NULL);

        outfile << "Algorithm: Lloyds" << endl << endl;
    }
    else if(method == "LSH"){//LSH METHOD
        gettimeofday(&start, NULL);
        ios_base::sync_with_stdio(false);

        obj.reverse_range_search_LSH(objLSH, L, R, dataList);
        outfile << "Algorithm: Range Search LSH" << endl << endl;

        gettimeofday(&end, NULL);
    }
    else if(method == "Hypercube"){//HYPERCUBE METHOD
        gettimeofday(&start, NULL);
        ios_base::sync_with_stdio(false);

        obj.reverse_range_search_HYPER(objHYPER, R, dataList, M, probes);
        outfile << "Algorithm: Range Search Hypercube" << endl << endl;

        gettimeofday(&end, NULL);
    }

    obj.output(outfile, dataList.front()->d);

    double exec_time;
  
    exec_time = (end.tv_sec - start.tv_sec) * 1e6;
    exec_time = (exec_time + (end.tv_usec - start.tv_usec)) * 1e-6;

    outfile << "clustering_time: " << exec_time << endl << endl;
    
    obj.silhouttes(outfile);  

    if(option == 1)//OPTIONAL OUTPUT 
        obj.optional(outfile, dataList.front()->d);
        
}