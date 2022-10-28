import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from os import path
from matplotlib.ticker import ScalarFormatter


P = [2**i for i in range(12)]

t = [0.0917698,0.1540468, 0.2041118, 0.2419248, 0.2605368, 0.2915368, 0.3254318, 0.5692648 ,0.5563728, 0.5534008, 0.5546748, 0.5529678]

formatter = ScalarFormatter()
formatter.set_scientific(False)

# Plot data
fig, ax = plt.subplots()
ax.set_xscale("log", basex=2)
ax.xaxis.set_major_formatter(formatter)

plt.title('Processors vs Time')
plt.xlabel('Processors')
plt.ylabel('Time (s)')
plt.plot(P, t, 'o-')
plt.savefig('time.png')
