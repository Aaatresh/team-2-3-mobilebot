import numpy as np
import matplotlib.pyplot as plt

# with open('../common/userdef.h') as f:

#     txt = f.read()
#     if 'DEVANSH' in txt:
#         USER = "DEVANSH"

#     if 'SHREYA' in txt:
#         USER = "SHREYA"

#     if 'ANIRUDH' in txt:
#         USER = "ANIRUDH"



plt.figure()


fig, axs = plt.subplots(1,2)

for USER in ["DEVANSH", "SHREYA", "ANIRUDH"]:

  data = np.genfromtxt("speed_vs_pwm_"+USER+".txt", delimiter = ",")

  LEFT_DATA = data[:,0] == 1
  RIGHT_DATA = data[:,0] == 2

  
  axs[0].plot(data[LEFT_DATA, 1], data[RIGHT_DATA, 2], label=USER)#, marker="x")
  axs[0].legend()
  axs[0].set(xlabel= "Duty Cycle")
  axs[0].set(ylabel= "Speed [m/s]")
  axs[0].set_title("LEFT_MOTOR")
  axs[0].set_aspect(1)
  axs[0].grid()
  # plt.subplots(122)
  axs[1].plot(data[RIGHT_DATA, 1], data[RIGHT_DATA, 2], label=USER)#, marker="x")
  axs[1].legend()
  axs[1].set(ylabel="Speed [m/s]")
  axs[1].set(xlabel="Duty Cycle")
  axs[1].set_title("RIGHT_MOTOR")
  axs[1].set_aspect(1)
  axs[1].grid()

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

plt.tight_layout()
plt.savefig("1.1_CALIBRATION_CURVES.pdf")

