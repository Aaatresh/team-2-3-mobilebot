import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt("speed_vs_pwm.txt", delimiter = ",")

plt.figure()
plt.grid()
plt.scatter(data[:, 0], data[:, 1])
"""
plt.title("Scatter plot of speed vs pwm data points")
plt.xlabel("PWM duty cycle")
plt.ylabel("Speed of robot (m/s)")
plt.show()
"""

# Fitting curve to plotted points
m1, b1 = np.polyfit(data[:21, 0], data[:21, 1], deg=1)
m2, b2 = np.polyfit(data[21:34, 0], data[21:34, 1], deg=1)
m3, b3 = np.polyfit(data[34:, 0], data[34:, 1], deg=1)

#plt.figure()
plt.plot(data[:21, 0], m1 * data[:21, 0] + b1, 'k--')
plt.plot(data[21:34, 0], m2 * data[21:34, 0] + b2, 'k--')
plt.plot(data[34:, 0], m3 * data[34:, 0] + b3, 'k--')
plt.title("Curve fit of speed vs pwm data points")
plt.xlabel("PWM duty cycle")
plt.ylabel("Speed of robot (m/s)")
#plt.show()
plt.savefig("1.1_anirudh.pdf")

