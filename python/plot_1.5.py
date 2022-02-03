import numpy as np
import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser(description="Plot path of robot using data stored in log file.")
parser.add_argument("--log_file", type=str, default=None)

args = parser.parse_args()

my_data = np.genfromtxt(args.log_file, delimiter=',', skip_header=1)[35:75]

#print(my_data)
b = 0.1584

X = [x * 0.1 for x in range(40, my_data.shape[0]+40)]

"""
plt.figure(figsize=(6, 6))
plt.title("Plot of robot's x position when run at 1.0m/s for 1s.")
plt.ylabel("x position (m)")
plt.xlabel("Time (s)")
#plt.plot((2.0, 2.5), (0, 0), 'r--')
#plt.plot((2.5, 4.5), (0, 1.0), 'r--')
#plt.plot((4.5, 5.5), (1.0, 1.0), 'r--')
#plt.plot(X, my_data[:, 6], label="Low pass filtered reference signal")
plt.plot(X, my_data[:, 3], 'b-', label="Robot's x position from odometry")
axes = plt.gca()
#axes.set_aspect("square")
#plt.legend(loc="upper left")
plt.show()
"""

plt.figure()
plt.title("Plot of robot's frame heading when turning at pi/8 rad/s for 2s.")
plt.ylabel("Robot frame heading (rad)")
plt.xlabel("Time (s)")
#plt.plot(X, my_data[:, 7], label="Low pass filtered reference signal")
#plt.plot(X, my_data[:, 7], label="Reference signal")
#plt.plot(X, -np.unwrap(np.array(my_data[:, 5])), label="Robot frame heading from odometry")
plt.plot(X, my_data[:, 5], label="Robot frame heading from odometry")
axes = plt.gca()
#axes.set_aspect("equal")
#plt.legend(loc="upper left")
plt.show()

# plt.savefig("path.png")
