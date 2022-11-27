import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

folder = "Experiments/"
filename = "times.csv"

# Read the data from the file
data = pd.read_csv(folder + 'data.csv', header=None)

# [type, n, p, time, cost]
ns = [i for i in range(15, 29)]
ps = [i for i in range(1, 16)]

t_serial = []
for i in ns:
    t_serial.append(data.loc[(data[0] == 'ser') & (data[1] == i)][3].values[0])

# First is serial
t_omp = []
for i, n in enumerate(ns):
    t_omp.append([t_serial[i]])
    for p in ps:
        if p == 1:
            continue
        t_omp[i].append(data.loc[(data[0] == 'omp') & (data[1] == n) & (data[2] == p)][3].mean())

# Plot times t_omp
for i, n in enumerate(ns):
    plt.plot(ps, t_omp[i], label='n={}'.format(n))
plt.legend(ncol=4, loc='upper left', prop={'size': 7}, framealpha=0.1)
plt.title('OMP Times')
plt.xlabel('Processes')
plt.ylabel('Time (s)')
#plt.xscale('log', base=2)
#plt.yscale('log', base=10)
#plt.xlim(0.5, 130)
#plt.ylim(0.001, 100)
#plt.gca().xaxis.set_major_formatter(plt.ScalarFormatter())
#plt.gca().yaxis.set_major_formatter(plt.ScalarFormatter())
plt.grid()
plt.savefig(folder + 't_omp.png')
plt.clf()

'''
# Plot speedup t_omp
for i, n in enumerate(ns):
    plt.plot(ps, [t_serial[i] / t for t in t_omp[i]], label='n={}'.format(n))
plt.legend(ncol=4, loc='upper left', prop={'size': 7}, framealpha=0.1)
plt.title('OMP Speedup')
plt.xlabel('Processes')
plt.ylabel('Speedup')
#plt.xscale('log', base=2)
#plt.yscale('log', base=10)
#plt.xlim(0.5, 130)
#plt.ylim(0.001, 100)
#plt.gca().xaxis.set_major_formatter(plt.ScalarFormatter())
#plt.gca().yaxis.set_major_formatter(plt.ScalarFormatter())
plt.grid()
plt.savefig(folder + 's_omp.png')
plt.clf()

# Plot efficiency t_omp
for i, n in enumerate(ns):
    plt.plot(ps, [t_serial[i] / (p * t) for p, t in enumerate(t_omp[i])], label='n={}'.format(n))
plt.legend(ncol=4, loc='upper left', prop={'size': 7}, framealpha=0.1)
plt.title('OMP Efficiency')
plt.xlabel('Processes')
plt.ylabel('Efficiency')
#plt.xscale('log', base=2)
#plt.yscale('log', base=10)
#plt.xlim(0.5, 130)
#plt.ylim(0.001, 100)
#plt.gca().xaxis.set_major_formatter(plt.ScalarFormatter())
#plt.gca().yaxis.set_major_formatter(plt.ScalarFormatter())
plt.grid()
plt.savefig(folder + 'e_omp.png')
plt.clf()
'''