//#include "tsp_serial.cpp"
#include "tsp_parallel.cpp"
#include <omp.h>

using namespace std;

void printTime()
{
    time_t now = time(0);
    char* dt = ctime(&now);
    cout << "The local date and time is: " << dt << endl;
}

void test(string filename);
void test2(string filename);

int main(int argc, char *argv[])
{
    //for(int i=1; i<=6; i++) test("Test/test" + to_string(i) + ".txt");
    
    test2("Data/points_random.txt");

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

void test2(string filename)
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
    //tsp.printGraph();
    auto t1 = omp_get_wtime();
    tsp.solve();
    auto t2 = omp_get_wtime();
    printf("Time in sec: %f\n", t2 - t1);
    //tsp.printSolution();

    string filename_out = filename.substr(0, filename.find_last_of(".")) + "_calc.txt";
    tsp.printSolutionToFile(filename_out);
}