/*
 * main.cpp
 *
 *  Created on: May 20, 2013
 *      Author: jqnorris
 */

#include "simulation.h"
#include <iostream>

int main(int argc, char **argv)
{
	Simulation ip_sim;

	for(int i=0; i<ip_sim.num_realizations; i++)
	{
		ip_sim.run_dimension();
		ip_sim.record_dimension();
	}

	ip_sim.write_dimension();

	return 0;
}



