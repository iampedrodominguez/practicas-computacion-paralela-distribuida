# PD 08

## Velocidad de ejecución en Paralelo
Calcule la velocidad de ejecución en paralelo (**en FLOPs**) para cada caso,
*e.g.* contabilizando las operaciones de coma flotante y dividiéndolas entre
el tiempo:

```c++
// NOTE: 5 floating point operations
  double f(double x) {
  double pi = 3.141592653589793;
  double value;

  value = 50.0 / (pi * (2500.0 * x * x + 1.0));

  return value;
}
```
```c++
// NOTE: 10 * n floating point operations
for(i = 0; i < n; i++) {
  // NOTE: 4 floating point operations
  x = ((n - i - 1) * a + (i) * b) / (n - 1);
  // NOTE: 6 floating point operations
  total = total + f(x);
}
```

```c++
// NOTE: 4 floating point operations
total = (b - a) * total / (double) n;
error = fabs(total - exact);
```

Con lo presentado anteriormente tendríamos que el número de operaciones
en secuencial del código presentado es de `10 * N + 4` **FLOPs**.

### Paralelo

Teniendo en cuenta lo anteriormente presentado, podemos defir que, la velocidad
de ejecución en paralelo del código presentado es de `10 * N / P + 4` **FLOPs**
entre el tiempo de ejecución; donde `P` es el número de *threads* en las que se
ejecuta el código. Con lo presentado anteriormente tendríamos que la velocidad de
ejecución en paralelo del código presentado es de `(10 * N / P + 4) / t_exec` **FLOPs/s**.
Entonces se presenta la siguiente tabla de resultados variando P:

## Principio de isoeficiencia

Como se puede deducir del código secuencial, $T_s = O(n)$. En cuanto al código en paralelo, en OMP $T_p = O(\frac{n}{p})$, mientras que en MPI $T_p = O(\frac{n}{p} + log(p))$, esto debido al tiempo de comunicación necesario en MPI. Por lo tanto, teóricamente, en OMP, $S=O(\frac{n}{\frac{n}{p}}) = O(p)$, por lo que la eficiencia $E=O(1)$ sería ideal. Por otro lado, en MPI, $S=O(\frac{n}{\frac{n}{p}+log(p)})$, y su eficiencia $E=O(\frac{1}{1 + \frac{plog(p)}{n}})$, lo que nos indica que, para obtener la eficiencia ideal, $n = O(plog(p))$.

## Gráficas de Velocidad de ejecución en Paralelo
Presente los resultados en sendos graficos **t vs np**, **s vs np**.