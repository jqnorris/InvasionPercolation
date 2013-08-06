// IP_Clusters.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <deque>
#include <map>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <boost/random.hpp>
#include <boost/random/linear_congruential.hpp>
#include <exception>
#include <algorithm> 

boost::rand48 rng(static_cast<unsigned int>(__rdtsc()));
// boost::rand48 rng(static_cast<unsigned int>(12));
boost::uniform_01<boost::rand48> drand48(rng);

typedef std::pair<short int,short int> Site;
typedef std::pair<Site, short int> Bond;

class unitCell_Square
{
public:
	bool is_occupied;
	double bonds[2];
	unsigned int clusterN;
	unitCell_Square(void)
	{
		is_occupied = false;
		bonds[0] = drand48();
		bonds[1] = drand48();
		clusterN = 0;
	};
};


std::map<Site, unitCell_Square> lattice;
typedef std::map<Site, unitCell_Square>::iterator lattice_iterator;
std::multimap<double, Bond> accessible_Bonds;
std::deque<Bond> cluster;
std::deque<Bond> trappedBonds;
std::map<unsigned int, unsigned int> cluster_map;
unsigned int new_cluster_number = 1;

Site get_end(Bond bond)
{
	Site pos = bond.first;
	short int direction = bond.second;
	switch (direction)
	{
		case 0: // down
            return std::make_pair(pos.first, pos.second-1);
		case 1: // right
            return std::make_pair(pos.first+1, pos.second);
		case 2: // up 
            return std::make_pair(pos.first, pos.second+1);
		case 3: // left
            return std::make_pair(pos.first-1, pos.second);
		default:
			return std::make_pair(0,0);
    }
}

double get_strength(Bond bond)
{
	Site pos = bond.first;
	short int direction = bond.second;
	if(direction < 2)
	{
		return lattice[pos].bonds[direction];
	}

	return lattice[get_end(bond)].bonds[direction-2];
}

void add_new_Bonds(Site & inv_site)
{
	Bond new_bond;
	for(short int dir=0; dir < 4; dir++)
	{
		new_bond = std::make_pair(inv_site, dir);
		if(!lattice.at(get_end(new_bond)).is_occupied)
		{
			accessible_Bonds.insert(std::make_pair(get_strength(new_bond), new_bond));
		}		
	}
}


bool invade_bond(Bond & inv_bond)
{
	Site inv_site = get_end(inv_bond);
	if (lattice.at(inv_site).is_occupied)
	{
		trappedBonds.push_back(inv_bond);
		return true;
	}
	else
	{
		cluster.push_back(inv_bond);
		lattice.at(inv_site).is_occupied = true;
		add_new_Bonds(inv_site);
	}
    return false;
}

unsigned int find(unsigned int x)
{
	unsigned int y = x;
	while(cluster_map[y] != y)
	{
		y = cluster_map[y];
	}
	
	while(cluster_map[x] != x)
	{
		unsigned int z = cluster_map[x];
		cluster_map[x] = y;
		x = z;
	}
	return y;
}

unsigned int combine(unsigned int x, unsigned int y)
{
	unsigned int w = std::max(find(x), find(y));
	unsigned int z = std::min(find(x), find(y));
	return cluster_map[w] = z;
}

unsigned int hk_algorithm(unsigned int left, unsigned int up)
{
	if(left != up || up ==0)
	{
		switch(!!left + !!up)
		{
		case 0:
			cluster_map[new_cluster_number] = new_cluster_number;
			return new_cluster_number++;
		case 1:
			return std::max(left, up);
		case 2:
			return combine(left, up);
		}
	}
	return left;
}

