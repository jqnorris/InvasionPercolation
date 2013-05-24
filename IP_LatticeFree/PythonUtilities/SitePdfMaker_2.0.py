# -*- coding: utf-8 -*-
"""
Created on Mon Feb  4 13:14:01 2013

@author: jqnorris
"""

import numpy as np
from matplotlib import pyplot as plt
import pylab

size=10.0

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

minX = x2.min()
maxX = x2.max()
minY = y2.min()
maxY = y2.max()

deltaX = maxX - minX
midX = minX + 0.5*deltaX
deltaY = maxY - minY
midY = minY + 0.5*deltaY

L = max([deltaX, deltaY])
plt.figure(figsize=(size, size))
plt.axis('off')
plt.plot([0], [0], 's', markersize=size/L*60.0, markeredgecolor='none', color='cyan', zorder=1)
plt.plot(x2, y2, 's', markersize=size/L*60.0, markeredgecolor='none', color='cyan', zorder=1)
plt.xlim(midX-0.5*(L+1), midX+0.5*(L+1))
plt.ylim(midY-0.5*(L+1), midY+0.5*(L+1))
plt.axis('off')
plt.savefig("siteCluster.pdf")
