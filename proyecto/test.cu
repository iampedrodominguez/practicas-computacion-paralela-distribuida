#include <iostream>
#include <vector>

#define INF INT_MAX
using namespace std;

typedef vector<vector<int>> Graph;

__device__ int cost;

__global__ void test(Graph graph, int n)

{
    //Get thread ID.x
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    //int idy = threadIdx.y + blockIdx.y * blockDim.y;

    if (idx < n) {
        int mn = INF;

        //Find the smallest row value
        for (int j = 0; j < n; j++)
            if (graph[idx][j] < mn)
                mn = graph[idx][j];
        
        //If small value is valid
        if (mn != INF && mn > 0)
        {   
            cost += mn;

            //Reduce the cost for each row element
            for(int j=0; j<n; j++){
                if(graph[idx][j] != INF){
                    graph[idx][j] -= mn;
                }
            }
        }
    }
}

int main(){

    int* cost;
    int n = 10;

    //Allocatig GPU memory for each matrix
    Graph CPUgraph = vector<vector<int>>(vector<int>(10),10);
    Graph* GPUgraph = new Graph(10,10,0);

    size_t bytes_i = n * n * sizeof(int);
    cudaMalloc((void**)GPUgraph, bytes_i);

    //Allocating GPU memory for global variable cost
    cudaMalloc(&cost, sizeof(int));

    //Pensar como pasarlo a GPU
    cudaMemcpy(GPUgraph, &(CPUgraph), bytes_i, cudaMemcpyHostToDevice); 

    //Declare dimensions of GPU grid
    dim3 BLOCKS(n, n);

    //Call reduce functions in GPU
    test<<<1, BLOCKS>>>(*GPUgraph, n);

    //Return matrix
    if (cudaMemcpy(&(CPUgraph), GPUgraph, bytes_i, cudaMemcpyDeviceToHost) == cudaSuccess){
        printf("Se pudo copiar a imagen final\n");
    } 

    for (auto row : CPUgraph) {
       for (auto col : row)
              cout << col << " ";
       cout << endl;
    }

    //Free memory from GPU
    cudaFree(GPUgraph);
    cudaFree(cost);

    return 0;
}