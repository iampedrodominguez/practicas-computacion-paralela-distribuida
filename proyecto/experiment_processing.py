import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

folder = "Experiments/"
filename = "times.csv"

# Read the data from the file
data = pd.read_csv(folder + filename, header=None)

# [type, n, p, time, cost]
ns = [i for i in range(15, 29)]
ps = [i for i in range(1, 11)]

# Evaluate if for each , column 5 is the same for all
for n in ns:
    cost = data.loc[(data[1] == n) & (data[0] == 'sec')][4].values[0]
    for p in ps:
        if p == 1:
            continue
        if cost != data.loc[(data[0] == 'omp') & (data[1] == n) & (data[2] == p)][4].values[0]:
            print("Error in: n = ", n, " p = ", p)

t_serial = []
for n in ns:
    t_serial.append(data.loc[(data[0] == 'sec') & (data[1] == n)][3].values[0])

# First is serial
t_omp = []
for i, n in enumerate(ns):
    t_omp.append([t_serial[i]])
    for p in ps:
        if p == 1:
            continue
        t_omp[i].append(data.loc[(data[0] == 'omp') & (data[1] == n) & (data[2] == p)][3].mean())

# Colors and shapes
colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']
shapes = ['o', 'v', '^', '<', '>', 's', 'p', '*', 'h', 'H', 'D', 'd', 'P', 'X']

# Plot times t_omp
color = 0
shape = 0
for i, n in enumerate(ns):
    # Add transparency
    plt.plot(ps, t_omp[i], color=colors[color], marker=shapes[shape], label="n = " + str(n), alpha=0.5)
    color += 1
    if color == len(colors):
        color = 0
        shape += 1

plt.legend(ncol=6, loc='upper left', prop={'size': 7}, framealpha=0.1)
plt.title('OMP Times')
plt.xlabel('Processes')
plt.ylabel('Time (s)')
plt.yscale('log', base=10)
#print x limit
#print("x limit: ", plt.xlim())
#print y limit
#print("y limit: ", plt.ylim())
plt.xlim(0.0, 10.5)
plt.ylim(1.5, 2000)
plt.gca().yaxis.set_major_formatter(plt.ScalarFormatter())
plt.grid()
plt.savefig(folder + 't_omp.png')
plt.clf()


# Plot speedup t_omp
color = 0
shape = 0
for i, n in enumerate(ns):
    plt.plot(ps, [t_serial[i]/t for t in t_omp[i]], color=colors[color], marker=shapes[shape], label="n = " + str(n), alpha=0.5)
    color += 1
    if color == len(colors):
        color = 0
        shape += 1
# Add the ideal speedup
plt.plot(ps, ps, color='k', linestyle='--', label="Ideal")

plt.legend(ncol=4, loc='upper left', prop={'size': 7}, framealpha=0.1)
plt.title('OMP Speedup')
plt.xlabel('Processes')
plt.ylabel('Speedup')
#plt.xscale('log', base=2)
plt.yscale('log', base=10)
#print x limit
#print("x limit: ", plt.xlim())
#print y limit
#print("y limit: ", plt.ylim())
plt.xlim(0.0, 10.5)
plt.ylim(0.95, 3)
#plt.gca().xaxis.set_major_formatter(plt.ScalarFormatter())
plt.gca().yaxis.set_major_formatter(plt.ScalarFormatter())
plt.grid()
plt.savefig(folder + 's_omp.png')
plt.clf()

# Plot efficiency t_omp
color = 0
shape = 0
for i, n in enumerate(ns):
    plt.plot(ps, [t_serial[i]/(p*t) for p, t in zip(ps, t_omp[i])], color=colors[color], marker=shapes[shape], label="n = " + str(n), alpha=0.5)
    color += 1
    if color == len(colors):
        color = 0
        shape += 1

plt.plot(ps, [1 for p in ps], color='k', linestyle='--', label="Ideal")

plt.legend(ncol=6, loc='upper left', prop={'size': 7}, framealpha=0.1)
plt.title('OMP Efficiency')
plt.xlabel('Processes')
plt.ylabel('Efficiency')
#plt.xscale('log', base=2)
plt.yscale('log', base=10)
#print x limit
#print("x limit: ", plt.xlim())
#print y limit
#print("y limit: ", plt.ylim())
plt.xlim(0.0, 10.5)
plt.ylim(0.1, 2)
#plt.gca().xaxis.set_major_formatter(plt.ScalarFormatter())
plt.gca().yaxis.set_major_formatter(plt.ScalarFormatter())
plt.grid()
plt.savefig(folder + 'e_omp.png')
plt.clf()