# Pregunta 03: D

## Ejecución de scripts

Se ejecutaron los scripts *experiment_a.bash*, *experiment_b.bash* y *experiment_c.bash* en el cluster **Khipu** en los 3 casos se pusieron parámetros de hasta 2 ** 5 procesadores o threads. De los cuales se obtuvieron cada una de las carpetas adjuntas de datos presentes. Para cada par de parámetros se realizaron diferentes números de iteraciones dependiendo del tiempo de ejecución para obtener todos los datos necesarios.

Para más referencias, puede revisar los *scripts* utilizados.

## Análisis de resultados

Para el primer caso, de solo utilizar **MPI**, se observa que el tiempo de ejecución decrese conforme el número de procesadores aumenta esto de acuerdo a la teoría ya que el número de filas ejecutadas por procesador en menor. Sin embargo, como se detalló en la práctica anterior el p ideal se presume como $P = 1$. Ya que, no se observa la intersección de los tiempos.

En el segundo caso, al utilizar **OMP**, podemos ver que conforme aumenta el número de threads el tiempo de ejecución disminuye ya que cada *thread* realiza los cálculos de $N / threads$ *rows*. En este caso, podemos ver que la curva es mucho más pronunciada que en el primer caso.

Para el tercer caso, *híbrido*, podemos que el tiempo de cómputo de acuerdo a cada P, decrese; sin embargo, el problema sigue siendo el constante aumento de los tiempos de comunicación que a partir del P = 8 aumentan considerablemente. Además que en los últimos casos de incrementa exponencialmente. Esto posiblemente a que los recursos están siendo explotados por los procesodores y threads.
