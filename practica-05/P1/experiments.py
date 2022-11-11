# read Data/. files and plot
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

valmin = 200
valmax = 1000
valstep = 200

experiments = [i for i in range(valmin, valmax + valstep, valstep)]
probabilities = [0 for i in range(valmin, valmax + valstep, valstep)]
# read data
for i, experiment in enumerate(experiments):
    data = pd.read_csv(f"Data/exp_{experiment}.csv", header=None)
    data = data.to_numpy().flatten()
    probabilities[i] = np.mean(data)

plt.plot(experiments, probabilities)
plt.title("Success probability in each experiment")
plt.xlabel("Experiment")
plt.ylabel("Success probability")
plt.axhline(y=0.00001, color='r', linestyle='-')
plt.savefig("experiments.png")
