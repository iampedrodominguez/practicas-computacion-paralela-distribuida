//omp scan left
#include <iostream>
#include <omp.h>
#include <math.h>

void prefix_sum(int A[], int n){
    int B[n+1][n+1];
    int C[n+1][n+1];
    #pragma omp parallel 
    {    
        #pragma omp for
        for(int i=1; i<=n; i++){
            B[0][i] = A[i-1];
        }

        for(int i=1; i<=(int) log2(n); i++){
            #pragma omp for 
            for(int j=1; j<=n/((int)pow(2,i)); j++){
                B[i][j] = B[i-1][2*j-1] + B[i-1][2*j];
            }
        }
        for(int i=(int) log2(n); i>=0; i--){
            #pragma omp for
            for(int j=1; j<=n/((int)pow(2,i)); j++){
                if(j % 2 == 0){
                    C[i][j] = B[i + 1][j/2];
                }
                else if(j == 1){
                    C[i][j] = B[i][j];
                }else{
                    C[i][j] = B[i+1][(j-1)/2] + B[i][j];
                }
            }
        }
    }
    for(int i=0; i<n; i++){
        std::cout << C[0][i+1] << " ";
    }
    std::cout << "\n";
    
}

int main(){
    int A[] = {1,2,3,4,5,6,7,8,9,10};
    int n = sizeof(A)/sizeof(A[0]);
    prefix_sum(A, n);
    return 0;

}