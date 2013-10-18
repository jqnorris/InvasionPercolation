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

f = open('chemicalLevels.txt', 'r')
N = int(f.readline())
header = f.readline()

data = []

for line in iter(f):
    line_Num = np.fromstring(line, dtype=int, sep="\t")
    data.append([line_Num[0], line_Num[1], [], [], [], []])
    for j in range(line_Num[1]):
        data[line_Num[0]][2].append(line_Num[4*j+2])
        data[line_Num[0]][3].append(line_Num[4*j+3])
        data[line_Num[0]][4].append(line_Num[4*j+4])
        data[line_Num[0]][5].append(line_Num[4*j+5])
    
f.close()

cmap=plt.get_cmap('hsv_r')

for i in range(0, len(data)):
    for j in range(0,data[i][1]):
        plt.plot([data[i][2][j], data[i][4][j]], [data[i][3][j], data[i][5][j]], color=cmap(i/(1.0*len(data))), zorder=2, solid_capstyle="round")
plt.plot([0],[0],'*', markeredgecolor='none', color=myRed, zorder=5)
plt.axis('equal')
plt.axis('off')
plt.savefig("chemicalLevels.pdf")
