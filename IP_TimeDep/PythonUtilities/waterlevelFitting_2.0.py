# -*- coding: utf-8 -*-
"""
Created on Tue Jan 29 11:33:24 2013

@author: jqnorris
"""

import numpy as np
import matplotlib.pyplot as plt
import pylab
from scipy import stats
from scipy.optimize import curve_fit

cutoff = 0.49
pc=0.5
beginBins = 100


def func(x, lam, tau, N0):
    return np.log(N0)-lam*(pc-cutoff)*x-tau*np.log(x)

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

cutoffs = np.arange(0.4, 0.49, 0.001)
fit1 = np.zeros(cutoffs.size)
fit2 = np.zeros(cutoffs.size)

iterator = 0
for cutoff in cutoffs:
    frequency = np.zeros(100000)
    magnitude = np.array(range(100000))
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

    popt, pcov = curve_fit(func, magBin, np.log(freqBin))
    fit1[iterator] = popt[0]
    fit2[iterator] = popt[1]
    iterator += 1


fig = plt.figure()
ax = fig.add_subplot(111)


ax.plot(cutoffs, fit1, color='blue')
slope, intercept, r_value, p_value, std_err = stats.linregress(cutoffs, fit1)
ax.plot(cutoffs, slope*cutoffs+intercept, color='black')
plt.figtext(0.5, 0.2,"y = {0:0.3f}x+{1:0.3f}".format(intercept, slope), ha='center', color='blue')
ax2 = ax.twinx()
ax2.plot(cutoffs, fit2, color='red')
slope, intercept, r_value, p_value, std_err = stats.linregress(cutoffs, fit2)
ax2.plot(cutoffs, slope*cutoffs+intercept, color='black')
plt.figtext(0.5, 0.8,"y = {0:0.3f}x+{1:0.3f}".format(intercept, slope), ha='center', color='red')
ax.set_xlabel("Cutoff")
ax.set_ylabel("Exponential(Blue)")
ax2.set_ylabel("Power-Law(Red)")
ax.set_title("Cuttoff Dependance of Fitting")
plt.savefig('fitting.pdf')
plt.close()
