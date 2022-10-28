#include <iostream>

using namespace std;

int main()
{
    //Generate random data and write it to a file
    int N = 32;
    int num;
    FILE *f = fopen("data.txt", "w");
    for (int i = 0; i < N; i++)
    {
        num = rand() % 1000;
        fprintf(f, "%d\n", num);
    }
    fclose(f);

    return 0;
}