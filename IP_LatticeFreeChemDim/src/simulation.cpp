/*
 * simulation.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: jqnorris
 */

#include "simulation.h"
#include <boost/random.hpp>
#include <boost/random/linear_congruential.hpp>
#include <map>
#include <set>
#include <deque>
#include <stdlib.h>
#include <fstream>

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

Bond_Data::Bond_Data()
{
	strength = boost_drand48();
	order_added = 0;
	invaded = false;
	blocked = false;
	available = true;
}

Bond_Data::~Bond_Data()
{

}


Simulation::Simulation()
{
	pc = 0.499;

	num_chem_levels = 1000;
	num_realizations = 10000;
	realization = 0;
}

Simulation::~Simulation()
{

}


Site Simulation::get_neighbor(Site pos, unsigned int direction)
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

bool Simulation::bond_is_blocked(Bond & inv_bond)
{
	Site invSite = inv_bond.second;
	if (invaded_sites.count(invSite) == true)
	{
		lattice[inv_bond].available = false;
		lattice[inv_bond].blocked = true;
		lattice[inv_bond].order_added = blocked_index--;
		return true;
	}

	return false;
}

void Simulation::add_new_bonds(Site & inv_site)
{
	Site new_neighbor;
	Bond new_bond;
	for (int dir=0;dir<4;++dir)
	{

		new_neighbor = get_neighbor(inv_site, dir);
		if (invaded_sites.count(new_neighbor) == 0)
		{
			Bond_Data new_bond_data;

			if (new_bond_data.strength < pc)
			{
				new_bond = std::make_pair(inv_site, new_neighbor);
				lattice.insert(std::make_pair(new_bond, new_bond_data));
				accessible_bonds.insert(std::make_pair(new_bond_data.strength, new_bond));
			}
		}
	}
}


bool Simulation::invade_bond(Bond & inv_bond)
{
	if(bond_is_blocked(inv_bond))
	{
		return false;
	}
	else
	{
		lattice[inv_bond].available = false;
		lattice[inv_bond].invaded = true;
		lattice[inv_bond].order_added = time_step++;

		Site inv_site = inv_bond.second;
		invaded_sites.insert(inv_site);
		add_new_bonds(inv_site);
	}

	return true;
}

bool Simulation::step_time()
{
	if(accessible_bonds.size() > 0)
	{
		std::multimap<double, Bond>::iterator weakest;

		weakest = accessible_bonds.begin();
		bool already_invaded = invade_bond(weakest->second);

		accessible_bonds.erase(weakest);
		return true;
	}

	return false;
}

void Simulation::step_time(int number_steps)
{
	for(int i = 0; i < number_steps; i++)
	{
		if(!step_time())
		{
			break;
		}
	}
}

bool Simulation::cluster_terminated()
{
	if(accessible_bonds.size() == 0)
	{
		return true;
	}

	return false;
}

void Simulation::write_cluster()
{
	std::map<long int, Bond> ordered_cluster;
	std::map<long int, Bond>::iterator oc_iter;
	std::map<Bond, Bond_Data>::iterator lattice_iter;

	Bond_Data lattice_bond_data;

	for(lattice_iter = lattice.begin(); lattice_iter != lattice.end(); lattice_iter++)
	{
		lattice_bond_data = lattice_iter->second;

		if(lattice_bond_data.invaded)
		{
			ordered_cluster.insert(std::make_pair(lattice_bond_data.order_added, lattice_iter->first));
		}
	}

	std::ofstream toFile1("fractures.txt", std::ios::trunc);
	toFile1 << ordered_cluster.size() << std::endl;
	toFile1 << "Invasion for: temp" << "\n";
	toFile1.precision(17);

	Bond current_Line;
	for(oc_iter = ordered_cluster.begin(); oc_iter != ordered_cluster.end(); oc_iter++)
	{
		toFile1 <<  lattice[oc_iter->second].strength << "\t";
		toFile1 <<  oc_iter->second.first.first << "\t";
		toFile1 <<  oc_iter->second.first.second << "\t";
		toFile1 <<  oc_iter->second.second.first << "\t";
		toFile1 <<  oc_iter->second.second.second << "\n";
	}

	toFile1.close();
}

