/*
 * SetupPoreNeighbors.h
 *
 *  A function that sets the pointers of each pore element
 *	to point to neighboring rock.
 *
 *  Created on: Nov 30, 2012
 *      Author: jqnorris
 */

#ifndef SETUPPORENEIGHBORS_H_
#define SETUPPORENEIGHBORS_H_

#include "PoreArray.h"
#include "Matrix.h"
#include "Options.h"

void setupPoreNeighbors(PoreArray &, Matrix &, Options &);


#endif /* SETUPPORENEIGHBORS_H_ */
