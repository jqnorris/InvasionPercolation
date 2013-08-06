// InvasionPercolation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <map>
#include <set>
#include <deque>
#include <stdlib.h>
#include <fstream>
#include <boost/random.hpp>
#include <boost/random/linear_congruential.hpp>

double drand48(void)
{
  boost::rand48 rng;
  static boost::uniform_01<boost::rand48> zeroone(rng);
  return zeroone();
}

typedef std::tuple<int,int,int> Site;
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
		case 0: // left
            return std::make_tuple( std::get<0>(pos), std::get<1>(pos)-1, std::get<2>(pos));
		case 1: // right
            return std::make_tuple( std::get<0>(pos), std::get<1>(pos)+1, std::get<2>(pos));
		case 2: // front
            return std::make_tuple( std::get<0>(pos)+1, std::get<1>(pos), std::get<2>(pos));
		case 3: // back
            return std::make_tuple( std::get<0>(pos)-1, std::get<1>(pos), std::get<2>(pos));
		case 4: // up
            return std::make_tuple( std::get<0>(pos), std::get<1>(pos), std::get<2>(pos)+1);
		case 5: // down
            return std::make_tuple( std::get<0>(pos), std::get<1>(pos), std::get<2>(pos)-1);
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
    for (int dir=0; dir<6; dir++)
    {
    	newNeighbor = get_neighbor(invSite, dir);
    	if (invadedSites.count(newNeighbor) == 0)
    	{
    		newStrength = drand48();
       		newBond = std::make_pair(newStrength, std::make_pair(invSite, newNeighbor));
        	accessibleBonds.insert(std::make_pair(newStrength, newBond));
    	}
    }
    return false;
}


int main(int argc, char **argv)
{
	long int i, N;
	bool already_invaded;

	N = atoi(argv[1]);
    Site start = std::make_tuple(0, 0, 0);
    invadedSites.insert(start);
    for (int neigh=0;neigh<4; neigh++)
    {
    	Bond newBond;
    	double newStrength = drand48();
    	newBond = std::make_pair(newStrength, std::make_pair(start, get_neighbor(start, neigh)));
    	accessibleBonds.insert(std::make_pair(newStrength, newBond));
    }

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
		toFile1 <<  std::get<0>(current_Line.second.first) << "\t";
		toFile1 <<  std::get<1>(current_Line.second.first) << "\t";
		toFile1 <<  std::get<2>(current_Line.second.first) << "\t";
		toFile1 <<  std::get<0>(current_Line.second.second) << "\t";
		toFile1 <<  std::get<1>(current_Line.second.second) << "\t";
		toFile1 <<  std::get<2>(current_Line.second.second) << "\n";
	}


	while (!trapped.empty())
	{
		current_Line = trapped.front();
		trapped.pop_front();
		toFile2 <<  current_Line.first << "\t";
		toFile2 <<  std::get<0>(current_Line.second.first) << "\t";
		toFile2 <<  std::get<1>(current_Line.second.first) << "\t";
		toFile2 <<  std::get<2>(current_Line.second.first) << "\t";
		toFile2 <<  std::get<0>(current_Line.second.second) << "\t";
		toFile2 <<  std::get<1>(current_Line.second.second) << "\t";
		toFile2 <<  std::get<2>(current_Line.second.second) << "\n";
	}

	  toFile1.close();
	  toFile2.close();

    return 0;
}

