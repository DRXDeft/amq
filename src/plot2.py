import matplotlib.pyplot as plt

x = ['1000','10000','100000','1000000']
k = [5.64897,7.81238,10.0295,12.2515]
plt.plot(x,k,'s-',color = 'r', label = 'MPHF')
plt.xlabel("number_of_keys")
plt.ylabel("log_e(running time) ms")
plt.legend(loc="best")
plt.show();