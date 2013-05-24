# -*- coding: utf-8 -*-
"""
Created on Tue Jan 29 11:33:24 2013

@author: jqnorris
"""

import numpy as np
import matplotlib.pyplot as plt

f = open('trapped.txt', 'r')
nT = int(f.readline())
header = f.readline()

trapped = np.zeros((nT, 5))
i=0
for line in iter(f):
    trapped[i-2] = np.fromstring(line, sep="\t")
    i += 1
f.close()

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


plt.hist((strength,trapped), bins=500, range=(0, 1), histtype='stepfilled', stacked=True, color=('blue', 'green'))
plt.legend(("\"Trapped\" Bonds", "Broken Bonds"), loc='upper right', frameon=False)
plt.title("Stacked Histogram of Probabilites")
plt.ylabel('$N$', fontsize=20)
plt.xlabel('$p$', fontsize=20)
plt.axis('tight')
plt.savefig('histogramTrapped.pdf')