int main(int argc, char **argv)
{
	long int i, N;
	bool already_invaded;
	short int L, j, k;
	double pc = 0.5;
	L = atoi(argv[1]);

	N = L*L;

	for(j=-L/2; j < L/2; j++)
	{
		for(k=-L/2+1; k < L/2+1; k++)
		{
			Site loc = std::make_pair(j, k);
			unitCell_Square cell;
			lattice.insert(std::make_pair(loc, cell));
		}
	}

	Site start = std::make_pair(0,0);
	lattice[start].is_occupied=true;
	add_new_Bonds(start);
	try{
		for (i=0; i<N;)
		{
    		std::multimap<double, Bond>::iterator weakest;
    		weakest = accessible_Bonds.begin();
    		already_invaded = invade_bond(weakest->second);
    		if(!already_invaded)
    		{
    			i++;
    		}
    		accessible_Bonds.erase(weakest);
		}
	}
	catch (std::exception& e) {}

	std::ofstream toFile1("fractures.txt", std::ios::trunc);
	std::ofstream toFile2("trapped.txt", std::ios::trunc);
	toFile1 << cluster.size() << "\n";
	toFile2 << trappedBonds.size() << "\n";
	toFile1 << "Invasion for: temp" << "\n";
	toFile2 << "Trapping for: temp" << "\n";
	toFile1.precision(17);
	toFile2.precision(17);

	Bond current_Line;
	while (!cluster.empty())
	{
		current_Line = cluster.front();
		cluster.pop_front();
		toFile1 << get_strength(current_Line) << "\t";
		toFile1 <<  current_Line.first.first << "\t";
		toFile1 <<  current_Line.first.second << "\t";
		Site other_end = get_end(current_Line);
		toFile1 <<  other_end.first << "\t";
		toFile1 <<  other_end.second << "\n";
	}


	while (!trappedBonds.empty())
	{
		current_Line = trappedBonds.front();
		trappedBonds.pop_front();
		toFile2 << get_strength(current_Line) << "\t";
		toFile2 <<  current_Line.first.first << "\t";
		toFile2 <<  current_Line.first.second << "\t";
		Site other_end = get_end(current_Line);
		toFile2 <<  other_end.first << "\t";
		toFile2 <<  other_end.second << "\n";
	}

	toFile1.close();
	toFile2.close();

	unitCell_Square * current_cell;
	unitCell_Square * next_cell;
	for(j=-L/2; j < L/2-1; j++)
	{
		lattice[std::make_pair(j, L/2)].clusterN = hk_algorithm(lattice[std::make_pair(j, L/2)].clusterN,0);
		for(k=L/2; k > -L/2+1; k--)
		{
			current_cell = &lattice[std::make_pair(j,k)];
			// Down
			next_cell = &lattice[get_end(std::make_pair(std::make_pair(j,k), 0))];
			if(current_cell->bonds[0] < pc)
			{
				next_cell->clusterN = hk_algorithm(next_cell->clusterN, current_cell->clusterN);
			}
			else
			{
				next_cell->clusterN = hk_algorithm(next_cell->clusterN, 0);
			}
			// Right
			next_cell = &lattice[get_end(std::make_pair(std::make_pair(j,k), 1))];
			if(current_cell->bonds[1] < pc)
			{
				next_cell->clusterN = current_cell->clusterN;
			}
		}
		// Bottom Rights
		current_cell = &lattice[std::make_pair(j,k)];
		next_cell = &lattice[get_end(std::make_pair(std::make_pair(j,k), 1))];
		if(current_cell->bonds[1] < pc)
		{
			next_cell->clusterN = hk_algorithm(next_cell->clusterN, current_cell->clusterN);
		}
		else
		{
			next_cell->clusterN = hk_algorithm(next_cell->clusterN, 0);
		}
	}
	// Right Downs
	lattice[std::make_pair(j, L/2)].clusterN = hk_algorithm(lattice[std::make_pair(j, L/2)].clusterN,0);
	for(k=L/2; k > -L/2+1; k--)
	{
		current_cell = &lattice[std::make_pair(j,k)];
		// Down
		next_cell = &lattice[get_end(std::make_pair(std::make_pair(j,k), 0))];
		if(current_cell->bonds[0] < pc)
		{
			next_cell->clusterN = hk_algorithm(next_cell->clusterN, current_cell->clusterN);
		}
		else
		{
			next_cell->clusterN = hk_algorithm(next_cell->clusterN, 0);
		}
	}
	
	for(j=-L/2; j < L/2; j++)
	{
		for(k=-L/2+1; k < L/2+1; k++)
		{
			current_cell = &lattice[std::make_pair(j,k)];
			current_cell->clusterN = find(current_cell->clusterN);
		}
	}
	long int bond_count = 0;
	for(lattice_iterator iter = lattice.begin(); iter != lattice.end(); iter++)
	{
		current_cell = &iter->second;
		if(current_cell->bonds[0] < pc)
		{
			bond_count++;
		}
		if(current_cell->bonds[1] < pc)
		{
			bond_count++;
		}
	}


	std::ofstream toFile3("clusters.txt", std::ios::trunc);
	toFile3 << bond_count << "\n";

	unitCell_Square Line;
	Site begin;
	Site other_end;
	std::map<int, unsigned int> cluster_sizes;
	while (!lattice.empty())
	{
		begin = lattice.begin()->first;
		Line = lattice.begin()->second;
		lattice.erase(begin);
		if(Line.bonds[0] < pc)
		{
			if(cluster_sizes.count(Line.clusterN) > 0)
			{
				cluster_sizes[Line.clusterN]++;
			}
			else
			{
				cluster_sizes.insert(std::make_pair(Line.clusterN,1));
			}
			toFile3 << Line.clusterN << "\t";
			toFile3 << begin.first << "\t";
			toFile3 << begin.second << "\t";
			other_end = get_end(std::make_pair(begin, 0));
			toFile3 <<  other_end.first << "\t";
			toFile3 <<  other_end.second << "\n";
		}
		if(Line.bonds[1] < pc)
		{
			if(cluster_sizes.count(Line.clusterN) > 0)
			{
				cluster_sizes[Line.clusterN]++;
			}
			else
			{
				cluster_sizes.insert(std::make_pair(Line.clusterN,1));
			}
			toFile3 << Line.clusterN << "\t";
			toFile3 << begin.first << "\t";
			toFile3 << begin.second << "\t";
			other_end = get_end(std::make_pair(begin, 1));
			toFile3 <<  other_end.first << "\t";
			toFile3 <<  other_end.second << "\n";
		}
	}

	toFile3.close();

	std::map<unsigned int, unsigned int> cluster_count;
	unsigned int size, begin2;
	while (!cluster_sizes.empty())
	{
		begin2 = cluster_sizes.begin()->first;
		size = cluster_sizes.begin()->second;
		cluster_sizes.erase(begin2);
		if(cluster_count.count(size) > 0)
			{
				cluster_count[size]++;
			}
			else
			{
				cluster_count.insert(std::make_pair(size, 1));
			}
	}

	std::ofstream toFile4("cluster_distribution.txt", std::ios::trunc);
	toFile4 << cluster_count.size() << "\n";

	unsigned int count;
	while (!cluster_count.empty())
	{
		size = cluster_count.begin()->first;
		count = cluster_count.begin()->second;
		cluster_count.erase(size);
		toFile4 << size << "\t" << count << "\n";
	}

	toFile4.close();

    return 0;
}

