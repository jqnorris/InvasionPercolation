/*
 * SetupNeighbors.h
 *
 *	A function that sets the pointers of each rock element
 *	to point to neighboring pores.
 *
 *  Created on: Nov 29, 2012
 *      Author: jqnorris
 */

#ifndef SETUPNEIGHBORS_H_
#define SETUPNEIGHBORS_H_

#include "Rock.h"
#include "Pore.h"
#include "PoreArray.h"
#include "Matrix.h"
#include "Options.h"


int setupMatrixNeighbors(PoreArray &, Matrix &, Options &);



#endif /* SETUPNEIGHBORS_H_ */
