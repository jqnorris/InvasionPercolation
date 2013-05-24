/*
 * Data.cpp
 *
 *  Created on: Dec 7, 2012
 *      Author: jqnorris
 */

#include <iostream>
#include <fstream>
#include "Data.h"

using std::ofstream;

Data::Data(PoreArray & P, Matrix & M, Options & O, Simulation & S):pores(P),matrix(M),options(O),simulation(S)
{
	initialize();
}

Data::~Data()
{

}

//A function that writes:
//	1. The invaded sites, in the order they were invaded
//	2. The damaged bonds, in the order they were damaged
// 	3. All the rocks and their strength.
//  4. The strength of bonds that are trapped
void Data::writeInvasion()
{
	ofstream toFile1("invasion.txt", std::ios::app);
	ofstream toFile2("fractures.txt", std::ios::app);
	ofstream toFile3("clusters.txt", std::ios::app);
	ofstream toFile4("trapped.txt", std::ios::app);

	toFile2.precision(17);
	toFile4.precision(17);

	for(int i=0; i< simulation.currentStep; i++)
	{
		toFile1 << simulation.cluster[i]->positionX << "\t" << simulation.cluster[i]->positionY << "\t" << "\n";
	}

	for(int i=0; i< simulation.currentStep-1; i++)
	{
		toFile2 << simulation.fractures[i]->strength << "\t";
		toFile2 << simulation.fractures[i]->pore1->positionX << "\t";
		toFile2 << simulation.fractures[i]->pore1->positionY << "\t";
		toFile2 << simulation.fractures[i]->pore2->positionX << "\t";
		toFile2 << simulation.fractures[i]->pore2->positionY << "\t";
		toFile2	<< "\n";
	}

	toFile1.close();
	toFile2.close();

	for(int i=0; i< matrix.xDimension; i++)
	{
		for(int j=0; j< matrix.yDimension; j++)
		{
			toFile3 << matrix.matrix[i][j].pore1->positionX << "\t";
			toFile3 << matrix.matrix[i][j].pore1->positionY << "\t";
			toFile3 << matrix.matrix[i][j].pore2->positionX << "\t";
			toFile3 << matrix.matrix[i][j].pore2->positionY << "\t";
			toFile3 << matrix.matrix[i][j].strength << "\n";
		}
	}

	toFile3.close();

	while(0 < simulation.trapped.size())
	{
		toFile4 << *(simulation.trapped.begin()) << "\n";
		simulation.trapped.erase(simulation.trapped.begin());
	}

	toFile4.close();
}

// A function to set up the files for writing.
void Data::initialize()
{
	ofstream toFile1("invasion.txt", std::ios::trunc);
	toFile1 << "Invasion for: " << options.jobName << "\n";
	toFile1.close();

	ofstream toFile2("fractures.txt", std::ios::trunc);
	toFile2 << "Fractures for: " << options.jobName << "\n";
	toFile2.close();

	ofstream toFile3("clusters.txt", std::ios::trunc);
	toFile3 << "Clusters for: " << options.jobName << "\n";

	ofstream toFile4("trapped.txt", std::ios::trunc);
	toFile4 << "Trapped bonds for: " << options.jobName << "\n";
}
