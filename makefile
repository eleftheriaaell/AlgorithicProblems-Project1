executable1 = lsh
executable2 = cube
executable3 = cluster

all : lsh cube cluster

lsh : data.cpp LSH.cpp LSH_main.cc
	g++ -o lsh data.cpp LSH.cpp LSH_main.cc -O2

cube: data.cpp hypercube.cpp hypercube_main.cc
	g++ -o cube data.cpp hypercube.cpp hypercube_main.cc -O2

cluster: data.cpp LSH.cpp hypercube.cpp clustering.cpp cluster_main.cc
	g++ -o cluster data.cpp LSH.cpp hypercube.cpp clustering.cpp cluster_main.cc -O2

clean:
	rm -f $(executable1) $(executable2) $(executable3)