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

sq_w = 1.0
sq_d = 1.0

class WaypointFollower():
    def __init__(self):
        """ Setup LCM and subscribe """
        self.lc = lcm.LCM()
        self.lcm_sub = self.lc.subscribe("ODOMETRY", self.odometry_handler)
        self.waypoints = [[0.0,0.0],[sq_w,0.0],[sq_w,sq_d],[0.0,sq_d],[0.0,0.0]]
        self.wpt_num = 0
        self.wpt_thresh = 0.01
   
    def odometry_handler(self, channel, data):
        msg = odometry_t().decode(data)
        msg.x
        msg.y
        msg.theta
        
    def motor_cmd_publish():
        msg = mbot_motor_command_t()
        msg.utime = 0.0
        msg.trans_v = 0.0
        msg.angular_v = 0.0
        self.lc.publish("MBOT_MOTOR_COMMAND",msg.encode())


lc = lcm.LCM()

L = 0.5; # square side-length

print("STARTING!")

stop_cmd = mbot_motor_command_t()
stop_cmd.trans_v = 0.0
stop_cmd.angular_v = 0.0;
lc.publish("MBOT_MOTOR_COMMAND", stop_cmd.encode())

for i in range(4):
    motor_cmd = mbot_motor_command_t()
    motor_cmd.trans_v = 0.25;
    motor_cmd.angular_v = 0.0
    lc.publish("MBOT_MOTOR_COMMAND", motor_cmd.encode())
    sleep(1.0) 

    lc.publish("MBOT_MOTOR_COMMAND", stop_cmd.encode())
    sleep(0.3)

    angular_cmd = mbot_motor_command_t()
    angular_cmd.trans_v = 0.0
    angular_cmd.angular_v = -3.14/4;
    lc.publish("MBOT_MOTOR_COMMAND", angular_cmd.encode())
    sleep(2.0)

    lc.publish("MBOT_MOTOR_COMMAND", stop_cmd.encode())
    sleep(0.3)

    