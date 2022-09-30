#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}
int separate(int *x, int low, int high)
{
    int i, pivot, last;
    pivot = x[low];
    swap(x + low, x + high);
    last = low;
    for (i = low; i < high; i++)
    {
        if (x[i] <= pivot)
        {
            swap(x + last, x + i);
            last += 1;
        }
    }
    swap(x + last, x + high);
    return last;
}
void qs(int *z, int zstart, int zend, int firstcall)
{
    {
        int pivot;
        if (firstcall == 1)
        {
            qs(z, 0, zend, 0);
        }
        else
        {
            if (zstart < zend)
            {
                pivot = separate(z, zstart, zend);
                qs(z, zstart, pivot - 1, 0);
                qs(z, pivot + 1, zend, 0);
            }
        }
    }
}
int main(int argc, char **argv)
{
    int i, n, *w;
    n = atoi(argv[1]);
    w = malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
        w[i] = rand() % 100;
    qs(w, 0, n - 1, 1);
    if (n < 25)
        for (i = 0; i < n; i++)
            printf("%d\n", w[i]);
    return 0;
}