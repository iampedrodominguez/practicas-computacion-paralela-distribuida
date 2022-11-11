# Pregunta 2

## Análisis

En el archivo p2.cpp se realizó la implementación del algoritmo de ordenación Bubblesort en paralelo (se divide el array en p grupos, se ordena cada uno con el Bubblesort, y se realiza un merge entre los subarrays para obtener el array ordenado), y el algoritmo Odd-Even Sort en paralelo. Ambos algoritmos se implementaron con OpenMP.

Para el primer algoritmo, dividimos el array en p partes, y ordenamos cada uno en un proceso con ayuda de SortHelper, el cual ordena el array con el algoritmo Bubblesort dados los índices de inicio y fin. Luego, se realiza un merge entre los subarrays, y se copia el array al array original. Para el segundo algoritmo, el array se divide en pares y se pasan a los procesos para que ordenen los pares.

Si ejecutamos el archivo debug.bash, veremos en consola el array original, los arrays resultantes de los ordenamientos en paralelo (además del secuencial) y el tiempo de ejecución de cada uno. También se verifica si los array son iguales.

## Resultados

Observación: El tiempo secuencial de n = 2^20 fue calculado a partir de los anteriores (aproximadamente por 4 debido a la complejidad O(n^2) del algoritmo) debido al elevado tiempo que requería. También se puede observar el comportamiento de los tiempos respecto al número de procesadores y a la cantidad de datos. Cuando duplicamos el número de datos, el tiempo se multiplica por 4 aproximadamente, debido a la complejidad de la ordenación O(n^2). Por otro lado, al duplicar el número de procesadores, el tiempo del Bubblesort en paralelo se divide entre 4 por la complejidad O(n^2/p^2) de la ordenación de cada subarray, mientras que el tiempo de Odd-Even solo se divide entre 2 aproximadamente, limitado por la complejidad O(n/p lg(n/p)) del algoritmo y la comunicación entre procesos.

Se generan las gráficas times.png y speedup.png a partir de los experimentos usados. Como se observa en times.png, el algoritmo de ordenación Bubblesort en paralelo (línea sólida) es más rápido que el de Odd-Even (líneas punteadas) para un n constante. Conforme aumentamos el valor de n, el valor óptimo de la cantidad de procesos a usar incrementa. Sin embargo, se puede notar que este valor es mayor para una misma cantidad de elementos en el algoritmo de Bubblesort en paralelo.

En cuanto al Speedup, se refleja el comportamiento observado en times.png. El algoritmo de Bubblesort en paralelo (línea sólida) se acerca más al Speedup ideal que el de Odd-Even (líneas punteadas). Y para cantidades bajas de elementos en el array, ambos algoritmos no son escalables. Sin embargo, el Bubblesort en paralelo es más escalable que el Odd-Even para valores elevados de la cantidad de elementos n, ya que se acerca mejor a la curva ideal.

