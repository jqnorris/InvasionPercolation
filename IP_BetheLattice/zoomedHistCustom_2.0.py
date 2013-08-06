# -*- coding: utf-8 -*-
"""
Created on Tue Jan 29 11:33:24 2013

@author: jqnorris
"""

myRed=(229/255.0,25/255.0,50/255.0)
myBlue=(25/255.0,178/255.0,255/255.0)
myGreen=(50/255.0,255/255.0,0/255.0)

import numpy as np
import matplotlib.pyplot as plt

f = open('fractures.txt', 'r')
N = int(f.readline())
header = f.readline()

data = np.zeros((N, 3))
i=0
for line in iter(f):
    data[i-2] = np.fromstring(line, sep="\t")
    i += 1
f.close()

strength = data[:,0]

t = range(0, strength.size)


plt.hist(strength[5000000:], bins=200, range=(0.19, 0.21), histtype='stepfilled', color=myBlue)
plt.title("Histogram of Failed Strengths around p=0.2")
plt.ylabel('$N$', fontsize=20)
plt.xlabel('$p$', fontsize=20)
plt.axis('tight')
plt.savefig('histogramZoomed.pdf')
plt.savefig('histogramZoomed.svg')

