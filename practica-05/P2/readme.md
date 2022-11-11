# Pregunta 02: C

## Análisis experimental teórico

En este problema, se hizo la paralelización en los 2 primeros *for* puesto que para el tercero se podía causar overhead.

Dicho esto, el tiempo de ejecución del algoritmo en paralelo es de O(n/p) + O(nlogn/p) + O(n).

Además, respecto a los tipos de ordenamiento realizados en el segundo *for* se adicionó el *heap_sort* y en una misma iteración

se considero el *heap_sort* para los procesos con *rank* par y el sort habitual de stl para los impares.

## Resultados

Se puede observar que, respecto al tiempo de ejecución, tiene una tendencia al aumento hasta los 128 procesos, y a partir de ahi tiene una tendencia a bajar.

Esto concuerda con la complejidad teórica puesto que a mayor cantidad de procesos, el tiempo de ejecución es menor a partir de cierto umbral, el cual es 128.

Respecto a los tiempos de ejecución locales, se puede apreciar que el *heap_sort* presenta un mayor tiempo de ejecución a comparación del *sort* habitual de c++.
