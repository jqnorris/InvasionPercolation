# -*- coding: utf-8 -*-
"""
Created on Mon Feb  4 13:14:01 2013

@author: jqnorris
"""

import numpy as np
from matplotlib import pyplot as plt
from scipy import stats


myRed=(229/255.0,25/255.0,50/255.0)
myBlue=(25/255.0,178/255.0,255/255.0)
myGreen=(50/255.0,255/255.0,0/255.0)

beginBins = 20

f = open('cluster_distribution.txt', 'r')
N = int(f.readline())

data = np.zeros((N, 2))
i=0
for line in iter(f):
    data[i] = np.fromstring(line, sep="\t")
    i += 1
f.close()

magnitude = data[:,0]
frequency = data[:,1]

linBins = magnitude[0:beginBins]
logBins = np.logspace(np.log10(magnitude[beginBins]), np.log10(magnitude[-1]), 50)

bins = np.concatenate((linBins, logBins))

magBin = np.zeros(bins.size-2)
freqBin = np.zeros(bins.size-2)

j = 0
for i in range(0, magnitude.size):
    if j <= beginBins:
        magBin[j] = bins[j]
        freqBin[j] = frequency[j]
        j+=1
    if beginBins < j < magBin.size:
        magBin[j] = (bins[j+1]+bins[j])/2.0
        if magnitude[i] < bins[j]:
            freqBin[j] += frequency[i]
        else:
            freqBin[j] = freqBin[j]/(bins[j+1]-bins[j])
            j+=1;
            
counter2 = -1
for i in range(0, freqBin.size):
    if (freqBin[i]>0):
        counter2+=1
        freqBin[counter2] = freqBin[i]
        magBin[counter2] = magBin[i]

magBin = magBin[1:counter2]
freqBin = freqBin[1:counter2]


slope, intercept, r_value, p_value, std_err = stats.linregress(np.log10(magBin), np.log10(freqBin))
line = np.power(10, intercept)*np.power(magBin, slope)

plt.plot(magBin, freqBin, '.', zorder=1, color=myBlue)
plt.plot(magnitude, frequency, '+', color=myRed, zorder=0)
plt.plot(magBin,line, color='black', zorder=2)
plt.figtext(0.8, 0.8,"N = {0:0.0f} t^{1:f}".format(np.power(10, intercept), slope), ha='right')
plt.xlabel('Cluster Size')
plt.ylabel('N')
plt.xscale('log')
plt.yscale('log')
plt.ylim(10**-2, 10**5)
plt.savefig("clusterDist.pdf")
plt.savefig("clusterDist.svg")

