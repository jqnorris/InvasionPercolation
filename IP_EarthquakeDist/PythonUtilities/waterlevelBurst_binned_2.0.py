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

f = open('burstClusters.txt', 'r')
N = int(f.readline())

data = np.zeros((N, 3))
i=0
for line in iter(f):
    data[i] = np.fromstring(line, sep="\t")
    i += 1
f.close()

burstN = data[:,0]
clusterN = data[:,1]
counts = data[:,2]

print np.unique(burstN).size == burstN.size





