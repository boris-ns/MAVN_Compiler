#pragma once

#include <vector>
#include "IR.h"

typedef std::vector<std::vector<int>> InterferenceMatrix;


class InterferenceGraph
{
public:
	InterferenceGraph();
	~InterferenceGraph();

	InterferenceMatrix im;
	Variables variables;
};


InterferenceGraph& BuildInterferenceGraph(Instructions& instructions);