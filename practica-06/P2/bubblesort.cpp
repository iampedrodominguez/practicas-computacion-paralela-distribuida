#include  <iostream>
using namespace std;

#define LIMIT 1000

void  imprimir_array(int A [], int N);
void  Sort(int A [], int N);
void  Swap(int* X, int* Y);
void  leer_array(int array[], int* limite, char* filename);

int numeros[LIMIT]; // tamanho maximo del array
int size = LIMIT;  

int main  (int argc, char* argv[])
  {
  if (argc < 2) { // no hay archivo => exit
    exit(1);
    }

  // se leen los numeros de un archivo
  leer_array(numeros, &size, argv[1]); 
  // se imprime el array
  imprimir_array(numeros, size);
  // se ordena el  array
  Sort(numeros, size);
  // se imprime el array ordenado 
  printf ("\nArray ordenado:\n");
  imprimir_array(numeros, size);
  
  return  0;
  }

void leer_array(int array[], int* size, char* filename)
  {
    FILE* opened;  // file
    char buf[30];
    int  numero; // numero de elementos leidos
    int  counter = 0; // counter for elems in file

  opened = fopen(filename, "r"); 
  
  while (!feof(opened) && counter < *size) 
    { // lee hasta EOF o muchos elementos
      fgets(buf, 30, opened); // lee una linea, max 30 caracteres
      numero = atoi(buf); // convierte a entero
      array[counter++] = numero; // dar valores al array
      if ( counter >= *size) // muchos numeros para el array
	cout<<"...muchos numeros para el array\n";
    }
  fclose(opened);

  *size = --counter; // ajusta el tamaño
  cout<<*size<<" numeros leidos"<<endl;
  }

void imprimir_array(int A [], int N)
  {
   int  i;
   cout<<"[";
   for  (i = 0;  i < N;  i ++)
     cout<<A[i]<<" ";
   cout<<"]\n";
  }

void Sort(int A [], int N)
  {
   int i, j;

   for(i=0; i<N-1; i++)
     for(j=0; j<N-i-1; j ++)
       if(A[j] > A[j+1])           
           Swap(&A[j],&A[j+1]);
  }

void Swap(int* X, int* Y)
  {
   int temp=*X;
   *X = * Y;
   *Y = temp;
  }

