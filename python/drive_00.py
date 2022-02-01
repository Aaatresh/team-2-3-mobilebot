#! /usr/bin/python
import lcm
from time import sleep
import sys
sys.path.append("lcmtypes")

from lcmtypes import mbot_encoder_t
from lcmtypes import mbot_imu_t
from lcmtypes import mbot_motor_command_t
from lcmtypes import odometry_t
from lcmtypes import pose_xyt_t
from lcmtypes import robot_path_t
from lcmtypes import timestamp_t

lc = lcm.LCM("udpm://239.255.76.67:7667?ttl=1")

L = 0.3
# x, y, theta, where theta indicates the direction to be pointed when reaching waypoint
waypoints = [
  (0,0,0),
  (L, 0, 0),
  # (L, L, np.pi/2),
  # (0, L, np.pi),
  # (0, 0, 1.5*np.pi)
]

# drive square, assuming robot starts at start

path = robot_path_t()

for p in waypoints:
  pose = pose_xyt_t()
  pose.x = p[0]
  pose.y = p[1]
  pose.theta = p[2]
  path.path.append(pose)

path.path_length = len(path.path)


lc.publish("CONTROLLER_PATH", path.encode())
