# -*- coding: utf-8 -*-
"""
Created on Tue Jan 29 11:33:24 2013

@author: jqnorris
"""

import numpy as np
import matplotlib.pyplot as plt

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

average = range(0, strength.size)

average[0]=strength[0]

for i in xrange(1, strength.size):
    average[i]= (i*average[i-1]+strength[i])/(i+1)

t = np.arange(strength.size)
plt.plot(t, average)
plt.title("Moving Average $\\rightarrow$ ${0:f}$".format(average[strength.size-1]))
plt.xlabel('$t$', fontsize=20)
plt.ylabel('$\overline{p}$', fontsize=20)
plt.savefig('MovingAverage.pdf')