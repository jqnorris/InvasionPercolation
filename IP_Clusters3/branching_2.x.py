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

class branch_class:
    sub_order = 0
    def __init__(self, i_bond, i_free_end, i_order):
        self.bonds = [i_bond]
        self.free_end = i_free_end
        self.order = i_order
    def __gt__(self, branch2):
        return self.order > branch2.order
    def __lt__(self, branch2):
        return self.order < branch2.order
    def __eq__(self, branch2):
        return self.order == branch2.order
    


class tree_class:
    max_order = 1
    branches = [[ [] for i in range(20) ] for i in range(20)]

size=10.0
f = open('cluster_distribution.txt')
temp = np.fromstring(f.readline(), sep='\t')
N = int(temp[0])
largest_cluster = temp[1]
seed_x = temp[2]
seed_y = temp[3]
data = np.zeros((N, 2))
i=0
for line in iter(f):
    data[i] = np.fromstring(line, sep='\t')
    i += 1
largest_cluster_size = int(data[-1,0])
f.close()

f = open('clusters.txt', 'r')
N = int(f.readline())

data = np.zeros((largest_cluster_size, 5))
i=0
for line in iter(f):
    temp = np.fromstring(line, sep="\t")
    if(temp[0] == largest_cluster):
        data[i] = temp
        i += 1
f.close()

clusterN = data[:,0]
x1 = data[:,1]
y1 = data[:,2]
x2 = data[:,3]
y2 = data[:,4]

bonds = zip(zip(x1, y1), zip(x2, y2))

ordered_bonds = defaultdict(list)
for bond in bonds:
    ordered_bonds[bond[0]].append(bond)
    ordered_bonds[bond[1]].append(bond)

# Collecting all the free ends
pending = defaultdict(list)
garbage_list = []
for key in ordered_bonds:
    if len(ordered_bonds[key]) == 1:
        if key != (seed_x,seed_y):
            if key == ordered_bonds[key][0][0]:
                pending[ordered_bonds[key][0][1]].append(branch_class(ordered_bonds[key][0], ordered_bonds[key][0][1], 1))
                garbage_list.append(key)
            else:
                pending[ordered_bonds[key][0][0]].append(branch_class(ordered_bonds[key][0], ordered_bonds[key][0][0], 1))
                garbage_list.append(key)

for garbage in garbage_list:
    del ordered_bonds[garbage]

# Look for connections
processing = defaultdict(list)
tree = tree_class()
while len(ordered_bonds) > 0:
    garbage_list = []
    processing = defaultdict(list)
    for key in pending:
        if key == (seed_x,seed_y):
            if len(ordered_bonds) == 1:
                garbage_list.append(key)
                for branch in pending[key]:
                    branch.sub_order = branch.order
                    tree.branches[branch.order][branch.sub_order].append(branch)                    
        elif key in ordered_bonds:
            if len(pending[key]) == 1:
                if len(ordered_bonds[key]) == 2:
                    garbage_list.append(key)
                    new_bond = [bond for bond in ordered_bonds[key] if bond not in pending[key][0].bonds][0]
                    new_end = [end for end in new_bond if end != key][0]
                    pending[key][0].bonds.append(new_bond)
                    pending[key][0].free_end = new_end
                    processing[pending[key][0].free_end].append(pending[key][0])               
            elif len(pending[key]) == 2:
                if len(ordered_bonds[key]) == 3:
                    garbage_list.append(key)
                    new_bond = [bond for bond in ordered_bonds[key] if bond not in [pending[key][0].bonds[-1], pending[key][1].bonds[-1]]][0]          
                    new_end = [end for end in new_bond if end != key][0]
                    if pending[key][0] == pending[key][1]:
                        pending[key][0].sub_order = pending[key][0].order   
                        pending[key][1].sub_order = pending[key][1].order
                        tree.branches[pending[key][0].order][pending[key][0].sub_order].append(pending[key][0])
                        tree.branches[pending[key][1].order][pending[key][1].sub_order].append(pending[key][1])
                        processing[new_end].append(branch_class(new_bond, new_end, pending[key][0].order+1))
                    else:
                        branches = sorted([pending[key][0], pending[key][1]])
                        branches[0].sub_order = branches[1].order
                        tree.branches[branches[0].order][branches[0].sub_order].append(branches[0])
                        branches[1].bonds.append(new_bond)
                        branches[1].free_end = new_end
                        processing[branches[1].free_end].append(branches[1])
            elif len(pending[key]) == 3:
                if len(ordered_bonds[key]) == 4:
                    garbage_list.append(key)
                    new_bond = [bond for bond in ordered_bonds[key] if bond not in [pending[key][0].bonds[-1], pending[key][1].bonds[-1], pending[key][2].bonds[-1]]][0]
                    new_end = [end for end in new_bond if end != key][0]
                    branches = sorted([pending[key][0], pending[key][1], pending[key][2]])
                    if branches[0] == branches[1] == branches[2]:
                        subOrders = [branches[0].order, branches[0].order, branches[0].order+1]                
                        shuffle(subOrders)
                        branches[0].sub_order = subOrders[0]  
                        branches[1].sub_order = subOrders[1]
                        branches[2].sub_order = subOrders[2]
                        tree.branches[branches[0].order][branches[0].sub_order].append(branches[0])
                        tree.branches[branches[1].order][branches[1].sub_order].append(branches[1])
                        tree.branches[branches[2].order][branches[2].sub_order].append(branches[2])
                        processing[new_end].append(branch_class(new_bond, new_end, branches[0].order+1))
                    elif branches[0] == branches[1]:
                        branches[0].sub_order = branches[2].order   
                        branches[1].sub_order = branches[2].order
                        tree.branches[branches[0].order][branches[0].sub_order].append(branches[0])
                        tree.branches[branches[1].order][branches[1].sub_order].append(branches[1])
                        branches[2].bonds.append(new_bond)
                        branches[2].free_end = new_end
                        processing[branches[2].free_end].append(branches[2])
                    elif branches[1] == branches[2]:
                        subOrders = [branches[1].order, branches[1].order+1]                
                        shuffle(subOrders)
                        branches[0].sub_order = subOrders[0]
                        branches[1].sub_order = branches[1].order
                        branches[2].sub_order = branches[2].order
                        tree.branches[branches[0].order][branches[0].sub_order].append(branches[0])
                        tree.branches[branches[1].order][branches[1].sub_order].append(branches[1])
                        tree.branches[branches[2].order][branches[2].sub_order].append(branches[2])
                        processing[new_end].append(branch_class(new_bond, new_end, branches[2].order+1))
                    else:
                        subOrders = [branches[1].order, branches[2].order]
                        shuffle(subOrders)
                        branches[0].sub_order = subOrders[0]  
                        branches[1].sub_order = branches[2].order
                        tree.branches[branches[0].order][branches[0].sub_order].append(branches[0])
                        tree.branches[branches[1].order][branches[1].sub_order].append(branches[1])
                        branches[2].bonds.append(new_bond)
                        branches[2].free_end = new_end
                        processing[branches[2].free_end].append(branches[2])
    for garbage in garbage_list:
        del ordered_bonds[garbage]
        del pending[garbage]
    for key in processing:
        if key in pending:
            for item in processing[key]:
                pending[key].append(item)
        else:
            pending[key] = processing[key]
    
