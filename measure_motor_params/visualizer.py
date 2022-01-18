import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt("speed_vs_pwm.txt", delimiter = ",")

plt.figure()
plt.scatter(data[:, 0], data[:, 1])
"""
plt.title("Scatter plot of speed vs pwm data points")
plt.xlabel("PWM duty cycle")
plt.ylabel("Speed of robot (m/s)")
plt.show()
"""

# Fitting curve to plotted points
m, b = np.polyfit(data[:, 0], data[:, 1], deg=1)

#plt.figure()
plt.plot(data[:, 0], m * data[:, 0] + b)
plt.title("Curve fit of speed vs pwm data points")
plt.xlabel("PWM duty cycle")
plt.ylabel("Speed of robot (m/s)")
plt.show()


