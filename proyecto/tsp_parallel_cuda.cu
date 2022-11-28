#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <climits>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <omp.h>

using namespace std;

#define INF INT_MAX

typedef vector<vector<int>> Graph;

class Node {
public:
    Graph graph;
    int cost;
    int cost_parent;

    Node() {
        cost = 0;
        cost_parent = 0;
    }
};

class TSP

{
private:
    int n;
    Graph graph;
    Node* root;
    vector<int> path;
    int p;
    void reduceGraph(Node* node);
    void reduce(Node* node, int from, int to);

public:
    TSP(int n, Graph graph, int p);
    void printGraph();
    void solve();
    bool printSolution();
    bool printSolutionToFile(string filename);
    int getScore();
};

TSP::TSP(int n, Graph graph, int p)
{
    this->n = n;
    this->graph = graph;
    this->p = p;
    
    root = new Node;
    root->graph = graph;
    root->cost = 0;
    root->cost_parent = 0;
};


__global__ void reduceGraphColumns(int graph[], int n, int cost[])
{
    //Get thread ID.x
    int idx = threadIdx.x + blockIdx.x * blockDim.x;

    if (idx < n)
    {
        int mn = INF;
        for (int j=0; j<n; j++)
            if (graph[j * n + idx] < mn)
                mn = graph[j * n + idx];

        if(mn != INF && mn > 0)
        {    
            cost[idx] += mn;
            for(int j=0; j<n; j++)

                if(graph[j * n + idx] != INF)
                    graph[j * n + idx] -= mn;
        }
    }
}

__global__ void reduceGraphRows(int graph[], int n, int cost[])

{
    //Get thread ID.x
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    //int idy = threadIdx.y + blockIdx.y * blockDim.y;

    
    if (idx < n) {
        int mn = INF;

        //Find the smallest row value
        for (int j = 0; j < n; j++)
            if (graph[idx * n + j] < mn)
                mn = graph[idx * n + j];
        
        //If small value is valid
        if (mn != INF && mn > 0)
        {   
            
            cost[idx] = mn;

            //Reduce the cost for each row element
            for(int j=0; j<n; j++){
                if(graph[idx * n + j] != INF){
                    graph[idx * n + j] -= mn;
                }
            }
        }
    }
}

void TSP::reduceGraph(Node* node)
{
    int* CPUcost = new int[n];
    int* GPUcost = new int[n];

    //Allocatig GPU memory for matrix
   
    int* CPUgraph = new int[n * n];
    int* GPUgraph = new int[n * n];

    for(int i = 0 ; i < n; i++) {
        CPUcost[i] = 0;
        for (int j = 0; j < n; j++)
            CPUgraph[i * n + j] = node->graph[i][j];
    }

    size_t bytes_m = n * n * sizeof(int);
    size_t bytes_c = n * sizeof(int);

    //Allocating GPU memory for global variable COST and GPUGRAPH
    cudaMalloc((void**)&GPUgraph, bytes_m);
    cudaMalloc((void**)&GPUcost, bytes_c);

    // Copy data to GPU
    cudaMemcpy(GPUgraph, CPUgraph, bytes_m, cudaMemcpyHostToDevice); 
    cudaMemcpy(GPUcost, CPUcost, bytes_c, cudaMemcpyHostToDevice); 

    // Declare dimensions of GPU grid
    dim3 BLOCKS(n, n);

    // Call reduce functions in GPU
    reduceGraphRows<<<1, BLOCKS>>>(GPUgraph, n, GPUcost);
    reduceGraphColumns<<<1, BLOCKS>>>(GPUgraph, n, GPUcost);

    // Copy data from GPU
    cudaMemcpy(CPUgraph, GPUgraph, bytes_m, cudaMemcpyDeviceToHost);
    cudaMemcpy(CPUcost, GPUcost, bytes_c, cudaMemcpyDeviceToHost);

    int globalCost = 0;

    for (int i = 0 ; i < n; i++) {
        globalCost += CPUcost[i];
        for (int j = 0; j < n; j++)
            cout << CPUgraph[i * n + j] << " ";
        cout << endl;
    }

    for(int i = 0 ; i < n; i++) {
        for (int j = 0; j < n; j++)
            node->graph[i][j] = CPUgraph[i * n + j];
    }
    
    node->cost += globalCost;

    // Free memory from GPU and CPU
    free(CPUgraph);
    free(CPUcost);
    cudaFree(GPUgraph);
    cudaFree(GPUcost);
}

void TSP::reduce(Node* node, int from, int to) {
    node->cost += node->graph[from][to] + node->cost_parent;
    for (int i = 0; i < n; i++) {
        node->graph[from][i] = INF;
        node->graph[i][to] = INF;
    }
    node->graph[to][0] = INF;
    reduceGraph(node);
}


void TSP::solve()
{
    path = {0};
    reduceGraph(root);

    auto cmp = [](pair<Node*, vector<int>> a, pair<Node*, vector<int>> b) {
        return a.first->cost > b.first->cost;
    };

    priority_queue<pair<Node*, vector<int>>, vector<pair<Node*, vector<int>>>, decltype(cmp)> pq(cmp);
    pq.push({root, {0}});

    while (!pq.empty()) {
        auto cur = pq.top();
        Node* curnode = cur.first;
        vector<int> curpath = cur.second;
        pq.pop();
        int from = curpath.back();
        if(curpath.size() == n)
        {
            curpath.push_back(0);
            path = curpath;
            break;
        }

#pragma omp parallel shared(pq, curnode, curpath)  num_threads(p)
{
        #pragma omp for 
        for(int to = 0; to < n; to++){
            if(curnode->graph[from][to] != INF)
            {
                //if to is elegible
                Node* child = new Node();
                child->graph = curnode->graph;
                vector<int> childpath = curpath;
                childpath.push_back(to);
                child->cost_parent = curnode->cost;

                reduce(child, from, to);
#pragma omp critical
                pq.push({child, childpath});
            }
        }
#pragma omp barrier
}

        delete curnode;
    }
} 

bool TSP::printSolution()
{
    if(path.size() == 0) {
        printf("Make solve to generate a solution\n");
        return 0;
    }
    int cost = 0;
    int prev = -1;
    printf("Solution: ");
    for (int &i : path) {
        printf("%d ", i);
        if (prev != -1) cost += graph[prev][i];
        prev = i;
    }
    printf("\nCost: %d\n", cost);
    return 1;
}

bool TSP::printSolutionToFile(string filename)
{
    if(!printSolution()) return 0;
    
    int cost = 0;
    int prev = -1;
    for (int &i : path) {
        if (prev != -1) cost += graph[prev][i];
        prev = i;
    }
    
    ofstream file;
    file.open(filename, ios::out);
    if(file.fail()) return 0;
    file << cost << "\n";
    for (int &i : path) {
        file << i + 1 << "\n";
    }
    file.close();
    return 1;
}

void TSP::printGraph()
{
    printf("Graph of size %d:\n", n);
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            printf("%d ", graph[i][j]);
        printf("\n");
    }
    printf("\n");
}

int TSP::getScore()
{
    int cost = 0;
    int prev = -1;
    for (int &i : path) {
        if (prev != -1) cost += graph[prev][i];
        prev = i;
    }
    return cost;
}