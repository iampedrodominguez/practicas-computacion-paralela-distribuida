import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from os import path

BLACK = '#000000'
BLUE = '#3264B0'

t_min = 1
t_max = 5

N_constant = 1024

T = [2 ** i for i in range(t_min, t_max + 1)]

t_comp = []

comp_error = 0
data_dir = input("Dir: ")
for t in T:
    path_str = f'{data_dir}/data_{N_constant}_{t}.csv'
    if not path.exists(path_str):
        continue
    # Read data from file
    data = pd.read_csv(path_str, sep=',', header=None)
    # Get average of each column
    avg = data.mean(axis=0)
    comp_error = np.std(data.iloc[:,0])
    avg = np.array(avg)

    # Get averages
    t_comp.append(avg[0])

# Plot data
plt.title('Threads vs Time: N = 1024', fontweight = 'bold')
plt.plot(T, t_comp, 'o-', color = BLUE, label = 'Computation time', linewidth = 3)
plt.errorbar(T, t_comp, color = BLACK, yerr = comp_error, capsize=4, fmt='o')
plt.legend()
plt.xlabel('Threads', fontweight = 'bold')
plt.ylabel('Time (ms)', fontweight = 'bold')
plt.savefig('b_omp_graph_execution_vs_threads.png', dpi=200)

# Clean plot
plt.clf()
