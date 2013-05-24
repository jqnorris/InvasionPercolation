/*
 * Pore.h
 *
 * Definition of the pore elements for the simulation
 *
 *  Created on: Nov 29, 2012
 *      Author: jqnorris
 */

#ifndef PORE_H_
#define PORE_H_

class Rock;

class Pore {
public:
	Pore();
	virtual ~Pore();
	int volume;
	int positionX;
	int positionY;
	Rock ** neighbors;
};

#endif /* PORE_H_ */
