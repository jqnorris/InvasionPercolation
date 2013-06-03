/*
 * Matrix.h
 *
 *An array that contains all the rock elements.
 *
 *  Created on: Nov 29, 2012
 *      Author: jqnorris
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "Rock.h"
#include "Options.h"

class Matrix {
public:
	Matrix(Options &);
	virtual ~Matrix();
	Rock** matrix;
	int xDimension;
	int yDimension;
	void forAllRock(void (&f)(Rock &, Options&), Options &);
	static void initializeRock(Rock &, Options&);
};

#endif /* MATRIX_H_ */
