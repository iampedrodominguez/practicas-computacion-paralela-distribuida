#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    //Generate random data and write it to a file
    srand(time(NULL));
    int N = stoi(argv[1]);
    int num;
    int maxNum = N;
    string filename = "Data/data" + to_string(N) + ".txt";
    FILE *f = fopen(filename.c_str(), "w");
    for (int i = 0; i < N; i++)
    {
        num = rand() % maxNum;
        fprintf(f, "%d\n", num);
    }
    fclose(f);

    return 0;
}