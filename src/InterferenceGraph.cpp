#include "InterferenceGraph.h"
#include <iomanip>
#include "Constants.h"

using namespace std;

InterferenceGraph::InterferenceGraph(Variables& vars)
	: regVariables(vars)
{
}

InterferenceGraph::~InterferenceGraph()
{
}

/* Creates interference graph (matrix) from instructions. */
void InterferenceGraph::BuildInterferenceGraph(Instructions& instructions)
{
	ResizeInterferenceMatrix(regVariables.size());

	for (Instructions::iterator it = instructions.begin();
		it != instructions.end();
		it++)
	{
		for (Variables::iterator defVar = (*it)->m_def.begin();
			defVar != (*it)->m_def.end();
			defVar++)
		{
			for (Variables::iterator outVar = (*it)->m_out.begin();
				outVar != (*it)->m_out.end();
				outVar++)
			{
				int defPos = (*defVar)->GetPos();
				int outPos = (*outVar)->GetPos();

				if (defPos != outPos)
				{
					im[defPos][outPos] = __INTERFERENCE__;
					im[outPos][defPos] = __INTERFERENCE__;
				}
				else
				{
					im[defPos][outPos] = __EMPTY__;
					im[outPos][defPos] = __EMPTY__;
				}
			}
		}
	}
}

/* Resizes matrix to the passed size. */
void InterferenceGraph::ResizeInterferenceMatrix(size_t size)
{
	im.resize(size);

	for (int i = 0; i < im.size(); ++i)
		im[i].resize(size);
}

/* Creates stack from list of register variables. */
void InterferenceGraph::BuildVariableStack()
{
	for (Variables::iterator it = regVariables.begin();
		it != regVariables.end();
		it++)
	{
		varStack.push(*it);
	}
}

/* Applies reg to the variable that has varPos for it's position. */
void InterferenceGraph::ApplyRegToVariable(int varPos, Regs reg)
{
	for (Variables::iterator it = regVariables.begin();
		it != regVariables.end();
		it++)
	{
		if ((*it)->GetPos() == varPos)
			(*it)->SetAssignment(reg);
	}
}

/* Allocates real registers to variables according to the interference. */
bool InterferenceGraph::ResourceAllocation()
{
	while (!varStack.empty())
	{
		list<Regs> freeRegs = { t0, t1, t2, t3, t4 };
		Variable* var = varStack.top();
		varStack.pop();

		int varPos = var->GetPos();
		Regs currentReg = freeRegs.front();
		var->SetAssignment(currentReg);
		freeRegs.pop_front();

		for (int i = 0; i < im[varPos].size(); ++i)
		{
			if (im[varPos][i] == __INTERFERENCE__)
			{
				// moramo dodeliti novi registar
				if (freeRegs.empty())
					return false;

				ApplyRegToVariable(i, freeRegs.front());
				freeRegs.pop_front();
			}
			else
			{
				// mozemo dodeliti isti registar kao sto ima i var
				ApplyRegToVariable(i, currentReg);
			}
		}
	}

	return true;
}

/* Prints interference matrix to the console. */
void InterferenceGraph::PrintInterferenceMatrix()
{
	// Print first row (names of registers) as header
	printf("   ");
	for (size_t i = 0; i < im.size(); ++i)
		printf("r%-3d", i);

	printf("\n");

	// Print first column (names of registers) and actual data
	for (size_t i = 0; i < im.size(); ++i)
	{
		printf("r%d ", i);

		for (size_t j = 0; j < im[i].size(); ++j)
			printf("%-4d", im[i][j]);

		printf("\n");
	}
}