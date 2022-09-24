## Ejercicio N°1

Para este experimento se promediaron 10 ejecuciones del archivo `p1.cpp`y para cada set se modificó el número de procesadores en el rango de 2 a 16.

Como se observa en la gráfica, el tiempo de comunicación entre procesos aumenta a partir de p > 2. A partir de este hito, los tiempos de comunicación fluctúan y jamás alcanzan un valor menor a 2.

## Ejercicio N°2

Observamos que ambas gráficas tienen tiempos promedios bajos para representar a la latencia, entre 0.02 y 0.10 ms. Sin embargo, este tiempo es menor y más uniforme en la comunicación no bloqueada, lo que nos demuestra que representa mejor al tiempo de latencia debido a que no hay bloqueos y, por lo tanto, el tiempo de ejecución del programa representa mejor a la latencia. 

La diferencia entre los tiempos promedio de latencia calculados a partir de las gráficas sería 0.08 ms en la comunicación bloqueada y 0.02 ms en la no bloqueada, lo que da una diferencia de 0.06 ms.


## Ejercicio N°3

Tomando en cuenta que la eficiencia óptima es 1, podemos observar que mientras más procesadores la eficiencia converge a 0 esto debido al alto tiempo de comunicación entre los procesos y el master (proceso 0) para obtener el resultado deseado. Se experimentó con N potencias de 2 para lograr una mejor división y con valores como [128, 256, 512, 1024]. En todos, el valor de eficiencia decae casi en la misma proporción.

Con un N constante podemos ver que los tiempos de comunicación crecen a medida que el número de procesadores aumenta, esto debido a que se tendrá que hacer el Scatter y Broadcast a más procesos y a su vez el número de Gather también aumenta. Sin embargo, podemos observar que el tiempo de computo decrece esto en relación al número de filas que procesa cada uno. En general, el tiempo total crece con el número de procesadores y no podemos ver un cruce entre el tiempo de comunicacion y cómputo; por lo que, podemos decir que el "p" óptimo es menor a 2. Es decir, en este caso el caso secuencial es mucho más rápido que con programación paralela.