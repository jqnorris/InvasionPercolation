/*
 * PoreArray.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: jqnorris
 */

#include "PoreArray.h"
#include "Pore.h"
#include "Options.h"
#include "Rock.h"

PoreArray::PoreArray(Options & options)
{
	dimension=options.N;

	//Create an array of pores on the heap
	pores = new Pore*[dimension];
	for(int i=0; i<dimension; i++)
	{
		pores[i]= new Pore[dimension];
	}

	initializePores();

	reservoir.volume=0;
}

PoreArray::~PoreArray()
{
	//Clean up the array of pores
	for(int i=0; i<dimension; i++)
	{
		delete[] pores[i];
	}
	delete[] pores;
}

//A helper function that takes a function, and applies it to every pore in the array.
void PoreArray::forAllPores( void (&f)(Pore &, Options&), Options & options)
{
	for(int i=0; i<dimension; i++)
	{
		for(int j=0; j<dimension; j++)
		{
			f(pores[i][j], options);
		}
	}
}

//Sets the important values of the pores
void PoreArray::initializePores()
{
	for(int i=0; i<dimension; i++)
	{
		for(int j=0; j<dimension; j++)
		{
			pores[i][j].volume=0;
			pores[i][j].positionX=i;
			pores[i][j].positionY=j;
		}
	}
}

