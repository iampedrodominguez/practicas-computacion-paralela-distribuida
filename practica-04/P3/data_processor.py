import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from os import path
from matplotlib.ticker import ScalarFormatter


p_min = 0
p_max = 7
n = 10

P = [2**i for i in range(p_min, p_max+1)]

t = []

for p in P:
    path_str = 'Data/data' + str(p) + '.csv'
    if not path.exists(path_str):
        continue
    data = pd.read_csv(path_str, sep=',', header=None)
    avg = data.mean(axis=0)
    t.append(avg[0])



# Plot data
fig, ax = plt.subplots()
# format of the x axis (log scale) to integers
#ax.xaxis.set_major_formatter(ScalarFormatter())
ax.set_xscale('log', base=2)

formatter = ScalarFormatter()
formatter.set_scientific(False)
ax.xaxis.set_major_formatter(formatter)

plt.title('Processors vs Time para N = 10^6')
plt.xlabel('Processors')
plt.ylabel('Time (s)')
plt.plot(P, t, 'o-')
plt.savefig('plot.png')

# Clean plot
plt.clf()

# 

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