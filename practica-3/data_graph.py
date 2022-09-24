import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from os import path

P = [i for i in range(1,17)]

data = pd.read_csv('p2-b.csv', sep=',').to_numpy()
data = data[:,1:]

prom = []
for list_ in data:
    prom.append(np.mean(list_))

# Plot data
plt.title('Processors vs Time para N = 1024')
plt.plot(P, prom, label='Latency time')
plt.legend()
plt.xlabel('Processors')
plt.ylabel('Time (ms)')
plt.savefig('graphPvsT-2b.png')

