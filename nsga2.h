#pragma once
#include "WholePopulation.h"

class nsga2
{
public:
	nsga2();
	~nsga2();
	void solve(clock_t start, int runtimes);
	void fileprint(FILE *fp, int gen);

	WholePopulation* wpop;
};
