#pragma once

#include <vector>
#include "IR.h"

#define __NO_INTERFERENCE__ 0
#define __INTERFERENCE__    1

typedef std::vector<std::vector<int>> InterferenceMatrix;

class InterferenceGraph
{
public:
	InterferenceGraph(Variables& vars);
	~InterferenceGraph();

	void BuildInterferenceGraph(Instructions& instructions);

	InterferenceMatrix im;
	Variables variables;

private:

	void ResizeInterferenceMatrix(size_t size);

	Variables& regVariables;
};
