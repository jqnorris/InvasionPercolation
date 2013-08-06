# -*- coding: utf-8 -*-
"""
Created on Tue Jan 29 11:33:24 2013

@author: jqnorris
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

myRed=(229/255.0,25/255.0,50/255.0)
myBlue=(25/255.0,178/255.0,255/255.0)
myGreen=(50/255.0,255/255.0,0/255.0)

cutoff = .247
beginBins = 20

f = open('fractures.txt', 'r')
N = int(f.readline())

data = np.zeros((N, 7))
i=0
for line in iter(f):
    if i >1:
        data[i-2] = np.fromstring(line, sep="\t")
    i += 1
f.close()

strength = data[:,0]


frequency = np.zeros(1000000)
magnitude = np.array(range(1000000))

counter = 0
for i in range(1,strength.size):
    if (strength[i] < cutoff):
        counter +=1
    else:
        frequency[counter] += 1
        counter = 0

counter = -1
for i in range(0, frequency.size):
    if (frequency[i]>0):
        counter+=1
        frequency[counter] = frequency[i]
        magnitude[counter] = magnitude[i]

linBins = magnitude[0:beginBins]
logBins = np.logspace(np.log10(magnitude[beginBins]), np.log10(magnitude[counter]), 50)

bins = np.concatenate((linBins, logBins))

magBin = np.zeros(bins.size-2)
freqBin = np.zeros(bins.size-2)

j = 0
for i in range(0, counter):
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
plt.plot(magnitude[1:counter], frequency[1:counter], '+', color=myRed, zorder=0)
plt.plot(magBin,line, color='black', zorder=2)
plt.figtext(0.8, 0.8,"N = {0:0.0f} t^{1:f}".format(np.power(10, intercept), slope), ha='right')
plt.title('Frequency-Magnitude of Bursts(cutoff={0})'.format(cutoff))
plt.xlabel('$m_{c}$')
plt.ylabel('$N$')
plt.yscale('log')
plt.xscale('log')
plt.savefig('wlBursts_Binned{0:0.2f}M.pdf'.format(N/1000000.0))
plt.savefig('wlBursts_Binned{0:0.2f}M.svg'.format(N/1000000.0))
plt.close()
