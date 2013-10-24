

from heapq import *
import numpy as np
import matplotlib.pyplot as plt
import random
import operator

class stuple(tuple):
    def __add__(self, other):
        return self.__class__(map(operator.add, self, other))

accessible_bonds = []

lattice = []

invaded_sites = set()

trapped = []

growth = []


directions = [stuple([0,-1]),
             stuple([1,0])]

def invade_bond(inv_bond):
    inv_site = inv_bond[1][1]
    if inv_site in invaded_sites:
        trapped.append(inv_bond)
        return True
    growth.append(inv_bond)
    invaded_sites.add(inv_site)
    for direc in directions:
        new_neighbor = inv_site + direc
        if (new_neighbor not in invaded_sites):
            new_strength = random.random()
            heappush(accessible_bonds, stuple([new_strength, stuple([inv_site, new_neighbor])]))
    return False

N = 1000

start = stuple([0,0])

invaded_sites.add(start)

for direc in directions:
        new_neighbor = start + direc
        new_strength = random.random()
        heappush(accessible_bonds, stuple([new_strength, stuple([start, new_neighbor])]))

step = 0
while step < N:
    weakest = heappop(accessible_bonds)
    already_invaded = invade_bond(weakest)
    if not already_invaded:
        step += 1

strength = np.array([i[0] for i in growth])
x1 = np.array([i[1][0][0] for i in growth])
y1 = np.array([i[1][0][1] for i in growth])
x2 = np.array([i[1][1][0] for i in growth])
y2 = np.array([i[1][1][1] for i in growth])
    
plt.hist(strength, bins=500, range=(0, 1), histtype='stepfilled')
plt.title("Histogram of Failed Probabilites")
plt.ylabel('$N$', fontsize=20)
plt.xlabel('$p$', fontsize=20)
plt.axis('tight')
plt.savefig('histogram.pdf')

size = 10
minX = x2.min()
maxX = x2.max()
minY = y2.min()
maxY = y2.max()

deltaX = maxX - minX
midX = minX+0.5*deltaX
deltaY = maxY - minY
midY = minY + 0.5*deltaY
L = max([deltaX, deltaY])

if N < 5000:
    plt.figure(figsize=(size, size))
    for i in range(0, x1.size):
            plt.plot([x1[i], x2[i]], [y1[i], y2[i]], color='black', linewidth = size/(1.0*L)*5, zorder = 2, solid_capstyle = 'round')
    plt.plot([0], [0], '*', markersize = size/(1.0*L)*120.0, markeredgecolor = 'none', color = 'red', zorder = 5)
    plt.xlim(midX - 0.5*(L+1), midX+0.5*(L+1))
    plt.ylim(midY - 0.5*(L+1), midY+0.5*(L+1))
    plt.axis('off')
    plt.savefig("phylogenicTree.pdf")

