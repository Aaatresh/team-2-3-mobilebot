import numpy as np
import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser(description="Plot path of robot using data stored in log file.")
parser.add_argument("--log_file", type=str, default=None)

args = parser.parse_args()

my_data = np.genfromtxt(args.log_file, delimiter=',', skip_header=1)[:270]

#print(my_data)
b = 0.1584

X = [x * 0.1 for x in range(1, my_data.shape[0]+1)]

plt.figure()
plt.title("Plot of robot forward velocity when completing a square once at 0.25m/s and pi/4 rad/s.")
plt.ylabel("Forward velocity (m/s)")
plt.xlabel("Time (s)")
plt.plot(X, my_data[:, 6], label="Low pass filtered reference signal")
plt.plot(X, (my_data[:, 8] + my_data[:, 9]) / 2.0, label="Robot forward velocity from odometry")
axes = plt.gca()
#axes.set_aspect("equal")
plt.legend(loc="upper left")
plt.show()


plt.figure()
plt.title("Plot of robot rotational velocity when completing a square once at 0.25m/s and pi/4 rad/s.")
plt.ylabel("Rotational velocity (rad/s)")
plt.xlabel("Time (s)")
#plt.plot(X, my_data[:, 7], label="Low pass filtered reference signal")
plt.plot(X, my_data[:, 7], label="Reference signal")
plt.plot(X, (-my_data[:, 8] + my_data[:, 9]) / b, label="Robot rotational velocity from odometry")
axes = plt.gca()
#axes.set_aspect("equal")
plt.legend(loc="upper left")
plt.show()

# plt.savefig("path.png")


