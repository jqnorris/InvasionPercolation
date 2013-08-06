# -*- coding: utf-8 -*-
"""
Created on Wed Feb 13 12:45:08 2013

@author: jqnorris
"""

import numpy as np
import matplotlib.pyplot as plt
import subprocess
import os
from scipy import stats

if not os.path.exists('radiusG.txt'):
    if not os.path.exists('RadiusOfGyration_2.0'):
        subprocess.call('ln -s ~/Documents/Permeability/Utilities/RadiusOfGyration_2.0/Release/RadiusOfGyration_2.0', shell=True)

    subprocess.call('./RadiusOfGyration_2.0',
                    stdout=open(os.devnull, 'w'), shell=True)

radiusG = np.loadtxt("radiusG.txt", skiprows=1)

time = np.array(range(0, radiusG.size))

transients = 0; 
plt.plot(time[transients:], radiusG[transients:], color='blue')
slope, intercept, r_value, p_value, std_err = stats.linregress(np.log10(time[(transients+1):]), np.log10(radiusG[(transients+1):]))
line = np.power(10, intercept)*np.power(time, slope)
plt.plot(time,line, color='red')
plt.figtext(0.2, 0.8,"$R_G$ = {0:f} $t$^{1:f}".format(np.power(10, intercept), slope), ha='left')
plt.title('Radius of Gyration')
plt.ylabel('$R_{G}$')
plt.xlabel('$t$')
plt.xscale('log')
plt.yscale('log')
plt.xlim(time[transients], time[-1]*2.0)
plt.ylim(radiusG[transients]/1.5,radiusG[-1]*1.5)
plt.savefig("RGyration2.pdf")