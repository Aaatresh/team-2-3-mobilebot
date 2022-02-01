import numpy as np
import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser(description="Plot path of robot using data stored in log file.")
parser.add_argument("--log_file", type=str, default=None)

args = parser.parse_args()

my_data = np.genfromtxt(args.log_file, delimiter=',', skip_header=1)

print(my_data)

plt.figure()
plt.title("Robot's path through the maze at 0.25m/s fwd vel. and 0.785rad/s turn vel.")
plt.xlabel("x position")
plt.ylabel("y position")
plt.plot(my_data[:,3], my_data[:,4])
plt.show()

# plt.savefig("path.png")


