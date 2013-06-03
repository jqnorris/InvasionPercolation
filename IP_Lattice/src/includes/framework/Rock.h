/*
 * Rock.h
 *
 *	Definition of the rock/bond elements used in the simulation.
 *
 *  Created on: Nov 29, 2012
 *      Author: jqnorris
 */

#ifndef ROCK_H_
#define ROCK_H_

class Pore;

class Rock {
public:
	Rock();
	virtual ~Rock();
	int damage;
	double strength;
	Pore * pore1;
	Pore * pore2;
};

#endif /* ROCK_H_ */
