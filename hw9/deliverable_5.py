import matplotlib.pyplot as plt
import numpy as np

import csv

def MAF(X, data):
    filtered = [0] * (len(data) - X)
    for i in range(len(filtered)):
        points = data[i:(i+X)]
        filtered[i] = sum(points)/len(points)
    return filtered


t = [] # column 0
data1 = [] # column 1
data2 = [] # column 2

with open("C:/Users/chris/Downloads/01_Quickstart_nu32dip/01_Quickstart_nu32dip/ME-433/hw9/dsp/sigD.csv") as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data1.append(float(row[1])) # second column
        # data2.append(float(row[2])) # third column

dt = 1.0/10000.0 # 10kHz

#unfiltered data
Fs = 10000 # sample rate
Ts = 1.0/Fs; # sampling interval
ts = np.arange(0,t[-1],Ts) # time vector
y = data1 # the data to make the fft from
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]


# filtered data
y = MAF(420, data1) # the data to make the fft from
Fs = len(y)/t[-1] # sample rate
Ts = 1.0/Fs; # sampling interval
ts = np.arange(0,t[-1],Ts) # time vector
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frqf = k/T # two sides frequency range
frqf = frq[range(int(n/2))] # one side frequency range
Yf = np.fft.fft(y)/n # fft computing and normalization
Yf = Yf[range(int(n/2))]

fig, (ax1, ax2) = plt.subplots(2, 1)
plt.title('Signal D, 420 points', pad = 150)
ax1.loglog(frq ,abs(Y),'k') # plotting the fft
ax1.set_xlabel('Freq (Hz)')
ax1.set_ylabel('|Y(freq)|')

ax2.loglog(frqf,abs(Yf),'r') # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
plt.show()