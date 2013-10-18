// IP_Clusters3.cpp : Defines the entry point for the console application.
//

#include <deque>
#include <map>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <boost/random.hpp>
#include <boost/random/linear_congruential.hpp>
#include <exception>
#include <algorithm>

#ifdef _WIN32
//  Windows
#define rdtsc  __rdtsc

#else
//  For everything else
unsigned long long rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}
#endif

boost::rand48 rng(static_cast<unsigned int>(rdtsc()));
//boost::rand48 rng(static_cast<unsigned int>(12));
boost::uniform_01<boost::rand48> boost_drand48(rng);

typedef std::pair<short int,short int> Site;
typedef std::pair<Site, short int> Bond;

class unitCell_Square
{
public:
	bool is_invaded;
	bool is_open[2];
	double bonds[2];
	double bond_failure_time[2];
	unsigned int clusterN;
	unsigned int burstN[2];
	unitCell_Square(void)
	{
		bonds[0] = boost_drand48();
		bonds[1] = boost_drand48();
		is_invaded = false;
		is_open[0] = false;
		is_open[1] =false;
		clusterN = 0;
		burstN[0] = 0;
		burstN[1] = 0;
		bond_failure_time[0] = -1;
		bond_failure_time[1] = -1;
	};
};


std::map<Site, unitCell_Square> lattice;
typedef std::map<Site, unitCell_Square>::iterator lattice_iterator;
typedef std::pair<unsigned int, Site> cluster_seed;
std::multimap<double, Bond> accessible_Bonds;
std::deque<Bond> cluster;
std::deque<Bond> trappedBonds;
std::map<unsigned int, cluster_seed> cluster_map;
unsigned int new_cluster_number = 1;
double pc = 0.5;
double tau_short = 0.5*pow(10, -5);
double tau_long = 1;
double beta_short = 8;
double beta_long = 2;

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

