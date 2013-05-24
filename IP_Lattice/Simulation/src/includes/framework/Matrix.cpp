/*
 * Matrix.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: jqnorris
 */

#include "Matrix.h"
#include "Rock.h"
#include "Options.h"
#include <cstdlib>
#include <time.h>

Matrix::Matrix(Options & options)
{
	if(options.latticeType==4)//Square lattice
	{

		xDimension = 2*options.N+1;
		yDimension = options.N+1;
	}

	if(options.latticeType==6)//Hexagonal lattice
	{
		xDimension = 3*options.N+2;
		yDimension = options.N+1;
	}

	// Dynamically create an array of rock on the heap
	matrix = new Rock*[xDimension];
	for(int i=0; i<xDimension; i++)
	{
		matrix[i] = new Rock[yDimension];
	}

	// Use the time to generate a seed f
	srand48(time(NULL));

	//Give the random number generator a seed
//	unsigned short int seedvec[3];
//	seedvec[0]=1234;
//	seedvec[1]=1010;
//	seedvec[2]=9999;
//	unsigned short int test=*seed48(seedvec);

	//
	forAllRock(initializeRock,options);
}

Matrix::~Matrix()
{
	//Clean up the array of rock on the heap
	for(int i=0; i<xDimension; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}


//A function that takes in a function and applies it to every element of the array
void Matrix::forAllRock(void (&f)(Rock &, Options&), Options & options)
{
	for(int i=0; i<xDimension; i++)
	{
		for(int j=0; j<yDimension; j++)
		{
			f(matrix[i][j], options);
		}
	}
}

//A function that sets the important values of the rock elements
void Matrix::initializeRock(Rock & rock, Options & options)
{
	rock.damage=0;
	rock.strength=drand48();
}









