import numpy as np
import matplotlib.pyplot as plt

my_data = np.genfromtxt('log.txt', delimiter=',', skip_header=1)

print(my_data)

plt.figure()
plot(my_data[:,3], my_data[:,4])

plt.savefig("path.png")


