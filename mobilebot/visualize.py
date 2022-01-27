import numpy as numpy
import matplotlib.pyplot as plt

plt.figure()
for USER in ["DEVANSH", "SHREYA", "ANIRUDH"]:

  data = np.genfromtxt("pid"+USER+".txt", delimiter = ",")

  Desired = data[:,0] 
  Actual = data[:,1] 

  plt.plot(Desired)
  plt.plot(Actual)
  plt.show()
 