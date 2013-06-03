/*
 * Pore.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: jqnorris
 */

#include "Pore.h"
#include <stdio.h>

Pore::Pore()
{
	neighbors=NULL;
	positionX=-1;
	positionY=-1;

}

Pore::~Pore()
{
	if(neighbors!=NULL)
	{
		delete[] neighbors;
	}
}
