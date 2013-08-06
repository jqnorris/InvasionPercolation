# -*- coding: utf-8 -*-
"""
Created on Mon Feb  4 13:14:01 2013

@author: jqnorris
"""

import numpy as np
from matplotlib import pyplot as plt
import pylab

size=10.0

f = open('clusters.txt', 'r')
N = int(f.readline())

data = np.zeros((N, 5))
i=0
for line in iter(f):
    data[i] = np.fromstring(line, sep="\t")
    i += 1
f.close()

clusterN = data[:,0]
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

colors = plt.get_cmap('cool')
plt.figure(figsize=(size, size))
c_max = clusterN.max()
for i in range(0, x1.size):
    if clusterN[i] == 0:
        plt.plot([x1[i], x2[i]], [y1[i], y2[i]], color="black", linewidth=size/L*5, zorder=2, solid_capstyle="round")
    else:
        plt.plot([x1[i], x2[i]], [y1[i], y2[i]], color="black", linewidth=size/L*5, zorder=2, solid_capstyle="round")
plt.plot([0], [0], color='red', marker ='.', markersize=size/L*100)
plt.xlim(midX-0.5*(L+1), midX+0.5*(L+1))
plt.ylim(midY-0.5*(L+1), midY+0.5*(L+1))
plt.axis('off')
plt.savefig("bondClusters.pdf")
plt.savefig("bondClusters.svg")

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

for i in range(0, x1.size):
    plt.plot([x1[i], x2[i]], [y1[i], y2[i]], color="red", linewidth=size/L*15, zorder=1, solid_capstyle="round")
plt.plot([0], [0], color='red', marker ='.', markersize=size/L*100)
plt.savefig("bondClusters+IPGrowth.pdf")
plt.savefig("bondClusters+IPGrowth.svg")

