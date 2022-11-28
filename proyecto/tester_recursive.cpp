#include "tsp_recursive.cpp"
#include <omp.h>

using namespace std;

void test(string filename);
pair<int, pair<double, int>> test2(string filename, int p);
void test3(string filename);

int main(int argc, char *argv[]) {
#ifdef _DEBUG
    for(int i=1; i<=6; i++) test("Test/test" + to_string(i) + ".txt");
#else

#ifdef _DEBUG_BIG
    test3("Data/points_random.txt");
#else
    if(argc < 3) {
        printf("In file and Out file are needed\n");
        return 1;
    }
    int p = atoi(argv[1]);
    string inFile = argv[2];
    string outFile = argv[3];
    auto result = test2(inFile, p);
    int n = result.first;
    double time = result.second.first;
    int score = result.second.second;

    //Write to file
    ofstream file(outFile, ios::app);
    if(file.is_open()) {
        file << "sec," << n << ",1," << time << "," << score << "\n";
    }
    else {
        printf("Cannot open file %s\n", outFile);
        return 1;
    }
#endif
#endif
    return 0;
}

pair<int, pair<double, int>> test2(string filename, int p) {
    int n, val;
    Graph graph;
    
    ifstream file(filename);
    int id;
    vector<double> xs, ys;
    if(file.is_open()) {
        file >> n;
        graph.resize(n);
        for(int i = 0; i < n; i++)
            graph[i].resize(n, INF);
        xs.resize(n);
        ys.resize(n);
        for (int i = 0; i < n; i++)
            file >> id >> xs[i] >> ys[i];
    }
    else {
        printf("Cannot open file %s\n", filename);
        return {0, {0.0, 0}};
    }
    for(int i=0; i<n; i++) {
        for(int j=i+1; j<n; j++) {
            double dx = xs[i] - xs[j];
            double dy = ys[i] - ys[j];
            double dist = sqrt(dx*dx + dy*dy);
            graph[i][j] = (int) dist + 0.5;
            graph[j][i] = (int) dist + 0.5;
        }
    }
    
    TSP tsp(n, graph, p);
    auto t1 = omp_get_wtime();
    tsp.solve();
    auto t2 = omp_get_wtime();
    printf("Time in sec: %f\n", t2 - t1);
    tsp.print_solution();
    // tsp.print_graph(tsp.graph);

    return {n, {t2 - t1, tsp.get_score()}};
}