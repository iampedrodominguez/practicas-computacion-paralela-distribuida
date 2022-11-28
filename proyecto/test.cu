#include <iostream>
#include <vector>

#define INF INT_MAX
using namespace std;

#define N 10
typedef vector<vector<int>> Graph;

__device__ int cost;

__global__ void test(int graph[], int n)

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
            cost += mn;

            //Reduce the cost for each row element
            for(int j=0; j<n; j++){
                if(graph[idx * n + j] != INF){
                    graph[idx * n + j] -= mn;
                }
            }
        }
    }
}

int main(){

    int* cost;
    const int n = 10;

    //Allocatig GPU memory for matrix
   
    int* CPUgraph = new int[n * n];
    int* GPUgraph = new int[n * n];


    for (int i = 0 ; i < n; i++) {
        for (int j = 0; j < n; j++)
            CPUgraph[i * n + j] = 10;
    }

    size_t bytes_i = n * n * sizeof(int);

    cudaMalloc((void**)&GPUgraph, bytes_i);

    //Allocating GPU memory for global variable COST
    cudaMalloc((void**)&cost, sizeof(int));

    //Pensar como pasarlo a GPU
    cudaMemcpy(GPUgraph, CPUgraph, bytes_i, cudaMemcpyHostToDevice); 

    //Declare dimensions of GPU grid
    dim3 BLOCKS(n, n);

    //Call reduce functions in GPU
    test<<<1, BLOCKS>>>(GPUgraph, n);

    //cout << GPUgraph[0];
    //Return matrix
    if (cudaMemcpy(CPUgraph, GPUgraph, bytes_i, cudaMemcpyDeviceToHost) == cudaSuccess){
        cout << "Se pudo copiar a matriz final\n";
    } 


    for (int i = 0 ; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << CPUgraph[i * n + j] << " ";
        cout << endl;
    }
    //Free memory from GPU
    cudaFree(GPUgraph);
    cudaFree(cost);

    return 0;
}