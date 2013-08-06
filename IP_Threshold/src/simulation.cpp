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
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

typedef std::pair<int, int> location;
class Site
{
public:
	location loc;
	double weight;
	bool invaded;
	bool operator > (Site other) const
	{
		return loc > other.loc;
	}
	bool operator >= (Site other) const
	{
		return loc >= other.loc;
	}
	bool operator < (Site other) const
	{
		return loc < other.loc;
	}
	bool operator <= (Site other) const
	{
		return loc <= other.loc;
	}
	bool operator == (Site other) const
	{
		return loc == other.loc;
	}
};
typedef std::set<Site> SiteList;
typedef std::pair<double, std::pair<Site, Site> > Bond;
typedef std::multimap<double, Bond> BondMap;




SiteList    invadedSites;
BondMap     accessibleBonds;
std::deque<Bond> growth;
std::deque<Bond> trapped;
double threshold;

Site get_neighbor(Site site, unsigned int direction)
{
	Site temp;
	switch (direction)
	{
		case 0: // up
			temp.loc = std::make_pair(site.loc.first, site.loc.second+1);
            temp.weight = 0;
			return temp;
		case 1: // down
			temp.loc = std::make_pair(site.loc.first, site.loc.second-1);
			temp.weight = 0;
			return temp;
		case 2: // left
			temp.loc = std::make_pair(site.loc.first-1, site.loc.second);
			temp.weight = 0;
			return temp;
		case 3: // right
			temp.loc = std::make_pair(site.loc.first+1, site.loc.second);
			temp.weight = 0;
			return temp;
	}
}

void invade_bond(Bond & inv_bond)
{
	Site invSite = inv_bond.second.second;

	double newStrength;
	double tempWeight = inv_bond.first;
	Site newNeighbor;
	Bond newBond;
	growth.push_back(inv_bond);

	std::deque<Bond> tempList;

    for (int dir=0;dir<4;++dir)
    {
    	newNeighbor = get_neighbor(invSite, dir);
    	if (invadedSites.count(newNeighbor) == 0)
    	{
    		newStrength = drand48();
    		tempWeight += newStrength;
        	newBond = std::make_pair(newStrength, std::make_pair(invSite, newNeighbor));
        	tempList.push_back(newBond);
    	}
    }

    invSite.weight = tempWeight;
    invadedSites.insert(invSite);
    if(invSite.weight < threshold)
    {
    	while (!tempList.empty())
		{
    		newBond = tempList.front();
    		tempList.pop_front();
    		accessibleBonds.insert(std::make_pair(newBond.first, newBond));
		}
    }
    else
    {
    	while (!tempList.empty())
    	{
    	    newBond = tempList.front();
    	    tempList.pop_front();
    	    trapped.push_back(newBond);
    	}
    };
}


int main(int argc, char **argv)
{
	long int i, N;
	bool already_invaded;

	N = atoi(argv[1]);
	threshold = atof(argv[2]);
	srand48(atoi(argv[3]));

    Site start;
    start.loc = std::make_pair(0, 0);
    start.weight = 0;
    invadedSites.insert(start);
    for (int neigh=0;neigh<4; neigh++)
    {
    	Bond newBond;
    	double newStrength = drand48();
    	newBond = std::make_pair(newStrength, std::make_pair(start, get_neighbor(start, neigh)));
    	accessibleBonds.insert(std::make_pair(newStrength, newBond));
    }

    while(!accessibleBonds.empty())
    {
    	BondMap::iterator weakest;
    	weakest = accessibleBonds.begin();
    	accessibleBonds.erase(weakest);
    	invade_bond(weakest->second);
    	i++;
    	if(i == N)
    	{
    		accessibleBonds.clear();
    	}
    }

    std::stringstream fileName;
    fileName << "fractures" << argv[3] << ".txt";
	std::ofstream toFile1(fileName.str().c_str(), std::ios::trunc);
//	std::ofstream toFile2("trapped.txt", std::ios::trunc);
	toFile1 << growth.size() << "\n";
//	toFile2 << trapped.size() << "\n";
//	toFile1 << "Invasion for: temp" << "\n";
//	toFile2 << "Trapping for: temp" << "\n";
//	toFile1.precision(17);
//	toFile2.precision(17);
//
//	Bond current_Line;
//	while (!growth.empty())
//	{
//		current_Line = growth.front();
//		growth.pop_front();
//		toFile1 <<  current_Line.first << "\t";
//		toFile1 <<  current_Line.second.first.loc.first << "\t";
//		toFile1 <<  current_Line.second.first.loc.second << "\t";
//		toFile1 <<  current_Line.second.second.loc.first << "\t";
//		toFile1 <<  current_Line.second.second.loc.second << "\n";
//	}
//
//
//	while (!trapped.empty())
//	{
//		current_Line = trapped.front();
//		trapped.pop_front();
//		toFile2 <<  current_Line.first << "\t";
//		toFile2 <<  current_Line.second.first.loc.first << "\t";
//		toFile2 <<  current_Line.second.first.loc.second << "\t";
//		toFile2 <<  current_Line.second.second.loc.first << "\t";
//		toFile2 <<  current_Line.second.second.loc.second << "\n";
//	}
//
//	  toFile1.close();
//	  toFile2.close();

    return 0;
}



