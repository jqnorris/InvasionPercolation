/*
 * SetupNeighbors.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: jqnorris
 */

#include "SetupMatrixNeighbors.h"

int setupMatrixNeighbors(PoreArray & poreArray, Matrix & matrix, Options & options)
{
	if(options.latticeType == 4)//Square Lattice
	{
		int lastRow = matrix.xDimension-1;
		int lastColumn = matrix.yDimension-1;


		for(int i=0; i<matrix.xDimension; i+=2)
		{
			for(int j=0; j<matrix.yDimension; j++)
			{
				if(j==lastColumn)
				{
					matrix.matrix[i][j].pore1 =& poreArray.reservoir;
					matrix.matrix[i][j].pore2 =& poreArray.reservoir;
				}
				else
				{
					if(i%lastRow==0)
					{
						if (i==0)
						{
							matrix.matrix[i][j].pore1 =& poreArray.reservoir;
							matrix.matrix[i][j].pore2 =& poreArray.pores[i/2][j];
						}
						if (i==(lastRow))
						{
							matrix.matrix[i][j].pore1 =& poreArray.pores[i/2-1][j];
							matrix.matrix[i][j].pore2 =& poreArray.reservoir;
						}
					}
					else
					{
						matrix.matrix[i][j].pore1 =& poreArray.pores[i/2-1][j];
						matrix.matrix[i][j].pore2 =& poreArray.pores[i/2][j];
					};
				}
			}
		}


		for(int i=1; i<matrix.xDimension; i+=2)
		{
			for(int j=0; j<matrix.yDimension; j++)
			{
				if(j%lastColumn==0)
				{
					if(j==0)
					{
						matrix.matrix[i][j].pore1 =& poreArray.reservoir;
						matrix.matrix[i][j].pore2 =& poreArray.pores[(i-1)/2][j];
					}
					if(j==lastColumn)
					{
						matrix.matrix[i][j].pore1 =& poreArray.pores[(i-1)/2][j-1];
						matrix.matrix[i][j].pore2 =& poreArray.reservoir;
					}
				}
				else
				{
					matrix.matrix[i][j].pore1 =& poreArray.pores[(i-1)/2][j-1];
					matrix.matrix[i][j].pore2 =& poreArray.pores[(i-1)/2][j];
				};
			}
		}
	}

	if(options.latticeType == 6)//Hexagonal Lattice
	{
		int lastRow = matrix.xDimension-1;
		int lastColumn = matrix.yDimension-1;


		for(int i=0; i<matrix.xDimension; i+=3)
		{
			for(int j=0; j<matrix.yDimension; j++)
			{
				if(i==0 || i==lastRow-1)
				{
					if(i==0)
					{
						if(j==lastColumn)
						{
							matrix.matrix[i][j].pore1 =& poreArray.reservoir;
							matrix.matrix[i][j].pore2 =& poreArray.reservoir;
						}
						else
						{
							matrix.matrix[i][j].pore1 =& poreArray.reservoir;
							matrix.matrix[i][j].pore2 =& poreArray.pores[i/3][j];
						};
					}
					if(i==(lastRow-1))
					{
						if(j==0)
						{
							matrix.matrix[i][j].pore1 =& poreArray.reservoir;
							matrix.matrix[i][j].pore2 =& poreArray.reservoir;
						}
						else
						{
							matrix.matrix[i][j].pore1 =& poreArray.pores[i/3-1][j-1];
							matrix.matrix[i][j].pore2 =& poreArray.reservoir;
						}
					}
				}
				else
				{
					if(j%lastColumn==0)
					{
						if(j==0)
						{
							matrix.matrix[i][j].pore1 =& poreArray.reservoir;
							matrix.matrix[i][j].pore2 =& poreArray.pores[i/3][j];
						}
						if(j==lastColumn)
						{
							matrix.matrix[i][j].pore1 =& poreArray.pores[i/3-1][j-1];
							matrix.matrix[i][j].pore2 =& poreArray.reservoir;
						}
					}
					else
					{
						matrix.matrix[i][j].pore1 =& poreArray.pores[i/3-1][j-1];
						matrix.matrix[i][j].pore2 =& poreArray.pores[i/3][j];
					};
				}
			}
		}

		for(int i=1; i<matrix.xDimension; i+=3)
		{
			for(int j=0; j<matrix.yDimension; j++)
			{
				if(j==lastColumn)
				{
					matrix.matrix[i][j].pore1 =& poreArray.reservoir;
					matrix.matrix[i][j].pore2 =& poreArray.reservoir;
				}
				else
				{
					if((i-1)==0 || i==lastRow)
					{
						if ((i-1)==0)
						{
							matrix.matrix[i][j].pore1 =& poreArray.reservoir;
							matrix.matrix[i][j].pore2 =& poreArray.pores[(i-1)/3][j];
						}
						if (i==lastRow)
						{
							matrix.matrix[i][j].pore1 =& poreArray.pores[(i-1)/3-1][j];
							matrix.matrix[i][j].pore2 =& poreArray.reservoir;
						}
					}
					else
					{
						matrix.matrix[i][j].pore1 =& poreArray.pores[(i-1)/3-1][j];
						matrix.matrix[i][j].pore2 =& poreArray.pores[(i-1)/3][j];
					};
				}
			}
		}


		for(int i=2; i<matrix.xDimension; i+=3)
		{
			for(int j=0; j<matrix.yDimension; j++)
			{
				if(j%lastColumn==0)
				{
					if(j==0)
					{
						matrix.matrix[i][j].pore1 =& poreArray.reservoir;
						matrix.matrix[i][j].pore2 =& poreArray.pores[(i-2)/3][j];
					}
					if(j==lastColumn)
					{
						matrix.matrix[i][j].pore1 =& poreArray.pores[(i-2)/3][j-1];
						matrix.matrix[i][j].pore2 =& poreArray.reservoir;
					}
				}
				else
				{
					matrix.matrix[i][j].pore1 =& poreArray.pores[(i-2)/3][j-1];
					matrix.matrix[i][j].pore2 =& poreArray.pores[(i-2)/3][j];
				};
			}
		}


	}


	return 0;
}


