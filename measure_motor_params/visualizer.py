from operator import le
import numpy as np
import matplotlib.pyplot as plt
import csv

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
  


plt.tight_layout()
plt.savefig("1.1_CALIBRATION_CURVES.pdf")


## CALIBRATION 

# fit the positive duty cycles

with open("../common/MOTOR_CALIBRATION.h", 'w', newline="") as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',',
                            quotechar='|', quoting=csv.QUOTE_MINIMAL)
    spamwriter.writerow("")

for USER in ["DEVANSH", "SHREYA", "ANIRUDH"]:

  data = np.genfromtxt("speed_vs_pwm_"+USER+".txt", delimiter = ",")

  # LEFT POSITIVE calibration 
  LEFT_POS_INDS = []
  for i in range(len(data[:,0])):
    if (data[i,0] == 1) and (data[i,2] >= 0.2):
      LEFT_POS_INDS.append(i)

  # fit linear
  m_LEFT_POS, b_LEFT_POS = np.polyfit(data[LEFT_POS_INDS, 2], data[LEFT_POS_INDS, 1], deg=1)

  print(m_LEFT_POS, b_LEFT_POS)


  # RIGHT POSITIVE calibration 
  RIGHT_POS_INDS = []
  for i in range(len(data[:,0])):
    if (data[i,0] == 2) and (data[i,2] >= 0.2):
      RIGHT_POS_INDS.append(i)

  # fit linear
  m_RIGHT_POS, b_RIGHT_POS = np.polyfit(data[RIGHT_POS_INDS, 2], data[RIGHT_POS_INDS, 1], deg=1)
  print(m_RIGHT_POS, b_RIGHT_POS)


  # LEFT NEG calibration 
  LEFT_NEG_INDS = []
  for i in range(len(data[:,0])):
    if (data[i,0] == 1) and (data[i,2] <= -0.2):
      LEFT_NEG_INDS.append(i)

  # fit linear
  m_LEFT_NEG, b_LEFT_NEG = np.polyfit(data[LEFT_NEG_INDS, 2], data[LEFT_NEG_INDS, 1], deg=1)
  print(m_LEFT_NEG, b_LEFT_NEG)


  # RIGHT NEG calibration 
  RIGHT_NEG_INDS = []
  for i in range(len(data[:,0])):
    if (data[i,0] == 2) and (data[i,2] <= -0.2):
      RIGHT_NEG_INDS.append(i)

  # fit linear
  m_RIGHT_NEG, b_RIGHT_NEG = np.polyfit(data[RIGHT_NEG_INDS, 2], data[RIGHT_NEG_INDS, 1], deg=1)

  print(m_RIGHT_NEG, b_RIGHT_NEG)

  # create plots
  fig, axs = plt.subplots(1,2)

  data = np.genfromtxt("speed_vs_pwm_"+USER+".txt", delimiter = ",")

  LEFT_DATA = data[:,0] == 1
  RIGHT_DATA = data[:,0] == 2

  speeds_pos = np.linspace(0, 1.2, 100)
  speeds_neg = np.linspace(-1.2, 0, 100)
  

  
  axs[0].plot(data[LEFT_DATA, 2], data[RIGHT_DATA, 1], label=USER)#, marker="x")
  axs[0].plot(speeds_pos, m_LEFT_POS*speeds_pos+b_LEFT_POS, label="fit", linestyle="--", color="orange")
  axs[0].plot(speeds_neg, m_LEFT_NEG*speeds_neg+b_LEFT_NEG, label="fit", linestyle="--", color="orange")
  axs[0].legend()
  axs[0].set(ylabel= "Duty Cycle")
  axs[0].set(xlabel= "Speed [m/s]")
  axs[0].set_title("LEFT_MOTOR")
  axs[0].set_aspect(1)
  axs[0].grid()
  # plt.subplots(122)
  axs[1].plot(data[RIGHT_DATA, 2], data[RIGHT_DATA, 1], label=USER)#, marker="x")
  axs[1].plot(speeds_pos, m_RIGHT_POS*speeds_pos+b_RIGHT_POS, label="fit", linestyle="--", color="orange")
  axs[1].plot(speeds_neg, m_RIGHT_NEG*speeds_neg+b_RIGHT_NEG, label="fit", linestyle="--", color="orange")
  axs[1].legend()
  axs[1].set(xlabel="Speed [m/s]")
  axs[1].set(ylabel="Duty Cycle")
  axs[1].set_title("RIGHT_MOTOR")
  axs[1].set_aspect(1)
  axs[1].grid()

  

  plt.tight_layout()
  plt.savefig("1.1_"+USER+".pdf")





  with open("MOTOR_CALIBRATION_"+USER+".csv", 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',',
                            quotechar='|', quoting=csv.QUOTE_MINIMAL)
    spamwriter.writerow(['CHANNEL', "DIRECTION", "M", "B"])
    spamwriter.writerow([1, 1, m_LEFT_POS, b_LEFT_POS])
    spamwriter.writerow([1, -1, m_LEFT_NEG, b_LEFT_NEG])
    spamwriter.writerow([2, 1, m_RIGHT_POS, b_RIGHT_POS])
    spamwriter.writerow([2, -1, m_RIGHT_NEG, b_RIGHT_NEG])
    # spamwriter.writerow(['Spam', 'Lovely Spam', 'Wonderful Spam']) 

  with open("../common/MOTOR_CALIBRATION.h", 'a', newline="") as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',',
                            quotechar='|', quoting=csv.QUOTE_MINIMAL)
    spamwriter.writerow(["#ifdef USER_"+USER])
    spamwriter.writerow(["#define MOTOR_CALIB_LEFT_POS_M " + str(m_LEFT_POS)])
    spamwriter.writerow(["#define MOTOR_CALIB_LEFT_POS_B " + str(b_LEFT_POS)])
    spamwriter.writerow(["#define MOTOR_CALIB_LEFT_NEG_M " + str(m_LEFT_NEG)])
    spamwriter.writerow(["#define MOTOR_CALIB_LEFT_NEG_B " + str(b_LEFT_NEG)])
    spamwriter.writerow(["#define MOTOR_CALIB_RIGHT_POS_M " + str(m_RIGHT_POS)])
    spamwriter.writerow(["#define MOTOR_CALIB_RIGHT_POS_B " + str(b_RIGHT_POS)])
    spamwriter.writerow(["#define MOTOR_CALIB_RIGHT_NEG_M " + str(m_RIGHT_NEG)])
    spamwriter.writerow(["#define MOTOR_CALIB_RIGHT_NEG_B " + str(b_RIGHT_NEG)])
    spamwriter.writerow(["#endif"])
    
