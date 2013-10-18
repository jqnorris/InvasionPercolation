# -*- coding: utf-8 -*-
"""
Created on Thu Sep  5 09:34:59 2013

@author: jqnorris
"""

import numpy as np
import matplotlib.pyplot as plt

myRed=(229/255.0,25/255.0,50/255.0)
myBlue=(25/255.0,178/255.0,255/255.0)
myGreen=(50/255.0,255/255.0,0/255.0)

f = open('burstSizes.txt', 'r')
N = int(f.readline())
pc = float(f.readline())

data = np.zeros((N, 2))
i=0
for line in iter(f):
    data[i] = np.fromstring(line, sep="\t")
    i += 1
f.close()

burstSizesRAW = data[:,0]
sizeNumbersRAW = data[:,1]

burstSizesBIN = []
sizeNumbersBIN = []

burstIters = enumerate(burstSizesRAW)
next(burstIters)

for i, val in burstIters:
    burstSizesBIN.append(val)
    sizeNumbersBIN.append(sizeNumbersRAW[i]/(1.0*(val-burstSizesRAW[i-1])))
    
print burstSizesBIN
print sizeNumbersBIN

plt.plot(burstSizesRAW, sizeNumbersRAW, marker='o', linestyle='none', markerfacecolor=myBlue, markeredgecolor='none')
plt.plot(burstSizesBIN, sizeNumbersBIN, marker='o', linestyle='none', markerfacecolor=myRed, markeredgecolor='none')
plt.xscale('log')
plt.yscale('log')
plt.show()