# -*- coding: utf-8 -*-
"""
Created on Tue Jan 29 11:33:24 2013

@author: jqnorris
"""

import numpy as np
import matplotlib.pyplot as plt
myRed=(229/255.0,25/255.0,50/255.0)
myBlue=(25/255.0,178/255.0,255/255.0)
myGreen=(50/255.0,255/255.0,0/255.0)

f = open('trapped.txt', 'r')
nT = int(f.readline())
header = f.readline()

trapped = np.zeros((nT, 7))
i=0
for line in iter(f):
    trapped[i-2] = np.fromstring(line, sep="\t")
    i += 1
f.close()

f = open('fractures.txt', 'r')
N = int(f.readline())
header = f.readline()

data = np.zeros((N, 7))
i=0
for line in iter(f):
    data[i-2] = np.fromstring(line, sep="\t")
    i += 1
f.close()

strength = data[:,0]


numBins = 500.0
strHist, strBins = np.histogram(strength, bins=numBins, range=(0,1))
traHist, traBins = np.histogram(trapped, bins=numBins, range=(0,1))

strX = np.zeros(2*strHist.size)
strY = np.zeros(2*strHist.size)
for i, count in enumerate(strHist):
    value = count/float(strBins[i+1]-strBins[i])*float(10**-7)
    left = strBins[i]
    right = strBins[i+1]
    strX[2*i] = left
    strY[2*i] = value
    strX[2*i+1] = right
    strY[2*i+1] = value

traX = np.zeros(2*traHist.size)
traY = np.zeros(2*traHist.size)
for i, count in enumerate(traHist):
    value = count/float(traBins[i+1]-traBins[i])*float(10**-7)
    left = traBins[i]
    right = traBins[i+1]
    traX[2*i] = left
    traY[2*i] = value
    traX[2*i+1] = right
    traY[2*i+1] = value    
    
plt.plot(strX, strY, zorder=2, color='black')
plt.plot(traX, strY+traY, zorder=2, color='black')
plt.fill_between(strX, 0, strY, color = myBlue, zorder=1)
plt.fill_between(traX, strY, strY+traY, color = myGreen, zorder=1)
plt.ylabel(r'$f\left(s\right)\,\times\,10^{-7}$', fontsize=20)
plt.xlabel(r'$s\,\left(\mathrm{strength}\right)$', fontsize=20)
plt.axis('tight')
plt.savefig('histogramTrapped.svg')
