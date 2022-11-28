#include <iostream>
#include <vector>

#define INF INT_MAX
using namespace std;

#define N 10
typedef vector<vector<int>> Graph;

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

int main(){

    const int n = 10;
    int* CPUcost = new int[n];
    int* GPUcost = new int[n];

    //Allocatig GPU memory for matrix
   
    int* CPUgraph = new int[n * n];
    int* GPUgraph = new int[n * n];


    for (int i = 0 ; i < n; i++) {
        CPUcost[i] = 0;
        for (int j = 0; j < n; j++)
            CPUgraph[i * n + j] = 10;
    }

    size_t bytes_m = n * n * sizeof(int);
    size_t bytes_c = n * sizeof(int);

    cudaMalloc((void**)&GPUgraph, bytes_m);

    //Allocating GPU memory for global variable COST
    cudaMalloc((void**)&GPUcost, bytes_c);

    //Sending data to GPU
    cudaMemcpy(GPUgraph, CPUgraph, bytes_m, cudaMemcpyHostToDevice); 
    cudaMemcpy(GPUcost, CPUcost, bytes_c, cudaMemcpyHostToDevice); 

    //Declare dimensions of GPU grid
    dim3 BLOCKS(n, n);

    //Call reduce functions in GPU
    reduceGraphRows<<<1, BLOCKS>>>(GPUgraph, n, GPUcost);
    reduceGraphColumns<<<1, BLOCKS>>>(GPUgraph, n, GPUcost);

    //Return matrix
    cudaMemcpy(CPUgraph, GPUgraph, bytes_m, cudaMemcpyDeviceToHost);
    cudaMemcpy(CPUcost, GPUcost, bytes_c, cudaMemcpyDeviceToHost);

    int globalCost = 0;

    for (int i = 0 ; i < n; i++) {
        globalCost += CPUcost[i];
        for (int j = 0; j < n; j++)
            cout << CPUgraph[i * n + j] << " ";
        cout << endl;
    }

    
    cout << globalCost;
    //Free memory from GPU
    cudaFree(GPUgraph);
    cudaFree(GPUcost);

    return 0;
}