double get_failure_time(Bond bond)
{
	Site pos = bond.first;
	short int direction = bond.second;
	if(direction < 2)
	{
		return lattice[pos].bond_failure_time[direction];
	}

	return lattice[get_end(bond)].bond_failure_time[direction-2];
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

double get_time_to_failure(Bond bond)
{
	if (get_strength(bond) > pc)
	{
//	if (boost_drand48() > pc)
//	{
		//return tau_long*pow(-log(1-boost_drand48()), 1.0/beta_long);
		return tau_long;
	}
	// return tau_short*pow(-log(1-boost_drand48()), 1.0/beta_short);
	return tau_short;
}

void add_new_Bonds(Site & inv_site, double current_time)
{
	Bond new_bond;
	for(short int dir=0; dir < 4; dir++)
	{
		new_bond = std::make_pair(inv_site, dir);
		if (lattice.count(get_end(new_bond)) >> 0)
		{
			double new_failure_time = current_time+get_time_to_failure(new_bond);
			if(!lattice.at(get_end(new_bond)).is_invaded)
			{
				short int direction = new_bond.second;
				if(direction < 2)
				{
					lattice[inv_site].bond_failure_time[direction] = new_failure_time;
				}
				else
				{
					lattice[get_end(new_bond)].bond_failure_time[direction-2] = new_failure_time;
				}
				accessible_Bonds.insert(std::make_pair(new_failure_time, new_bond));
			}
		}		
	}
}

unsigned int burstIter = 1;
bool invade_bond(double current_time, Bond & inv_bond)
{
	Site inv_site = get_end(inv_bond);
	if (lattice.at(inv_site).is_invaded)
	{
		trappedBonds.push_back(inv_bond);
		return true;
	}
	else
	{
		if ( get_strength(inv_bond) < pc)
		{
			lattice.at(inv_bond.first).burstN[inv_bond.second] = burstIter;
		}
		else
		{
			burstIter++;
		}
		cluster.push_back(inv_bond);
		lattice.at(inv_site).is_invaded = true;
		add_new_Bonds(inv_site, current_time);
	}
    return false;
}

unsigned int find(unsigned int x)
{
	unsigned int y = x;
	while(cluster_map[y].first != y)
	{
		y = cluster_map[y].first;
	}
	
	while(cluster_map[x].first != x)
	{
		unsigned int z = cluster_map[x].first;
		cluster_map[x].first = y;
		x = z;
	}
	return y;
}

unsigned int combine(unsigned int x, unsigned int y)
{
	unsigned int w = std::max(find(x), find(y));
	unsigned int z = std::min(find(x), find(y));
	return cluster_map[w].first = z;
}


bool grow_clusters(Bond bond)
{
	unsigned int one = find(lattice[bond.first].clusterN);
	unsigned int two;
	Site random_end;
	if(lattice.count(get_end(bond)) > 0)
	{
		 two = find(lattice[get_end(bond)].clusterN);
	}
	else
	{
		two = -1;
	}
	if(two == -1)
	{
		if(one == 0)
		{
			if(boost_drand48() < 0.5)
			{
				random_end = bond.first;
			}
			else
			{
				random_end = get_end(bond);
			}
			cluster_map[new_cluster_number] = std::make_pair(new_cluster_number, random_end);
			lattice[bond.first].clusterN = new_cluster_number;
			new_cluster_number++;
		}
		return true;
	}
	if( (one != two || one == 0))
	{
		switch(!!one + !!two)
		{
		case 0:
			if(boost_drand48() < 0.5)
			{
				random_end = bond.first;
			}
			else
			{
				random_end = get_end(bond);
			}
			cluster_map[new_cluster_number] = std::make_pair(new_cluster_number, random_end);
			lattice[bond.first].clusterN = new_cluster_number;
			lattice[get_end(bond)].clusterN = lattice[bond.first].clusterN;
			new_cluster_number++;
			return true;
		case 1:
			lattice[bond.first].clusterN = std::max(one, two);
			lattice[get_end(bond)].clusterN = std::max(one, two);
			return true;
		case 2:
			lattice[bond.first].clusterN = combine(one, two);
			lattice[get_end(bond)].clusterN = lattice[bond.first].clusterN;
			return true;
		}
	}
	return false;
}

int main(int argc, char **argv)
{
	long int i, N, W;
	bool already_invaded;
	short int L, j, k, l, aspect_ratio;
	L = atoi(argv[1]);
	aspect_ratio = atoi(argv[2]);
	W = L * aspect_ratio;

	N = L*W;

	for(j=-W/2; j < W/2; j++)
	{
		for(k=-L/2+1; k < L/2+1; k++)
		{
			Site loc = std::make_pair(j, k);
			unitCell_Square cell;
			lattice.insert(std::make_pair(loc, cell));
		}
	}

	Site start = std::make_pair(0,0);
	lattice[start].is_invaded = true;
	add_new_Bonds(start, 0);
	try
	{
		while(accessible_Bonds.size() > 0 )
		{
    		std::multimap<double, Bond>::iterator weakest;
    		weakest = accessible_Bonds.begin();
    		already_invaded = invade_bond(weakest->first, weakest->second);
    		accessible_Bonds.erase(weakest);
		}
		std::multimap<double, Bond>::iterator weakest = accessible_Bonds.begin();
		cluster.push_back(weakest->second);
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
		toFile1 << get_failure_time(current_Line) << "\t";
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
		toFile2 << get_failure_time(current_Line) << "\t";
		toFile2 <<  current_Line.first.first << "\t";
		toFile2 <<  current_Line.first.second << "\t";
		Site other_end = get_end(current_Line);
		toFile2 <<  other_end.first << "\t";
		toFile2 <<  other_end.second << "\n";
	}

	toFile1.close();
	toFile2.close();


	accessible_Bonds.clear();
	for(j=-W/2; j < W/2; j++)
	{
		for(k=-L/2+1; k < L/2+1; k++)
		{
			for(l=0; l<2; l++)
			{
				Site loc = std::make_pair(j, k);
				Bond bond = std::make_pair(loc, l);
				accessible_Bonds.insert(std::make_pair(lattice[loc].bonds[l], bond));
			}
		}
	}

	std::multimap<double, Bond>::iterator weakest = accessible_Bonds.begin();
	while(weakest->first < pc)
	{	
		lattice[weakest->second.first].is_open[weakest->second.second] = grow_clusters(weakest->second);
		accessible_Bonds.erase(weakest);
		weakest = accessible_Bonds.begin();
	}


	unitCell_Square * current_cell;
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
		if(current_cell->is_open[0])
		{
			bond_count++;
		}
		if(current_cell->is_open[1])
		{
			bond_count++;
		}
	}

	typedef std::pair<unsigned int, unsigned int> mapping;
	std::map<mapping, unsigned int> burst_cluster_map;
	for(lattice_iterator iter = lattice.begin(); iter != lattice.end(); iter++)
	{
		for(short int n=0; n < 2; n++)
		{
			if(iter->second.burstN[n] != 0)
			{
				mapping this_map = std::make_pair(iter->second.burstN[n], find(iter->second.clusterN));
				if(burst_cluster_map.count(this_map) > 0)
				{
					burst_cluster_map[this_map]++;
			}	
				else
				{
					burst_cluster_map.insert(std::make_pair(this_map,1));
				}
			}
		}
	}
	

	std::ofstream toFileX("burstClusters.txt", std::ios::trunc);
	toFileX << burst_cluster_map.size() << "\n";
	for(std::map<mapping, unsigned int>::iterator iter = burst_cluster_map.begin(); iter != burst_cluster_map.end(); iter++)
	{
		toFileX << iter->first.first << "\t" << iter->first.second << "\t" << iter->second << "\n";
	}
	
	toFileX.close();

	std::ofstream toFile3("clusters.txt", std::ios::trunc);
	toFile3 << bond_count << "\n";

	unitCell_Square Line;
	Site begin;
	Site other_end;
	std::map<unsigned int, unsigned int> cluster_sizes;
	while (!lattice.empty())
	{
		begin = lattice.begin()->first;
		Line = lattice.begin()->second;
		lattice.erase(begin);
		if(Line.is_open[0])
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
		if(Line.is_open[1])
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
	unsigned int largest_cluster = 0;
	unsigned int largest_cluster_size = 0;
	std::map<unsigned int, unsigned int>::iterator iter;
	for(iter = cluster_sizes.begin(); iter != cluster_sizes.end(); iter++)
	{
		if(iter->second > largest_cluster_size)
		{
			largest_cluster = iter->first;
			largest_cluster_size = iter->second;
		}
	}

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
	toFile4 << cluster_count.size() << "\t" << largest_cluster << "\t";
	toFile4 << cluster_map[largest_cluster].second.first << "\t" << cluster_map[largest_cluster].second.second << "\n";

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

