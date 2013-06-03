# -*- coding: utf-8 -*-
"""
Created on Tue Jan 29 11:33:24 2013

@author: jqnorris
"""

import numpy as np
import matplotlib.pyplot as plt
import pylab
from scipy import stats

f = open('fractures.txt', 'r')
N = int(f.readline())
header = f.readline()

data = np.zeros((N, 5))
i=0
for line in iter(f):
    data[i-2] = np.fromstring(line, sep="\t")
    i += 1
f.close()

strength = data[:,0]
x1 = data[:,1]
y1 = data[:,2]
x2 = data[:,3]
y2 = data[:,4]

frequency = np.zeros(1000)
magnitude = np.array(range(1000))

counter = 0
for i in range(1,x1.size):
    if x1[i] == x2[i-1] and y1[i] == y2[i-1]:
        counter +=1
    else:
        frequency[counter] += 1
        counter = 0
        
counter2 = -1
for i in range(0, frequency.size):
    if (frequency[i]>0):
        counter2+=1
        frequency[counter2] = frequency[i]
        magnitude[counter2] = magnitude[i]

magnitude = magnitude[1:counter2]
frequency = frequency[1:counter2]

slope, intercept, r_value, p_value, std_err = stats.linregress(magnitude[0:30], np.log(frequency[0:30]))        
  
      
line = np.exp(intercept) * np.exp(slope*magnitude)

plt.plot(magnitude, frequency, color='blue')
plt.plot(magnitude, line, color='red')
plt.figtext(0.8, 0.8,"y = {0:0.0f} exp({1:f}*x)".format(np.exp(intercept), slope), ha='right')
plt.title('Frequency-Magnitude of Rupture Chains')
plt.xlabel('$m_{c}$')
plt.ylabel('$N$')
plt.yscale('log')
plt.savefig('stringDist.pdf')