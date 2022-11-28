#include <iostream>
#include <random>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    // Verification the input is valid
    if(argc < 2){
        printf("Need the number of points\n");
        return 1;
    }

    int n = atoi(argv[1]);
    
    // Maximum dimensions of the points
    int xy_min = 0;
    int xy_max = 1000;
    int x, y;

    // Set index matrix for points
    vector<vector<int>> points_used(xy_max, vector<int>(xy_max, 0));
    
    // Uniform distribution object
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(xy_min, xy_max);

    // File where points will be stored
    ofstream file("Data/points_random.txt");


    if (file.is_open()) {
        file << n << "\n";
        for (int i = 0; i < n; i++) {
            // Generate 
            do{
                x = (int) dis(gen);
                y = (int) dis(gen);
            }while(points_used[x][y] == 1);
            file << i + 1 << " " << x << " " << y << "\n";
            points_used[x][y] = 1;
        }
    }else{
        printf("Cannot open file %s\n", "Data/points_random.txt");
        return 1;
    }
    return 0;
}