void Simulation::write_blocked()
{
	std::set<Bond> ordered_blocked;
	std::set<Bond>::iterator ob_iter;
	std::map<Bond, Bond_Data>::iterator lattice_iter;

	Bond_Data lattice_bond_data;

	for(lattice_iter = lattice.begin(); lattice_iter != lattice.end(); lattice_iter++)
	{
		lattice_bond_data = lattice_iter->second;

		if(lattice_bond_data.blocked)
		{
			ordered_blocked.insert(lattice_iter->first);
		}
	}

	std::ofstream toFile1("blocked.txt", std::ios::trunc);
	toFile1 << ordered_blocked.size() << "\n";
	toFile1 << "Blocked Bonds for: temp" << "\n";
	toFile1.precision(17);

	Bond current_Line;
	for(ob_iter = ordered_blocked.begin(); ob_iter != ordered_blocked.end(); ob_iter++)
	{
		toFile1 <<  lattice[*ob_iter].strength << "\t";
		toFile1 <<  ob_iter->first.first << "\t";
		toFile1 <<  ob_iter->first.second << "\t";
		toFile1 <<  ob_iter->second.first << "\t";
		toFile1 <<  ob_iter->second.second << "\n";
	}

	toFile1.close();
}

void Simulation::dimension_initialize()
{
	lattice.clear();
	accessible_bonds.clear();
	invaded_sites.clear();
	time_step = 1;
	blocked_index = -1;

	chemical_level_num = 0;

	for(int i=0; i<num_chem_levels; i++)
	{
		S_l[i] = 0;

		B_l[i] = 0;
	}

	this_chemical_level_cluster.clear();
	this_chemical_level_blocked.clear();
	next_chemical_level_cluster.clear();
	next_chemical_level_blocked.clear();

	Site origin = std::make_pair(0,0);
	invaded_sites.insert(origin);
	add_new_bonds(origin);

	std::map<double, Bond>::iterator iter;
	if(accessible_bonds.size()>0)
	{
		for(iter=accessible_bonds.begin(); iter != accessible_bonds.end(); iter++)
		{
			waiting_on.insert(iter->second);
		}
	}
}

short int Simulation::get_bond_direction(Bond bond)
{
	short int x_diff = bond.first.first-bond.second.first;
	short int y_diff = bond.first.second-bond.second.second;

	if(x_diff == 0)
	{
		if(y_diff== 1)
		{
			return 0;
		}
		if(y_diff == -1)
		{
			return 1;
		}
	}
	else
	{
		if(x_diff == 1)
		{
			return 3;
		}
	}

	return 2;
}

bool Simulation::check_branch(Bond bond)
{
	if(lattice.count(bond) > 0)
	{
		if(!(lattice[bond].invaded || lattice[bond].blocked))
		{
			return true;
		}
	}

	return false;
}

bool Simulation::check_cluster()
{
	std::set<Bond>::iterator waiting_on_iter;

	if(waiting_on.size() == 0 || invaded_sites.size() > 100000000)
	{
		return true;
	}

	for(waiting_on_iter = waiting_on.begin(); waiting_on_iter != waiting_on.end(); waiting_on_iter++)
	{
		if(lattice[*waiting_on_iter].invaded)
		{
			next_chemical_level_cluster.insert(*waiting_on_iter);
			waiting_on.erase(*waiting_on_iter);
			continue;
		}
		if(lattice[*waiting_on_iter].blocked)
		{
			next_chemical_level_blocked.insert(*waiting_on_iter);
			waiting_on.erase(*waiting_on_iter);
			continue;
		}
	}

	return false;
}

void Simulation::prep_next_level()
{
	std::set<Bond>::iterator this_chem_level_iter;

	Bond this_bond;
	Bond bond_to_add;

	for(this_chem_level_iter = this_chemical_level_cluster.begin(); this_chem_level_iter != this_chemical_level_cluster.end(); this_chem_level_iter++)
	{
		this_bond = *this_chem_level_iter;
		short int not_new = get_bond_direction(this_bond);

		for(int i = 0; i < 4; i++)
		{
			if(i != not_new)
			{
				bond_to_add = std::make_pair(this_bond.second, get_neighbor(this_bond.second, i));

				if(lattice.count(bond_to_add) > 0)
				{
					lattice[bond_to_add].chemical_level = chemical_level_num+1;

					if(lattice[bond_to_add].available)
					{
						waiting_on.insert(bond_to_add);
						continue;
					}
					if(lattice[bond_to_add].invaded)
					{
						next_chemical_level_cluster.insert(bond_to_add);
						continue;
					}
					if(lattice[bond_to_add].blocked)
					{
						next_chemical_level_blocked.insert(bond_to_add);
						continue;
					}
				}
			}
		}
	}

}

