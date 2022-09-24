import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from os import path

p_min = 2
p_max = 10

P = [i for i in range(p_min, p_max + 1)]

t_comm = []
t_comp = []
t_total = []

for p in P:
    if not path.exists('Data/data'+str(p)+'.csv'):
        continue
    # Read data from file
    data = pd.read_csv('Data/data'+str(p)+'.csv', sep=',', header=None)
    # Get average of each column
    avg = data.mean(axis=0)
    
    # Get averages
    t_comm.append(avg[0])
    t_comp.append(avg[1])
    t_total.append(avg[2])

# Plot data
plt.title('Processors vs Time')
plt.plot(P, t_comm, label='Comunication time')
plt.plot(P, t_comp, label='Computation time')
plt.plot(P, t_total, label='Total time')
plt.legend()
plt.xlabel('Processors')
plt.ylabel('Time (ms)')
plt.savefig('graph.png')

