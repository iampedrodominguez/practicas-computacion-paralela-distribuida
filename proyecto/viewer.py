import numpy as np
import matplotlib.pyplot as plt

test_name = "wi29"
plot_best = False
plot_calc = True

filename = "Data/" + test_name + ".txt"
filename_best = "Data/" + test_name + "_best.txt"
filename_calc = "Data/" + test_name + "_calc.txt"

# load points from Data/xqf131.tsp
# Format: first line: n, other lines: id, x, y
points = np.loadtxt(filename, skiprows=1)
n_points = points.shape[0]

# Graphic points with index number, more far
plt.plot(points[:,1], points[:,2], 'o', markersize=2)

plt.savefig('g_points.png')

# load edges_best from optimal in red
if plot_best:
    path_lenght_best = np.loadtxt(filename_best, skiprows=0, max_rows=1)
    edges_best = np.loadtxt(filename_best, skiprows=1, dtype=int)
    edges_best = np.append(edges_best, edges_best[0])

    # plot edges_best
    for i in range(len(edges_best) - 1):
        p1 = edges_best[i]-1
        p2 = edges_best[i+1]-1
        plt.plot([points[p1,1], points[p2,1]], [points[p1,2], points[p2,2]], 'r-')

    plt.savefig('g_best.png')

if plot_calc:
    # load edges_calc from calc in blue
    path_lenght_calc = np.loadtxt(filename_calc, skiprows=0, max_rows=1)
    edges_calc = np.loadtxt(filename_calc, skiprows=1, dtype=int)
    edges_calc = np.append(edges_calc, edges_calc[0])

    # plot edges_calc
    for i in range(len(edges_calc) - 1):
        p1 = edges_calc[i]-1
        p2 = edges_calc[i+1]-1
        plt.plot([points[p1,1], points[p2,1]], [points[p1,2], points[p2,2]], 'b-')

    plt.savefig('g_calc.png')

plt.clf()