import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from os import path

p_min = 1
p_max = 5

N_constant = 1024

P = [2 ** i for i in range(p_min, p_max + 1)]

t_comm = []
t_comp = []
t_total = []

data_dir = input("Dir: ")
for p in P:
    path_str = f'{data_dir}/data_{N_constant}_{p}.csv'
    if not path.exists(path_str):
        continue
    # Read data from file
    data = pd.read_csv(path_str, sep=',', header=None)
    # Get average of each column
    avg = data.mean(axis=0)
    avg = np.array(avg)

    # Get averages
    t_comm.append(avg[0])
    t_comp.append(avg[1])
    t_total.append(avg[2])

# Plot data
plt.title('Processors vs Time: N = 1024', fontweight = 'bold')
plt.plot(P, t_comm, 'o--', label = 'Comunication time')
plt.plot(P, t_comp, 'o--', label = 'Computation time')
plt.plot(P, t_total, 'o--', label = 'Total time')
plt.legend()
plt.xlabel('Processors', fontweight = 'bold')
plt.ylabel('Time (ms)', fontweight = 'bold')
plt.savefig('mpi_graph_execution_vs_processors.png', dpi=1000)

# Clean plot
plt.clf()