def transpose(grid):
    return zip(*grid)

def removeBlankRows(grid):
    return [list(row) for row in grid if any(row)]

tree.branches = removeBlankRows(transpose(removeBlankRows(transpose(tree.branches))))
tree.max_order = len(tree.branches)
branch_freq = np.array([[len(j) for j in i] for i in tree.branches])


np.seterr(invalid= 'ignore')
length_order = np.nan_to_num(np.array([[(np.array([(np.array(k.bonds)).size for k in j])).mean() for j in i] for i in tree.branches]))
length_order = np.append(length_order, np.average(length_order, axis=1, weights=branch_freq).reshape((length_order.shape[0], 1)), axis=1)
branch_freq = np.append(branch_freq, np.sum(branch_freq, 1).reshape((branch_freq.shape[0], 1)), axis=1)
takunaga = np.zeros((len(tree.branches)-1, len(tree.branches)-1))
for i in range(len(tree.branches)-1):
    for j in range(i, len(tree.branches)-1):
        takunaga[i, j] = branch_freq[i, j+1]/(1.0* branch_freq[j+1, -1])
k_array = np.array(range(1, len(tree.branches)))

Tk = np.zeros(k_array.size)
n = len(k_array)
for k in range(k_array.size):
    for i in range(0, n-k):
        Tk[k] += takunaga[i, i+k]
    Tk[k] = Tk[k]/(1.0*(n-k))
        
np.savetxt("takunagaFreq.csv", branch_freq)
np.savetxt("takunagaLeng.csv", length_order)
np.savetxt("takunaga.csv", takunaga)

N = branch_freq[:,-1]
r = length_order[:,-1]

slope, intercept, r_value, p_value, std_err = stats.linregress(np.log10(r), np.log10(N))
line = np.power(10, intercept)*np.power(r, slope)

plt.plot(r, N)
plt.plot(r, line, color='black', zorder=2)
plt.figtext(0.8, 0.8,"$N = {{ {0:0.0f} }} r^{{ {1:f} }}$".format(np.power(10, intercept), slope), ha='right')
plt.xlabel('$r$')
plt.ylabel('$N$')
plt.xscale('log')
plt.yscale('log')
plt.savefig('takunagaDim.pdf')

slope, intercept, r_value, p_value, std_err = stats.linregress(k_array[1:-1], np.log10(Tk[1:-1]))
c = np.power(10, slope)
a = c*np.power(10, intercept)
line = a*np.power(c, k_array-1)
plt.figure()
plt.plot(k_array, Tk)
plt.plot(k_array, line, color='black', zorder=2)
plt.figtext(0.2, 0.8,"$T_k={{ {0:0.2f} }}  \\times \, {{ {1:0.2f} }}$".format(a, c) + "$^{k+1}$", ha='left')
plt.xlabel('$k$')
plt.ylabel('$T_k$')
plt.yscale('log')
plt.savefig('takunagaBranchings.pdf')



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
            
    
