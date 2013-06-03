/*
 * Simulation.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: jqnorris
 */

#include "Simulation.h"
#include "iostream"

using std::pair;

Simulation::Simulation(PoreArray & P, Matrix & M, Options & O):pores(P),matrix(M),options(O)
{
	initialize();

	// Dynamically create the arrays on the heap
	cluster = new Pore*[options.maxSteps+1]();
	fractures = new Rock*[options.maxSteps]();

	//Set the initial site where the invasion begins
	cluster[0] = & pores.pores[options.N/2][options.N/2];
	cluster[0]->volume=1;
	currentStep=1;

}

Simulation::~Simulation()
{
	//Clean up the arrays on the heap
	delete[] cluster;
	delete[] fractures;
}

void Simulation::initialize()
{
	//Setup all the pointers that make the simulation work
	setupPoreNeighbors(pores, matrix, options);
	setupMatrixNeighbors(pores, matrix, options);
}

void Simulation::timestep()
{
	//For the pore which was invaded on the previous step, add
	//all available neighbors to the map
	for(int i=0; i< options.latticeType; i++)
	{
		if(cluster[currentStep-1]->neighbors[i]->pore1->volume != cluster[currentStep-1]->neighbors[i]->pore2->volume)
		{
			perimeter.insert(pair<double, Rock*>(cluster[currentStep-1]->neighbors[i]->strength, cluster[currentStep-1]->neighbors[i]));
		}
	}

	//Check the first element in the map. If it's neighbors are already invaded,
	//remove it from the map.
	while(perimeter.begin()->second->pore1->volume == perimeter.begin()->second->pore2->volume)
	{
		trapped.insert(perimeter.begin()->second->strength);
		perimeter.erase(perimeter.begin());
	}

	//Damage the first element in the map(i.e. the one with the smallest strength)
	perimeter.begin()->second->damage=1;

	//Add the damaged element to the array of damaged elements
	fractures[currentStep-1] = perimeter.begin()->second;

	//Invade the appropriate pore and add it to the list of invaded pores.
	if(fractures[currentStep-1]->pore1->volume != 1)
	{
		fractures[currentStep-1]->pore1->volume = 1;
		cluster[currentStep] = fractures[currentStep-1]->pore1;
	}
	if(fractures[currentStep-1]->pore2->volume != 1)
	{
		fractures[currentStep-1]->pore2->volume = 1;
		cluster[currentStep] = fractures[currentStep-1]->pore2;
	}

	//Remove the damaged element from the map
	perimeter.erase(perimeter.begin());

	currentStep++;
}
