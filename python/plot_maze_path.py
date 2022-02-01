import numpy as np
import matplotlib.pyplot as plt
import argparse
import matplotlib.patches as patches

parser = argparse.ArgumentParser(description="Plot path of robot using data stored in log file.")
parser.add_argument("--log_file", type=str, default=None)

args = parser.parse_args()

my_data = np.genfromtxt(args.log_file, delimiter=',', skip_header=1)

print(my_data)

L = 0.61
waypoints = [ 
  (L, 0, 0), 
  (L, -L, -np.pi/2),
  (2*L, -L, 0), 
  (2*L, L, np.pi/2),
  (3*L, L, 0), 
  (3*L, -L, -np.pi/2),
  (4*L, -L, 0), 
  (4*L, 0, np.pi/2),
  (5*L, 0, 0)
]

plt.figure()
plt.title("Robot's path through the maze at 0.80m/s fwd vel. and 3.14rad/s turn vel.")
plt.xlabel("x position")
plt.ylabel("y position")

origin = (0, 0)
rects = []

rects.append(patches.Rectangle((origin[0] - 0.1, origin[1] - 0.1), 0.2, 0.2, fill=False, color="green", linewidth=1))
for pt in waypoints:
    ideal_path, = plt.plot([origin[0], pt[0]], [origin[1], pt[1]], 'r--')
    origin = pt

    rects.append(patches.Rectangle((origin[0] - 0.1, origin[1] - 0.1), 0.2, 0.2, fill=False, color="green", linewidth=1))

actual_path, = plt.plot(my_data[:,3], my_data[:,4])
#rect = patches.Rectangle((L - 0.1, -0.1), 0.2, 0.2, fill=False, color="purple", linewidth=2)
axes = plt.gca()
axes.set_aspect("equal")

for rect in rects:
    axes.add_patch(rect)

plt.legend([ideal_path, actual_path, rects[0]], ["Ideal path", "Odometry output (X, Y)", "Checkpoint (0.2m square)"])

plt.show()

# plt.savefig("path.png")


