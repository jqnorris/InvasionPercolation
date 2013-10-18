/*
 * main.cpp
 *
 *  Created on: May 20, 2013
 *      Author: jqnorris
 */

#include <map>
#include <set>
#include <deque>
#include <stdlib.h>
#include <cmath>
#include <fstream>

typedef std::pair<int, int> Site;
typedef std::set<Site> SiteList;
typedef std::pair<double, std::pair<Site, Site> > Bond;
typedef std::multimap<double, Bond> BondMap;


SiteList    invadedSites;
BondMap     accessibleBonds;
std::deque<Bond> growth;
std::deque<Bond> trapped;

Site get_neighbor(Site pos, unsigned int direction)
{
	switch (direction)
	{
	case 0: // up
		return std::make_pair(pos.first, pos.second+1);
	case 1: // down
		return std::make_pair(pos.first, pos.second-1);
	case 2: // left
		return std::make_pair(pos.first-1, pos.second);
	case 3: // right
		return std::make_pair(pos.first+1, pos.second);
	}
}

bool invade_bond(Bond & inv_bond)
{
	Site invSite = inv_bond.second.second;
	if (invadedSites.count(invSite) > 0){
		trapped.push_back(inv_bond);
		return true;
	}


	double newStrength;
	Site newNeighbor;
	Bond newBond;
	growth.push_back(inv_bond);
	invadedSites.insert(invSite);
	for (int dir=0;dir<4;++dir)
	{
		newNeighbor = get_neighbor(invSite, dir);
		if (invadedSites.count(newNeighbor) == 0)
		{
			newStrength = drand48();
			if(newStrength < 0.6 || accessibleBonds.size() < 10000)
			{
				newBond = std::make_pair(newStrength, std::make_pair(invSite, newNeighbor));
				accessibleBonds.insert(std::make_pair(newStrength, newBond));
			}
		}
	}
	return false;
}

long int i, N;
bool already_invaded;
const int num_r_values = 100;
const double log_r_max = 6;
const double log_r_min = 0.1;
double r[num_r_values];
const int num_runs = 10;
long int M_array[num_runs][num_r_values];

void initialize_sim(void)
{
	Site start = std::make_pair(0, 0);
	invadedSites.insert(start);
	for (int neigh=0;neigh<4; neigh++)
	{
		Bond newBond;
		double newStrength = drand48();
		newBond = std::make_pair(newStrength, std::make_pair(start, get_neighbor(start, neigh)));
		accessibleBonds.insert(std::make_pair(newStrength, newBond));
	}
}

void run_sim(void)
{
	for (i=0; i<N;)
	{
		BondMap::iterator weakest;
		weakest = accessibleBonds.begin();
		already_invaded = invade_bond(weakest->second);
		if(!already_invaded)
		{
			i++;
		}
		accessibleBonds.erase(weakest);
	}
}

void write_sim_to_file(void)
{
	std::ofstream toFile1("fractures.txt", std::ios::trunc);
		std::ofstream toFile2("trapped.txt", std::ios::trunc);
		toFile1 << growth.size() << "\n";
		toFile2 << trapped.size() << "\n";
		toFile1 << "Invasion for: temp" << "\n";
		toFile2 << "Trapping for: temp" << "\n";
		toFile1.precision(17);
		toFile2.precision(17);

		Bond current_Line;
		while (!growth.empty())
		{
			current_Line = growth.front();
			growth.pop_front();
			toFile1 <<  current_Line.first << "\t";
			toFile1 <<  current_Line.second.first.first << "\t";
			toFile1 <<  current_Line.second.first.second << "\t";
			toFile1 <<  current_Line.second.second.first << "\t";
			toFile1 <<  current_Line.second.second.second << "\n";
		}


		while (!trapped.empty())
		{
			current_Line = trapped.front();
			trapped.pop_front();
			toFile2 <<  current_Line.first << "\t";
			toFile2 <<  current_Line.second.first.first << "\t";
			toFile2 <<  current_Line.second.first.second << "\t";
			toFile2 <<  current_Line.second.second.first << "\t";
			toFile2 <<  current_Line.second.second.second << "\n";
		}

		  toFile1.close();
		  toFile2.close();
}

void setup_r_values(void)
{
	double temp;
	for(int i=0; i<num_r_values; i++)
	{
		temp = log_r_min + static_cast< double >((log_r_max-log_r_min)* i)/ static_cast< float >(num_r_values);
		r[i] = pow(10.0, temp);
	}
}

std::multiset<long long int> r_squared_array;

void record_sim(int i)
{
	std::deque<Bond>::iterator iter;
	long long int temp;

	for(iter=growth.begin(); iter!=growth.end(); iter++)
	{
		temp = (iter->second.second.first) * (iter->second.second.first) + (iter->second.second.second) * (iter->second.second.second);
		r_squared_array.insert(temp);
	}

	int count = 0;

	std::multiset<long long int>::iterator iter2 = r_squared_array.begin();

	for(int j=0; j<num_r_values; j++)
	{

		while(count<growth.size() && *iter2 < r[j]*r[j])
		{
			count++;
			iter2++;
		}
		M_array[i][j] = count;
	}

}

void clear_sim(void)
{
	invadedSites.clear();
	accessibleBonds.clear();
	growth.clear();
	trapped.clear();
	r_squared_array.clear();
}

void write_runs_to_file(void)
{
	std::ofstream toFile1("cluster_masses.txt", std::ios::trunc);
	toFile1 << num_runs << "\t" << num_r_values << "\t" << N << "\n";
	toFile1 << "Cluster mass as a function of radius" << "\n";

	for(int j=0; j< num_r_values; j++)
	{
		toFile1 << r[j] << "\t";
	}
	toFile1 << "\n";

	for(int j=0; j< num_runs; j++)
	{
		for(int k=0; k< num_r_values; k++)
		{
			toFile1 << M_array[j][k] << "\t";
		}
		toFile1 << "\n";
	}

	toFile1.close();
}

int main(int argc, char **argv)
{
	srand48(time(NULL));
	N = atoi(argv[1]);
	setup_r_values();

	for(int j = 0; j< num_runs-1; j++)
	{
		initialize_sim();
		run_sim();
		record_sim(j);
		clear_sim();
	}

	initialize_sim();
	run_sim();
	record_sim(num_runs-1);
	write_sim_to_file();

	write_runs_to_file();

	return 0;
}



