import numpy as np
import matplotlib.pyplot as plt

# Read distances from the CSV file
distances = np.genfromtxt('./distances.csv',
                          delimiter=',', skip_header=1)
size = distances.shape[1]

plt.figure(figsize=(8, 8))
plt.scatter(np.linspace(0, 1, size),
            distances[0], label='Nodes', color='red')


plt.xlabel('X')
plt.ylabel('Y')
plt.legend()
plt.grid(True)

plt.savefig("plot.png")
