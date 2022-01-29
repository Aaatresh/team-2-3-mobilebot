import numpy as np
import matplotlib.pyplot as plt

plt.figure()
#for USER in ["DEVANSH", "SHREYA", "ANIRUDH"]:

data = np.genfromtxt("pid_SHREYA.txt", delimiter = ",")
#print(data)

Desired = data[:,0] 
Actual = data[:,1] 

plt.plot(Desired)
plt.plot(Actual)
plt.show()
 