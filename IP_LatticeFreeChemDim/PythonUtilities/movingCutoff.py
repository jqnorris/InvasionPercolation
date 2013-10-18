# -*- coding: utf-8 -*-
"""
Created on Tue Jan 29 11:33:24 2013

@author: jqnorris
"""

import numpy as np
import matplotlib.pyplot as plt

numpoints = 10000;

strength = np.loadtxt("fractures.txt", skiprows=1, unpack=True)
time = np.arange(0,strength.size)

time = time[np.argsort(strength)]

strength.sort()

strength = 0.5-strength


plt.scatter(time[(strength.size-numpoints):(strength.size-1)],strength[(strength.size-numpoints):(strength.size-1)])
plt.yscale('log')
plt.ylim(10**-4, 10**-2)
plt.savefig('records.pdf')
    
    
