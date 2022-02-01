#! /usr/bin/python
import lcm
from time import sleep
import sys
import time
sys.path.append("lcmtypes")
import numpy as np
from lcmtypes import mbot_encoder_t
from lcmtypes import mbot_imu_t
from lcmtypes import mbot_motor_command_t
from lcmtypes import odometry_t
from lcmtypes import pose_xyt_t
from lcmtypes import robot_path_t
from lcmtypes import timestamp_t

sq_w = 1.0
sq_d = 1.0


lc = lcm.LCM()


print("STARTING!")
t0 = time.time()


stop_cmd = mbot_motor_command_t()
stop_cmd.trans_v = 0.0
stop_cmd.angular_v = 0.0;
lc.publish("MBOT_MOTOR_COMMAND", stop_cmd.encode())

motor_cmd = mbot_motor_command_t()
motor_cmd.trans_v = 0.0;
motor_cmd.angular_v = np.pi;


while time.time() - t0 <= 2.0:
    lc.publish("MBOT_MOTOR_COMMAND", motor_cmd.encode())
    sleep(0.01)


lc.publish("MBOT_MOTOR_COMMAND", stop_cmd.encode())
sleep(0.3)

print("DONE!")
