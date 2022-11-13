import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# Read the data from the file
data = pd.read_csv('Data/data.csv', header=None)

# [type, n, p, time, error]
n = [2**i for i in range(20, 31)]
p = [2**i for i in range(0, 8)]

t_serial = []

# get serial times
for i in n:
    t_serial.append(data.loc[(data[0] == 'ser') & (data[1] == i)][3].values[0])

# get parallel times
t_omp = []
for i, n_ in enumerate(n):
    t_omp.append([t_serial[i]])
    for p_ in p:
        if p_ == 1:
            continue
        t_omp[i].append(data.loc[(data[0] == 'omp') & (data[1] == n_) & (data[2] == p_)][3].values[0])

t_mpi = []
for i, n_ in enumerate(n):
    t_mpi.append([t_serial[i]])
    for p_ in p:
        if p_ == 1:
            continue
        t_mpi[i].append(data.loc[(data[0] == 'mpi') & (data[1] == n_) & (data[2] == p_)][3].values[0])

colors = plt.cm.tab20(np.linspace(0, 1, len(n)))

# plot t_omp
for i, n_ in enumerate(n):
    plt.plot(p, t_omp[i], color=colors[i], label='2^{}'.format(i + 20))
plt.legend(ncol=4, loc='upper left', prop={'size': 7}, framealpha=0.1)
plt.title('OMP Times')
plt.xlabel('Processes')
plt.ylabel('Time (s)')
plt.xscale('log', base=2)
plt.yscale('log', base=10)
plt.xlim(0.5, 130)
plt.ylim(0.001, 100)
plt.gca().xaxis.set_major_formatter(plt.ScalarFormatter())
plt.gca().yaxis.set_major_formatter(plt.ScalarFormatter())
plt.grid()
plt.savefig('Plots/t_omp.png')
plt.clf()

# plot t_mpi
for i, n_ in enumerate(n):
    plt.plot(p, t_mpi[i], color=colors[i], label='2^{}'.format(i + 20))
plt.legend(ncol=4, loc='upper left', prop={'size': 7}, framealpha=0.1)
plt.title('MPI Times')
plt.xlabel('Processes')
plt.ylabel('Time (s)')
plt.xscale('log', base=2)
plt.yscale('log', base=10)
plt.xlim(0.5, 130)
plt.ylim(0.001, 100)
plt.gca().xaxis.set_major_formatter(plt.ScalarFormatter())
plt.gca().yaxis.set_major_formatter(plt.ScalarFormatter())
plt.grid()
plt.savefig('Plots/t_mpi.png')
plt.clf()

# plot omp speedup, t_omp / t_serial
for i, n_ in enumerate(n):
    plt.plot(p, [t_omp[i][0] / t_omp[i][j] for j in range(len(p))], color=colors[i], label='2^{}'.format(i + 20))
# Add a line for the ideal speedup
plt.plot(p, p, color='black', linestyle='--', label='Ideal')
plt.legend(ncol=4, loc='upper left', prop={'size': 7}, framealpha=0.1)
plt.title('OMP Speedup')
plt.xlabel('Processes')
plt.ylabel('Speedup')
plt.xscale('log', base=2)
plt.yscale('log', base=10)
plt.xlim(0.5, 256)
plt.ylim(0.5, 50)
plt.gca().xaxis.set_major_formatter(plt.ScalarFormatter())
plt.gca().yaxis.set_major_formatter(plt.ScalarFormatter())
plt.grid()
plt.savefig('Plots/s_omp.png')
plt.clf()

# plot mpi speedup, t_mpi / t_serial
for i, n_ in enumerate(n):
    plt.plot(p, [t_mpi[i][0] / t_mpi[i][j] for j in range(len(p))], color=colors[i], label='2^{}'.format(i + 20))
# Add a line for the ideal speedup
plt.plot(p, p, color='black', linestyle='--', label='Ideal')
plt.legend(ncol=4, loc='upper left', prop={'size': 7}, framealpha=0.1)
plt.title('MPI Speedup')
plt.xlabel('Processes')
plt.ylabel('Speedup')
plt.xscale('log', base=2)
plt.yscale('log', base=10)
plt.xlim(0.5, 256)
plt.ylim(0.5, 50)
plt.gca().xaxis.set_major_formatter(plt.ScalarFormatter())
plt.gca().yaxis.set_major_formatter(plt.ScalarFormatter())
plt.grid()
plt.savefig('Plots/s_mpi.png')
plt.clf()

# plot omp efficiency, speedup / p
for i, n_ in enumerate(n):
    plt.plot(p, [t_omp[i][0] / (t_omp[i][j] * p[j]) for j in range(len(p))], color=colors[i], label='2^{}'.format(i + 20))
# Add a line for the ideal efficiency
plt.plot(p, [1 for p_ in p], color='black', linestyle='--', label='Ideal')
plt.legend(ncol=4, loc='upper left', prop={'size': 7}, framealpha=0.1)
plt.title('OMP Efficiency')
plt.xlabel('Processes')
plt.ylabel('Efficiency')
plt.xscale('log', base=2)
plt.yscale('log', base=10)
plt.xlim(0.5, 256)
plt.ylim(0.1, 2.0)
plt.gca().xaxis.set_major_formatter(plt.ScalarFormatter())
plt.gca().yaxis.set_major_formatter(plt.ScalarFormatter())
plt.grid()
plt.savefig('Plots/e_omp.png')
plt.clf()

# plot mpi efficiency, speedup / p
for i, n_ in enumerate(n):
    plt.plot(p, [t_mpi[i][0] / (t_mpi[i][j] * p[j]) for j in range(len(p))], color=colors[i], label='2^{}'.format(i + 20))
# Add a line for the ideal efficiency
plt.plot(p, [1 for p_ in p], color='black', linestyle='--', label='Ideal')
plt.legend(ncol=4, loc='upper left', prop={'size': 7}, framealpha=0.1)
plt.title('MPI Efficiency')
plt.xlabel('Processes')
plt.ylabel('Efficiency')
plt.xscale('log', base=2)
plt.yscale('log', base=10)
plt.xlim(0.5, 256)
plt.ylim(0.1, 2.0)
plt.gca().xaxis.set_major_formatter(plt.ScalarFormatter())
plt.gca().yaxis.set_major_formatter(plt.ScalarFormatter())
plt.grid()
plt.savefig('Plots/e_mpi.png')
plt.clf()

