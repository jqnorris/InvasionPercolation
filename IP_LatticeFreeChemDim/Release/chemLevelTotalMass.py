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

size=10.0

f = open('Sl.txt', 'r')
N = int(f.readline())
header = f.readline()
check = np.fromstring(f.readline(), dtype=int, sep="\t")

data = np.zeros((N-1,2))

i = 0
for line in iter(f):
    data[i] = np.fromstring(line, dtype=int, sep="\t")
    i += 1

chem_level = data[:,0]
level_mass = data[:,1]

S_l = 0*level_mass
temp_sum = 0

for l in chem_level:
    temp_sum += level_mass[l-1]    
    S_l[l-1] = temp_sum

slope, intercept, r_value, p_value, std_err = stats.linregress(np.log10(chem_level[1:]), np.log10(S_l[1:]))
line = np.power(10, intercept)*np.power(chem_level, slope)

plt.plot(chem_level, S_l,'.', color=myBlue)
plt.plot(chem_level, line, color='black', zorder=2, label="Fit")
plt.figtext(0.35, 0.75, r'$\mathsf{N_{b} \sim A_{b}}$', ha='right', fontsize=30)
plt.figtext(0.33, 0.78, r'{0:0.2f}'.format(slope), ha='left', fontsize=20)
plt.xscale('log')
plt.yscale('log')
plt.savefig("chemLevelTotalMass.pdf")
