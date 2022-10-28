import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from os import path

t_min = 1
t_max = 5

N_constant = 1024

T = [2 ** i for i in range(t_min, t_max + 1)]

t_comp = []

data_dir = input("Dir: ")
for t in T:
    path_str = f'{data_dir}/data_{N_constant}_{t}.csv'
    if not path.exists(path_str):
        continue
    # Read data from file
    data = pd.read_csv(path_str, sep=',', header=None)
    # Get average of each column
    avg = data.mean(axis=0)
    avg = np.array(avg)

    # Get averages
    t_comp.append(avg[0])

# Plot data
plt.title('Threads vs Time: N = 1024', fontweight = 'bold')
plt.plot(T, t_comp, 'o--', label = 'Computation time')
plt.legend()
plt.xlabel('Threads', fontweight = 'bold')
plt.ylabel('Time (ms)', fontweight = 'bold')
plt.savefig('omp_graph_execution_vs_threads.png', dpi=1000)

# Clean plot
plt.clf()
