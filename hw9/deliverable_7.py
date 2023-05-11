import matplotlib.pyplot as plt
import numpy as np

import csv

def IIR(A, data):
    B = 1-A
    filtered = [0] * (len(data))
    filtered[0] = data[0]
    for i in range(1, len(filtered)):
        filtered[i] = A*filtered[i-1] + B*data[i]
    return filtered

def dot(K, L):
   if len(K) != len(L):
      return 0

   return sum(i[0] * i[1] for i in zip(K, L))
        

def FIR(coeffs, data):
    filtered = [0] * (len(data) - len(coeffs))
    L = len(coeffs)
    for i, v in enumerate(filtered):
        
        filtered[i] = dot(data[i:(i+L)], coeffs)
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

coeffs = [0.090011231804371733,\
0.090548754148121274,\
0.090968151967928110,\
0.091268433002908794,\
0.091448886490887463,\
0.091509085171565419,\
0.091448886490887463,\
0.091268433002908794,\
0.090968151967928110,\
0.090548754148121274,\
0.090011231804371733]

# filtered data
A = 0.993
y = FIR(coeffs, data1) # the data to make the fft from
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
plt.title(('Signal D, 11 weights, bL = 900, fL = 100'), pad = 150)
ax1.loglog(frq ,abs(Y),'k') # plotting the fft
ax1.set_xlabel('Freq (Hz)')
ax1.set_ylabel('|Y(freq)|')

ax2.loglog(frqf,abs(Yf),'r') # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
plt.show()