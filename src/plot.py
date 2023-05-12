import numpy as np
import matplotlib.pyplot as plt
FPR3=[0.00100000,0.00100000 , 0.00000000]
FPR4=[0.00210000,0.00060000,0.00030000]
FPR5=[0.00186000,0.00088000,0.00028000]
FPR6=[0.00190700, 0.00097300,0.00024300]

x = np.arange(3)
total_width,n=0.8,4
width = total_width/n
x = x - (total_width-width) / 2
plt.bar(x,FPR3,color="r",width=width,label='1000')
plt.bar(x+width,FPR4,color="y",width=width,label='10000')
plt.bar(x+2*width,FPR5,color="c",width=width,label='100000')
plt.bar(x+3*width,FPR6,color="g",width=width,label='1000000')
plt.xlabel("Expected False Positive Rate")
plt.ylabel("log_e(running time) ms")
plt.legend(loc = "best")
plt.xticks([0,1,2],['1/2^7','1/2^8','1/2^10'])
my_y_ticks=[0.0000,0.0005,0.001,0.0015,0.002,0.0025]#np.arange(0.0001,0.0025,5)
plt.ylim((0.0001,0.0025))
plt.yticks(my_y_ticks)
plt.show()
