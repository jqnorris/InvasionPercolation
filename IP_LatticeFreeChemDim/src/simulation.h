/*
 * simulation.h
 *
 *  Created on: Sep 5, 2013
 *      Author: jqnorris
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <map>
#include <set>
#include <deque>
#include <vector>

typedef std::pair<int, int> Site;
typedef std::pair<Site, Site> Bond;

class Bond_Data {
	friend class Simulation;
public:
	Bond_Data();
	~Bond_Data();
private:
	double strength;
	long int order_added;
	bool invaded;
	bool blocked;
	bool available;
	int chemical_level;
};

class Simulation {
public:
	Simulation();
	virtual ~Simulation();

	bool step_time();
	void step_time(int);
	bool cluster_terminated();
	void test_dimension();
	void run_dimension();
	void record_dimension();
	void write_cluster();
	void write_blocked();
	void write_dimension();
	int num_chem_levels;
	int num_realizations;
private:
	// Invasion Percolation Data
	std::map<Bond, Bond_Data> lattice;
	std::map<double, Bond> accessible_bonds;
	std::set<Site> invaded_sites;
	long int time_step;
	long int blocked_index;
	double pc;

	// Invasion Percolation Functions
	Site get_neighbor(Site pos, unsigned int direction);
	bool bond_is_blocked(Bond & inv_bond);
	void add_new_bonds(Site & inv_site);
	bool invade_bond(Bond & inv_bond);

	// Fractal Dimension Data
	std::set<Bond> waiting_on;
	int chemical_level_num;
	std::set<Bond> this_chemical_level_cluster;
	std::set<Bond> this_chemical_level_blocked;
	std::set<Bond> next_chemical_level_cluster;
	std::set<Bond> next_chemical_level_blocked;
	int S_l[1000];
	int B_l[1000];

	// Fractal Dimension Functions
	void dimension_initialize();
	short int get_bond_direction(Bond bond);
	bool check_branch(Bond);
	bool check_cluster();
	void prep_next_level();
	void advance_level();
	void record_level_size();


	// Averaging Data
	int realization;
	int S_l_array[100][1000];
	int B_l_array[100][1000];
};

#endif /* SIMULATION_H_ */
