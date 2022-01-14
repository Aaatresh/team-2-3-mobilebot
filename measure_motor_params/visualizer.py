import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt("speed_vs_pwm.txt", delimiter = ",")

plt.plot(data[:, 0], data[:, 1])


