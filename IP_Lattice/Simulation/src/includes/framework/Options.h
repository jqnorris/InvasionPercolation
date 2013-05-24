/*
 * Options.h
 *
 * A bin for storing all the options values for the simulation
 *
 *  Created on: Nov 29, 2012
 *      Author: jqnorris
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>

class Options {
public:
	Options();
	virtual ~Options();
	std::string jobName;
	long int maxSteps;
	int N;
	int latticeType;
	bool trapping;
	bool error;
	void loadFromFile();
	void setDefault();
};

#endif /* OPTIONS_H_ */
