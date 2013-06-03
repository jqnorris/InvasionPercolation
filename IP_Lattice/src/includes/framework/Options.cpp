/*
 * Options.cpp
 *
 *
 * *  Created on: Nov 29, 2012
 *      Author: jqnorris
 */

#include "Options.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>

using std::cout;
using std::endl;
using std::getline;
using std::string;

Options::Options()
{
	//Get the options from file options.in
	loadFromFile();

}

Options::~Options()
{

}

//Read in the options from file and check to make sure options are correct.
//Otherwise use default values.
void Options::loadFromFile()
{
	int counter=0;

	string test, temp;

	std::ifstream optionsFile("options.in");

	if(optionsFile.good())
	{
		cout << endl << "Loading Options File." << endl << endl;

		getline(optionsFile, jobName);
		cout << "\tOptions for: " << jobName << endl;

		counter++;

		getline(optionsFile, test, '=');
		if (test == "MaxSteps")
		{
			getline(optionsFile, temp);
			maxSteps =atoi(temp.c_str());
			cout << "\t Max Steps = " << maxSteps << endl;

			counter++;
		}

		getline(optionsFile, test, '=');
		if (test == "N")
		{
			getline(optionsFile, temp);
			N =atoi(temp.c_str());
			cout << "\t N = " << N<< endl;

			counter++;
		}

		getline(optionsFile, test, '=');
		if (test == "LatticeType")
		{
			getline(optionsFile, temp);
			latticeType =atoi(temp.c_str());
			cout << "\t LatticeType = " << latticeType<< endl;

			counter++;
		}

		getline(optionsFile, test, '=');
		if (test == "Trapping")
		{
			getline(optionsFile, temp);
			if(0 == atoi(temp.c_str()))
			{
				trapping = false;
			}
			else
			{
				trapping = true;
			}
			cout << "\t Trapping = " << trapping << endl;

			counter++;
		}
	}
	else
	{

		cout << endl << "No Options File Found!!!" << endl;
		cout << endl << "\tUsing default values." << endl;

		setDefault();
		counter=-1;

	};

	if (counter==5 || counter==-1)
	{
		error=false;
		cout << endl << "Options successfully set." << endl << endl;

	}
	else
	{
		cout << endl << "\t***Error Loading Options!!!***" << endl;
		cout << endl << "\tUsing default values." << endl;
		setDefault();
		error=true;
	}
}

//A function to set default option values.
void Options::setDefault()
{
	jobName="DEFAULT";
	maxSteps=1000;
	N=100;
	latticeType=4;
	trapping=false;
}
