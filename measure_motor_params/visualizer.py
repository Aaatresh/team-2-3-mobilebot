import numpy as np
import matplotlib.pyplot as plt

with open('../common/userdef.h') as f:
    if 'DEVANSH' in f.read():
        USER = "DEVANSH"

    if 'SHREYA' in f.read():
        USER = "SHREYA"

    if 'ANIRUDH' in f.read():
        USER = "ANIRUDH"

data = np.genfromtxt("speed_vs_pwm_"+USER+".txt", delimiter = ",")

plt.figure()
plt.grid()

LEFT_DATA = data[:,0] == 1
RIGHT_DATA = data[:,0] == 2

fig, axs = plt.subplots(2)
axs[0].scatter(data[LEFT_DATA, 1], data[RIGHT_DATA, 2], label="LEFT MOTOR")
axs[0].legend()
axs[0].set(xlabel= "Duty Cycle [%]")
axs[0].set(ylabel= "Speed [m/s]")
# plt.subplots(122)
axs[1].scatter(data[RIGHT_DATA, 1], data[RIGHT_DATA, 2], label="RIGHT MOTOR")
axs[1].legend()
axs[1].set(ylabel="Speed [m/s]")
axs[1].set(xlabel="Duty Cycle [%]")

# plt.figure()
# plt.scatter(data[LEFT_DATA, 1], data[LEFT_DATA, 2] - data[RIGHT_DATA, 2], label="LEFT MOTOR")
# plt.scatter(data[RIGHT_DATA, 1], data[RIGHT_DATA, 2], label="RIGHT MOTOR")

"""
plt.title("Scatter plot of speed vs pwm data points")
plt.xlabel("PWM duty cycle")
plt.ylabel("Speed of robot (m/s)")
plt.show()
"""

# Fitting curve to plotted points
# m1, b1 = np.polyfit(data[:21, 0], data[:21, 1], deg=1)
# m2, b2 = np.polyfit(data[21:34, 0], data[21:34, 1], deg=1)
# m3, b3 = np.polyfit(data[34:, 0], data[34:, 1], deg=1)

# #plt.figure()
# plt.plot(data[:21, 0], m1 * data[:21, 0] + b1, 'k--')
# plt.plot(data[21:34, 0], m2 * data[21:34, 0] + b2, 'k--')
# plt.plot(data[34:, 0], m3 * data[34:, 0] + b3, 'k--')
# plt.title("Curve fit of speed vs pwm data points")
# plt.xlabel("PWM duty cycle")
# plt.ylabel("Speed of robot (m/s)")
# plt.show()
plt.savefig("1.1_" + USER + ".pdf")

