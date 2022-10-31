import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.ticker import ScalarFormatter

# Importing times without header
times = pd.read_csv('times.csv', header=None)

# size at the first column, unique values
sizes = times[0].unique().tolist()

# processes at the second column, unique values
processes = times[1].unique().tolist()

times_dict = dict()

# times plot
for size in sizes:
    # filter by size
    filtered = times[times[0] == size]

    times_dict[size] = dict()

    times_dict[size]['sequential'] = np.mean(filtered[2].tolist())
    times_dict[size]['parallel'] = filtered[3].tolist()
    times_dict[size]['odd_even'] = filtered[4].tolist()

# times plot, one palette of colors for each size
colors = plt.cm.tab20(np.linspace(0, 1, len(sizes)))

exp = 2
for size in sizes:
    plt.plot(processes, times_dict[size]['parallel'], color=colors[sizes.index(size)], label='Parallel (n = 2^{})'.format(exp))
    plt.plot(processes, times_dict[size]['odd_even'], color=colors[sizes.index(size)], linestyle='--', label='Odd-Even (n = 2^{})'.format(exp))
    exp += 1

# legend transparent
plt.legend(ncol=4, loc='upper right', prop={'size': 6}, framealpha=0.1)
plt.title('Times')
plt.xlabel('Processes')
plt.ylabel('Time (s)')
plt.xscale('log', base=2)
plt.yscale('log', base=2)
plt.ylim(0.0001, 10000)
plt.gca().xaxis.set_major_formatter(ScalarFormatter())
plt.gca().yaxis.set_major_formatter(ScalarFormatter())
plt.grid()
plt.savefig('times.png')
plt.clf()

# speedup plot
exp = 2
for size in sizes:
    sequential_time = times_dict[size]['sequential']
    times_dict[size]['speedup_parallel'] = [sequential_time / time for time in times_dict[size]['parallel']]
    times_dict[size]['speedup_odd_even'] = [sequential_time / time for time in times_dict[size]['odd_even']]
    plt.plot(processes, times_dict[size]['speedup_parallel'], color=colors[sizes.index(size)], label='Parallel (n = 2^{})'.format(exp))
    plt.plot(processes, times_dict[size]['speedup_odd_even'], color=colors[sizes.index(size)], linestyle='--', label='Odd-Even (n = 2^{})'.format(exp))
    exp += 1

plt.legend(ncol=4, loc='upper right', prop={'size': 6}, framealpha=0.1)
plt.title('Speedup')
plt.xlabel('Processes')
plt.ylabel('Speedup')
plt.xscale('log', base=2)
plt.yscale('log', base=2)
plt.ylim(0.1, 50000)
plt.gca().xaxis.set_major_formatter(ScalarFormatter())
plt.gca().yaxis.set_major_formatter(ScalarFormatter())
plt.grid()
plt.savefig('speedup.png')
plt.clf()
