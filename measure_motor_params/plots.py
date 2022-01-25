import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt("speed_vs_pwm_ANIRUDH.txt", delimiter=',')
print(data)

from sklearn.linear_model import LinearRegression

reg = LinearRegression().fit(data[:,1].reshape(-1,1), data[:,2].reshape(-1,1))
print(reg.coef_, reg.intercept_)

plt.scatter(data[:,1], data[:,2], marker='o')
plt.plot(np.linspace(0, 1, 250).reshape(-1,1), reg.predict(np.linspace(0, 1, 250).reshape(-1,1)))
equation = "%.4f*x + %.4f" % (reg.coef_[0][0], reg.intercept_[0])
plt.title('Right motor: ' + equation)
plt.savefig("right_motor_plot_anirudh.png")
 