#include "tsp_serial.cpp"
#include <omp.h>

using namespace std;

void test(string filename);
pair<int, pair<double, int>>  test2(string filename);
void test3(string filename);

int main(int argc, char *argv[])
{
#ifdef _DEBUG
    for(int i=1; i<=6; i++) test("Test/test" + to_string(i) + ".txt");
#else

#ifdef _DEBUG_BIG
    test3("Data/points_random.txt");
#else
    if(argc < 3)
    {
        printf("In file and Out file are needed\n");
        return 1;
    }
    string inFile = argv[1];
    string outFile = argv[2];
    auto result = test2(inFile);
    int n = result.first;
    double time = result.second.first;
    int score = result.second.second;

    //Write to file
    ofstream file(outFile, ios::app);
    if (file.is_open()) {
        file << "ser," << n << ",1," << time << "," << score << "\n";
    }else{
        printf("Cannot open file %s\n", outFile);
        return 1;
    }
#endif
#endif
    return 0;
}

void test(string filename){
    int n, val;
    Graph graph;

    // Read from matrix_random.txt
    ifstream file(filename);
    if (file.is_open()) {
        file >> n;
        graph.resize(n);
        for (int i = 0; i < n; i++) {
            graph[i].resize(n);
            for (int j = 0; j < n; j++) {
                file >> val;
                if(val == -1) 
                    graph[i][j] = INF;
                else 
                    graph[i][j] = val;
            }
        }
    }else{
        printf("Cannot open file %s\n", filename);
        return;
    }
    
    TSP tsp(n, graph);
    //tsp.printGraph();
    auto t1 = omp_get_wtime();
    tsp.solve();
    auto t2 = omp_get_wtime();
    tsp.printSolution();
    printf("Time in sec: %f\n", t2-t1);
}

pair<int, pair<double, int>> test2(string filename)
{
    int n, val;
    Graph graph;
    
    ifstream file(filename);
    int id;
    vector<double> xs, ys;
    if (file.is_open()) {
        file >> n;
        graph.resize(n);
        for (int i = 0; i < n; i++)
            graph[i].resize(n, INF);
        xs.resize(n);
        ys.resize(n);
        for (int i = 0; i < n; i++)
            file >> id >> xs[i] >> ys[i];
    }else{
        printf("Cannot open file %s\n", filename);
        return {0, {0.0, 0}};
    }
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            double dx = xs[i] - xs[j];
            double dy = ys[i] - ys[j];
            double dist = sqrt(dx*dx + dy*dy);
            graph[i][j] = (int) dist + 0.5;
            graph[j][i] = (int) dist + 0.5;
        }
    }
    
    TSP tsp(n, graph);
    auto t1 = omp_get_wtime();
    tsp.solve();
    auto t2 = omp_get_wtime();
    printf("Time in sec: %f\n", t2 - t1);

    return {n, {t2 - t1, tsp.getScore()}};
}

void test3(string filename)
{
    int n, val;
    Graph graph;
    
    ifstream file(filename);
    int id;
    vector<double> xs, ys;
    if (file.is_open()) {
        file >> n;
        graph.resize(n);
        for (int i = 0; i < n; i++)
            graph[i].resize(n, INF);
        xs.resize(n);
        ys.resize(n);
        for (int i = 0; i < n; i++)
            file >> id >> xs[i] >> ys[i];
    }else{
        printf("Cannot open file %s\n", filename);
        return;
    }
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            double dx = xs[i] - xs[j];
            double dy = ys[i] - ys[j];
            double dist = sqrt(dx*dx + dy*dy);
            graph[i][j] = (int) dist + 0.5;
            graph[j][i] = (int) dist + 0.5;
        }
    }
    
    TSP tsp(n, graph);
    auto t1 = omp_get_wtime();
    tsp.solve();
    auto t2 = omp_get_wtime();
    printf("Time in sec: %f\n", t2 - t1);

    string filename_out = filename.substr(0, filename.find_last_of(".")) + "_calc.txt";
    tsp.printSolutionToFile(filename_out);
}