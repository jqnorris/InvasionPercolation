# -*- coding: utf-8 -*-
"""
Created on Mon Feb  4 13:14:01 2013

@author: jqnorris
"""

import numpy as np
from matplotlib import pyplot as plt
myRed=(229/255.0,25/255.0,50/255.0)
myBlue=(25/255.0,178/255.0,255/255.0)
myGreen=(50/255.0,255/255.0,0/255.0)

size=10.0

f = open('fractures.txt', 'r')
N = int(f.readline())
header = f.readline()

data = np.zeros((N, 5))
i=0
for line in iter(f):
    data[i] = np.fromstring(line, sep="\t")
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
for i in range(0, x1.size):
    plt.plot([x1[i], x2[i]], [y1[i], y2[i]], color="black", linewidth=size/L*5, zorder=2, solid_capstyle="round")
plt.plot([0],[0],'*', markersize=size/L*120.0, markeredgecolor='none', color=myRed, zorder=5)
plt.xlim(midX-0.5*(L+1), midX+0.5*(L+1))
plt.ylim(midY-0.5*(L+1), midY+0.5*(L+1))
plt.axis('off')
plt.savefig("bondCluster.pdf")
