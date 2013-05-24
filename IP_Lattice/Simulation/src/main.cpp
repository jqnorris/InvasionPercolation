/*
 * simulation.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: jqnorris
 */
#include <iostream>
#include "includes/includes.h"

using std::cout;
using std::endl;

int main(void)
{

	// Create all the elements of the simulation
	Options options;

	PoreArray pores(options);

	Matrix matrix(options);

	Simulation simulation(pores, matrix, options);

	Data data(pores, matrix, options, simulation);

	std::cout << "Beginning Run" << std::endl;

	// Perform time steps until the invasion reaches the edge of the array
	// or the maximum steps is reached.
	for(int i=0; i<options.maxSteps-1; i++)
	{
		simulation.timestep();
		if(pores.reservoir.volume !=0)
		{
			i=options.maxSteps;
		}
	}

	// Output to file
	data.writeInvasion();

	std::cout << std::endl << simulation.currentStep << " Steps Successfully Completed!" << std::endl;

	return 0;
}
