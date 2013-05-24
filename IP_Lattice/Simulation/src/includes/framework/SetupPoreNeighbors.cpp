/*
 * SetupPoreNeighbors.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: jqnorris
 */

#include "SetupPoreNeighbors.h"
#include "Rock.h"

void setupPoreNeighbors(PoreArray & pores, Matrix & matrix, Options & options)
{
	if(options.latticeType==4)
	{
		for(int i=0; i<pores.dimension; i++)
		{
			for(int j=0; j<pores.dimension; j++)
			{
				pores.pores[i][j].neighbors = new  Rock*[options.latticeType];

				pores.pores[i][j].neighbors[0]= & matrix.matrix[2*i][j];
				pores.pores[i][j].neighbors[1]= & matrix.matrix[2*(i+1)][j];
				pores.pores[i][j].neighbors[2]= & matrix.matrix[2*i+1][j];
				pores.pores[i][j].neighbors[3]= & matrix.matrix[2*i+1][j+1];
			}
		}
	}

	if(options.latticeType==6)
	{
		for(int i=0; i<pores.dimension; i++)
		{
			for(int j=0; j<pores.dimension; j++)
			{
				pores.pores[i][j].neighbors = new  Rock*[options.latticeType];

				pores.pores[i][j].neighbors[0]= & matrix.matrix[3*i][j];
				pores.pores[i][j].neighbors[1]= & matrix.matrix[3*(i+1)][j+1];
				pores.pores[i][j].neighbors[2]= & matrix.matrix[3*i+1][j];
				pores.pores[i][j].neighbors[3]= & matrix.matrix[3*(i+1)+1][j];
				pores.pores[i][j].neighbors[4]= & matrix.matrix[3*i+2][j];
				pores.pores[i][j].neighbors[5]= & matrix.matrix[3*i+2][j+1];
			}
		}
	}
}


