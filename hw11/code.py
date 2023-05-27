from ulab import numpy as np # to get access to ulab numpy functions
# from ulab qimport matplotlib.pyplot as plt
import time


# Declare an array with some made up data like
a = np.zeros(1024)
i = 0
while i < 1024:
    x = i/360
    a[i] = np.sin(x) + np.sin(2*x) + np.sin(3*x)
    i = i + 1
n = len(a)

y = np.fft.fft(a)

for j in range(0,n-2):
    print((a[j], a[j+1], a[j+2]))
    time.sleep(0.05)
    time.sleep(0.05)




# print("done!")
