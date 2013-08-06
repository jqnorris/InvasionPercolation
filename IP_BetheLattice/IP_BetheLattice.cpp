// IP_BetheLattice.cpp : Defines the entry point for the console application.
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

typedef long int Site;
typedef std::set<Site> SiteList;
typedef std::pair<double, std::pair<Site, Site> > Bond;
typedef std::multimap<double, Bond> BondMap;

SiteList    invadedSites;
BondMap     accessibleBonds;
std::deque<Bond> growth;
long int z=3;

bool invade_bond(Bond & inv_bond)
{
	Site invSite = inv_bond.second.second;

	double newStrength;
	Site newNeighbor;
	Bond newBond;
	growth.push_back(inv_bond);
	invadedSites.insert(invSite);
    for (int dir=0; dir<z-1; ++dir)
    {
    		newStrength = drand48();
			newNeighbor = 3*(invSite+1)+dir;
       		newBond = std::make_pair(newStrength, std::make_pair(invSite, newNeighbor));
        	accessibleBonds.insert(std::make_pair(newStrength, newBond));
    }
    return false;
}


int main(int argc, char **argv)
{
	long int i, N;
	bool already_invaded;

	z = atoi(argv[1]);
	N = atoi(argv[2]);

    Site start = -1;
    invadedSites.insert(start);
    for (int dir=0; dir<z; ++dir)
    {
    		double newStrength = drand48();
			Site newNeighbor = 3*(start+1)+dir;
       		Bond newBond = std::make_pair(newStrength, std::make_pair(start, newNeighbor));
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
	toFile1 << growth.size() << "\n";
	toFile1 << "Invasion for: temp" << "\n";
	toFile1.precision(17);

	Bond current_Line;
	while (!growth.empty())
	{
		current_Line = growth.front();
		growth.pop_front();
		toFile1 << current_Line.first << "\t";
		toFile1 << current_Line.second.first << "\t";
		toFile1 << current_Line.second.second<< "\n";
	}

	  toFile1.close();

    return 0;
}

