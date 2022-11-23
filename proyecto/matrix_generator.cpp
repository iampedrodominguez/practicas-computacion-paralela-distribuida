#include <iostream>
#include <random>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("Need the size of the matrix\n");
        return 1;
    }
    int n = atoi(argv[1]);
    int max = 100;
    int min = 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    ofstream file("Data/matrix_random.txt");
    if (file.is_open()) {
        file << n << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if(i == j)
                    file << -1 << " ";
                else
                    file << dis(gen) << " ";
            }
            file << endl;
        }
    }else{
        printf("Cannot open file %s\n", "Data/matrix_random.txt");
        return 1;
    }
    return 0;
}