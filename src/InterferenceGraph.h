#pragma once

#include <vector>
#include <stack>
#include "IR.h"

#define __NO_INTERFERENCE__ 0
#define __INTERFERENCE__    1

typedef std::vector<std::vector<int>> InterferenceMatrix;
typedef std::stack<Variable*> VariableStack;

class InterferenceGraph
{
public:
	InterferenceGraph(Variables& vars);
	~InterferenceGraph();

	void BuildInterferenceGraph(Instructions& instructions);
	void BuildVariableStack();
	void ResourceAllocation();

	InterferenceMatrix im;
	VariableStack varStack;

private:

	void ResizeInterferenceMatrix(size_t size);
	void ApplyRegToVariable(int varPos, Regs reg);

	Variables& regVariables;
};
