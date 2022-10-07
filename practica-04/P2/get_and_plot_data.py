import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from os import path
from matplotlib.ticker import ScalarFormatter


n = 7
P = [2**i for i in range(n)]

path_str = "p2.csv"
data = pd.read_csv(path_str, sep=',')
t = data.mean(axis=1)

formatter = ScalarFormatter()
formatter.set_scientific(False)

# Plot data
fig, ax = plt.subplots()
ax.set_xscale('log', base=2)
ax.xaxis.set_major_formatter(formatter)

plt.title('Processors vs Time para N = 10^6')
plt.xlabel('Processors')
plt.ylabel('Time (ms)')
plt.plot(P, t, 'o-')
plt.savefig('time.png')

# Clean plot
plt.clf()

# Speedup
s = [t[0]/t[i] for i in range(1,len(t))]
theoretical_s = [P[i] for i in range(1, len(t))]
P = P[1:]

fig, ax = plt.subplots()
ax.set_xscale('linear')
plt.xticks(np.arange(0,64,10))
plt.yticks(np.arange(0,64,10))
ax.xaxis.set_major_formatter(formatter)

plt.title('Speedup vs Processors para N = 10^6')
plt.xlabel('Processors')
plt.ylabel('Speedup')
plt.plot(P, s, 'o-')
plt.plot(P, theoretical_s, 'o-')
plt.savefig('speedup.png')

# Clean plot
plt.clf()

# Efficiency
e = [s[i]/P[i] for i in range(len(s))]
e_constant = [1 for i in range(len(s))]

fig, ax = plt.subplots()
ax.set_xscale('log', base=2)
ax.xaxis.set_major_formatter(formatter)

plt.title('Efficiency vs Processors para N = 10^6')
plt.xlabel('Processors')
plt.ylabel('Efficiency')
plt.plot(P, e, 'o-')
plt.plot(P, e_constant, 'o-')
plt.savefig('efficiency.png')

# Clean plot
plt.clf()

'''
# Tiempo secuencial
t_seq_by_N = []
for n in N:
    path_str = 'Data/data1.N'+str(n)+'.csv'
    if not path.exists(path_str):
        continue
    # Read data from file
    data = pd.read_csv(path_str, sep=',', header=None)
    # Get average of each column
    avg = data.mean(axis=0)
    # Get averages
    t_seq_by_N.append(avg[2])

# Data
t_total_by_N = []
for n in N:
    times = []
    for p in P:
        path_str = 'Data/data'+str(p)+'.N'+str(n)+'.csv'
        if not path.exists(path_str):
            continue
        # Read data from file
        data = pd.read_csv(path_str, sep=',', header=None)
        # Get average of each column
        avg = data.mean(axis=0)
        # Get averages
        times.append(avg[2])
    t_total_by_N.append(times)

# Speedup
speedup = []
for i in range(len(t_total_by_N)):
    speedup_list = []
    for j in range(len(t_total_by_N[i])):
        speedup_list.append(t_total_by_N[i][j]/t_seq_by_N[i])
    speedup.append(speedup_list)

# Plot data
plt.title('Processors vs Speedup para N = 2^n')
plt.plot(P, speedup[0], label='N = 128')
plt.plot(P, speedup[1], label='N = 256')
plt.plot(P, speedup[2], label='N = 512')
plt.plot(P, speedup[3], label='N = 1024')
plt.legend()
plt.xlabel('Processors')
plt.ylabel('Speedup')
plt.savefig('graphPvsSpeedup.png')

# Clean plot
plt.clf()

# Eficiencia
eficiency = []
for i in range(len(speedup)):
    eficiency_list = []
    for j in range(len(speedup[i])):
        eficiency_list.append(speedup[i][j]/P[j])
    eficiency.append(eficiency_list)

# Plot data
plt.title('Processors vs Eficiency para N = 2^n')
plt.plot(P, eficiency[0], label='N = 128')
plt.plot(P, eficiency[1], label='N = 256')
plt.plot(P, eficiency[2], label='N = 512')
plt.plot(P, eficiency[3], label='N = 1024')
plt.legend()
plt.xlabel('Processors')
plt.ylabel('Eficiency')
plt.savefig('graphPvsEficiency.png')'''