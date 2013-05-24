/*
 * Simulation.h
 *
 *  Created on: Nov 30, 2012
 *      Author: jqnorris
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "../framework/Framework.h"
#include <map>
#include <set>

class Simulation {
public:
	Simulation(PoreArray &, Matrix &, Options &);
	virtual ~Simulation();
	PoreArray & pores; //References to the arrays and options bin
	Matrix & matrix;
	Options & options;
	long int currentStep;
	Pore ** cluster; // An array that will be used to store pointers to the
	// invaded cluster.

	Rock ** fractures;// An array that will be used to store pointers to the
	// damaged rock
	void initialize();
	void timestep();
	std::multimap<double, Rock*> perimeter;// A map that stores the strength and
	// a pointer to the rocks that are available for damage.
	std::set<double> trapped;//A map that stores the bonds that are
	//trapped and the strength of the bond
};

#endif /* SIMULATION_H_ */
