import numpy as np
import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser(description="Plot path of robot using data stored in log file.")
parser.add_argument("--log_file", type=str, default=None)

args = parser.parse_args()

my_data = np.genfromtxt(args.log_file, delimiter=',', skip_header=1)

#print(my_data)
b = 0.1584

X = [x * 0.1 for x in range(1, my_data.shape[0]+1)]

plt.figure()
plt.title("Plot of robot's x position against time at 0.25m/s for 2s.")
plt.ylabel("x position (m)")
plt.xlabel("Time (s)")
plt.plot(X, my_data[:, 6], label="Low pass filtered reference signal")
plt.plot(X, (my_data[:, 8] + my_data[:, 9]) / 2.0, label="Robot's x position from odometry")
axes = plt.gca()
#axes.set_aspect("equal")
plt.legend(loc="upper left")
plt.show()

"""
plt.figure()
plt.title("Plot of robot frame heading against time when turned at pi/8 rad/s for 2s.")
plt.ylabel("Robot frame heading (rad)")
plt.xlabel("Time (s)")
#plt.plot(X, my_data[:, 7], label="Low pass filtered reference signal")
plt.plot(X, my_data[:, 7], label="Reference signal")
plt.plot(X, (-my_data[:, 8] + my_data[:, 9]) / b, label="Robot frame heading from odometry")
axes = plt.gca()
#axes.set_aspect("equal")
plt.legend(loc="upper left")
plt.show()
"""
# plt.savefig("path.png")


