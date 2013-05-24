/*
 * PoreArray.h
 *
 * An array for storing all of the pores
 *
 *  Created on: Nov 29, 2012
 *      Author: jqnorris
 */

#ifndef POREARRAY_H_
#define POREARRAY_H_
#include "Pore.h"
#include "Options.h"
#include "Rock.h"

class PoreArray {
public:
	PoreArray(Options &);
	virtual ~PoreArray();
	Pore** pores;
	Pore reservoir; // A pore that represents the boundary of the array
	float leakoff;
	int dimension;
	void forAllPores(void (&f)(Pore&, Options&), Options &);
	void initializePores();
};

#endif /* POREARRAY_H_ */
