import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from os import path

BLACK = '#000000'
BLUE = '#3264B0'
GREEN = '#197D30'
RED = '#AF3230'

p_min = 1
p_max = 5

N_constant = 1024

P = [2 ** i for i in range(p_min, p_max + 1)]

t_comm = []
t_comp = []
t_total = []

comm_error = 0
comp_error = 0
total_error = 0
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
    comm_error = np.std(data.iloc[:,0])
    t_comp.append(avg[1])
    comp_error = np.std(data.iloc[:,1])
    t_total.append(avg[2])
    total_error = np.std(data.iloc[:,2])

# Plot data
plt.title('Processors vs Time: N = 1024', fontweight = 'bold')

plt.plot(P, t_comm, 'o-', color = BLUE, label = 'Comunication time', linewidth = 3)
plt.errorbar(P, t_comm, color = BLACK, yerr = comm_error, capsize=4, fmt='o')

plt.plot(P, t_comp, 'o-', color = GREEN, label = 'Computation time', linewidth = 3)
plt.errorbar(P, t_comp, color = BLACK, yerr = comp_error, capsize=4, fmt='o')

plt.plot(P, t_total, 'o-', color = RED, label = 'Total time', linewidth = 3)
plt.errorbar(P, t_total, color = BLACK, yerr = total_error, capsize=4, fmt='o')

plt.legend()
plt.xlabel('Processors', fontweight = 'bold')
plt.ylabel('Time (ms)', fontweight = 'bold')
plt.savefig('a_mpi_graph_execution_vs_processors.png', dpi=200)

# Clean plot
plt.clf()