void Simulation::advance_level()
{
	record_level_size();

	prep_next_level();

	std::cout << check_cluster() << std::endl;
	while(!check_cluster())
	{
		step_time(100000);
		std::cout << "Step Time" << "\t" << waiting_on.size() << "\t";
		if(waiting_on.size() == 1)
		{
			std::cout << lattice[*waiting_on.begin()].strength << std::endl;
		}
		else
		{
			std::cout << std::endl;
		}
	}

	this_chemical_level_cluster.clear();
	this_chemical_level_blocked.clear();

	this_chemical_level_cluster = next_chemical_level_cluster;
	this_chemical_level_blocked = next_chemical_level_blocked;

	next_chemical_level_cluster.clear();
	next_chemical_level_blocked.clear();

	chemical_level_num++;
}

void Simulation::record_level_size()
{
	S_l[chemical_level_num] = this_chemical_level_cluster.size();
	B_l[chemical_level_num] = this_chemical_level_blocked.size();
}

void Simulation::test_dimension()
{
	std::ofstream toFile3("chemicalLevels.txt", std::ios::trunc);
	toFile3 << num_chem_levels << "\n";
	toFile3 << "Testing" << "\n";

	std::set<Bond>::iterator chem_level_iter;

	for(int i = 0; i<num_chem_levels; i++)
	{
		std::cout << accessible_bonds.size() << std::endl;

		toFile3 << i << "\t";
		toFile3 << 11;
		if(this_chemical_level_cluster.size() >0 && i < 11)
		{
			for(chem_level_iter = this_chemical_level_cluster.begin(); chem_level_iter != this_chemical_level_cluster.end(); chem_level_iter++)
			{
				toFile3 << "\t" << (*chem_level_iter).first.first;
				toFile3 << "\t" << (*chem_level_iter).first.second;
				toFile3 << "\t" << (*chem_level_iter).second.first;
				toFile3 << "\t" << (*chem_level_iter).second.second;
			}
		}
		toFile3 << std::endl;
		advance_level();
		std::cout << "Level Advanced!" << std::endl;
	}

	toFile3 << num_chem_levels << "\t";
	toFile3 << this_chemical_level_cluster.size();
	if(this_chemical_level_cluster.size() >0)
	{
		for(chem_level_iter = this_chemical_level_cluster.begin(); chem_level_iter != this_chemical_level_cluster.end(); chem_level_iter++)
		{
			toFile3 << "\t" << (*chem_level_iter).first.first;
			toFile3 << "\t" << (*chem_level_iter).first.second;
			toFile3 << "\t" << (*chem_level_iter).second.first;
			toFile3 << "\t" << (*chem_level_iter).second.second;
		}

	}
	toFile3 << "\n";
	toFile3.close();

	std::ofstream toFile1("Sl.txt", std::ios::trunc);
	std::ofstream toFile2("Bl.txt", std::ios::trunc);
	toFile1 << chemical_level_num << "\n";
	toFile1 << "Testing" << "\n";
	toFile2 << chemical_level_num << "\n";
	toFile2 << "Testing" << "\n";

	for(int i = 0; i < chemical_level_num; i++)
	{
		toFile1 << i << "\t" << S_l[i] << "\n";
		toFile2 << i << "\t" << B_l[i] << "\n";
	}

	//write_cluster();
	//write_blocked();

	toFile1.close();
}

void Simulation::run_dimension()
{
	dimension_initialize();

	std::set<Bond>::iterator chem_level_iter;

	for(int i = 0; i<num_chem_levels; i++)
	{
		advance_level();
	}

}

void Simulation::record_dimension()
{
	for(int i=0; i<num_chem_levels; i++)
	{
		S_l_array[realization][i] = S_l[i];

		B_l_array[realization][i] = B_l[i];
	}

	realization++;
}

void Simulation::write_dimension()
{
	std::ofstream toFile1("Sl.txt", std::ios::trunc);
	toFile1 << num_realizations << "\t" << num_chem_levels << "\n";
	toFile1 << "Cluster masses as a function of chemical level\n";
	for(int i=0; i < num_realizations; i++)
	{
		for(int j=0; j < num_chem_levels; j++)
		{
			toFile1 << S_l_array[i][j] << "\t";
		}
		toFile1 << "\n";
	}

	toFile1.close();

	std::ofstream toFile2("Bl.txt", std::ios::trunc);
	toFile2 << num_realizations << "\t" << num_chem_levels << "\n";
	toFile2 << "Blocked masses as a function of chemical level\n";
	for(int i=0; i < num_realizations; i++)
	{
		for(int j=0; j < num_chem_levels; j++)
		{
			toFile2 << B_l_array[i][j] << "\t";
		}
		toFile2 << "\n";
	}

	toFile2.close();
}
