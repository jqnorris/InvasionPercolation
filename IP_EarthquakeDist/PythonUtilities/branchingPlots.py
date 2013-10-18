# -*- coding: utf-8 -*-
"""
Created on Thu Apr 25 11:10:59 2013

@author: jqnorris
"""

import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict
from random import shuffle;
from scipy import stats

size=10.0
branch_freq = np.loadtxt("takunagaFreq.csv") 
length_order = np.loadtxt("takunagaLeng.csv") 
takunaga = np.loadtxt("takunaga.csv")

N = branch_freq[:,-1]
r = length_order[:,-1]
k_array = np.array(range(1, len(length_order)))
orders = np.array(range(1, len(length_order)+1))

Tk = np.zeros(k_array.size)
n = len(k_array)
for k in range(k_array.size):
    for i in range(0, n-k):
        Tk[k] += takunaga[i, i+k]
    Tk[k] = Tk[k]/(1.0*(n-k))

slope, intercept, r_value, p_value, std_err = stats.linregress(-orders[1:-1], np.log10(N[1:-1]))

orders1 = np.append(orders, [0, 11])
line = np.power(10, intercept)*np.power(np.power(10, slope), -orders1)
Rb = np.power(10, slope)
plt.plot(orders, N, '.', markersize=12, color='blue')
plt.plot(orders1, line, linewidth=2, color='black', zorder=2)
plt.plot(orders1, line, linewidth=2, color='black', zorder=2)
plt.figtext(0.8, 0.75,"$N_i = {{ {0:0.0f} }} \\times\, {{ {1:0.4f} }}^{{ -i }}$".format(np.power(10, intercept), np.power(10, slope)), ha='right', fontsize=16)
plt.xlabel('$i$', fontsize=16)
plt.ylabel('$N_i$', fontsize=16)
plt.yscale('log')
plt.savefig('numberOrder.pdf')


slope, intercept, r_value, p_value, std_err = stats.linregress(orders[1:-1], np.log10(r[1:-1]))
Rr = np.power(10, slope)
orders1 = np.append(orders, [0, 11])
line = np.power(10, intercept)*np.power(np.power(10, slope), orders1)
plt.figure()
plt.plot(orders, r, '.', markersize=12, color='blue')
plt.plot(orders1, line, linewidth=2, color='black', zorder=2)
plt.figtext(0.85, 0.5,"$r_i = {{ {0:0.2f} }} \\times\, {{ {1:0.4f} }}^i$".format(np.power(10, intercept), np.power(10, slope)), ha='right', fontsize=16)
plt.xlabel('$i$', fontsize=16)
plt.ylabel('$r_i$', fontsize=16)
plt.yscale('log')
plt.savefig('lengthOrder.pdf')
plt.savefig('lengthOrder.svg')


D = np.log(Rb)/np.log(Rr)
print D
slope, intercept, r_value, p_value, std_err = stats.linregress(np.log10(r[1:-1]), np.log10(N[1:-1]))

r1 = np.append(r, [r[0]/2.0, r[-1]*2.0])
line = np.power(10, intercept)*np.power(r1, slope)
plt.figure()
plt.plot(r, N, '.', markersize=12, color='blue')
plt.plot(r1, line, linewidth=2, color='black', zorder=2)
plt.figtext(0.8, 0.75,"$N = {{ {0:0.0f} }} r^{{ {1:f} }}$".format(np.power(10, intercept), slope), ha='right', fontsize=16)
plt.xlabel('$r$', fontsize=16)
plt.ylabel('$N$', fontsize=16)
plt.xscale('log')
plt.yscale('log')
plt.savefig('takunagaDim.pdf')
plt.savefig('takunagaDim.svg')

slope, intercept, r_value, p_value, std_err = stats.linregress(k_array[1:-1], np.log10(Tk[1:-1]))
c = np.power(10, slope)
a = c*np.power(10, intercept)
line = a*np.power(c, k_array-1)
plt.figure()
plt.plot(k_array, Tk, '.', markersize=12, color='blue')
plt.plot(k_array, line, linewidth=2, color='black', zorder=2)
plt.figtext(0.2, 0.8,"$T_k={{ {0:0.2f} }}  \\times \, {{ {1:0.2f} }}$".format(a, c) + "$^{k+1}$", ha='left', fontsize=16)
plt.xlabel('$k$', fontsize=16)
plt.ylabel('$T_k$', fontsize=16)
plt.yscale('log')
plt.xlim(0,len(N))
plt.savefig('takunagaBranchings.pdf')
plt.savefig('takunagaBranchings.svg')



#minX = x2.min()
#maxX = x2.max()
#minY = y2.min()
#maxY = y2.max()
#
#deltaX = maxX - minX
#midX = minX + 0.5*deltaX
#deltaY = maxY - minY
#midY = minY + 0.5*deltaY
#
#L = max([deltaX, deltaY])
#
#fig = plt.figure(figsize=(size, size))
#colors = plt.get_cmap('jet_r')
#color_index = 0
#color_MAX = len(tree.branches)+2
#    color_index += 1
#    for sub_order in order:
#        for branch in sub_order:
#            labelLoc = branch.bonds[-1]
#            direction = (np.abs(labelLoc[0][0]-labelLoc[1][0]), np.abs(labelLoc[0][1]-labelLoc[1][1]))
#            if direction == (0,1):
#                textX = labelLoc[0][0] + 0.4
#                textY = labelLoc[0][1] - 0.5*(labelLoc[0][1]-labelLoc[1][1])
#                plt.text(textX, textY, '{0}:{1}'.format(branch.order, branch.sub_order), ha='center', va='center', fontsize=20)
#            if direction == (1,0):
#                textX = labelLoc[0][0] - 0.5*(labelLoc[0][0]-labelLoc[1][0])
#                textY = labelLoc[0][1] + 0.25
#                plt.text(textX, textY, '{0}:{1}'.format(branch.order, branch.sub_order), ha='center', va='center', fontsize=20)
#            for bond in branch.bonds:
#                plt.plot([bond[0][0], bond[1][0]], [bond[0][1], bond[1][1]], color=colors(color_index/(1.0*color_MAX)), linewidth=size/L*4, zorder=2, solid_capstyle="round")
#plt.plot([0], [0], color='black', marker ='.', markersize=size/L*30)
#plt.xlim(midX-0.5*(L+1), midX+0.5*(L+1))
#plt.ylim(midY-0.5*(L+1), midY+0.5*(L+1))
#plt.axis('off')
#plt.savefig("BranchingOrder.pdf")
            
    
