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
	list<Regs> freeRegs;
	list<Variable*> colored;

	for (int i = 1; i <= __REG_NUMBER__; i++) 
		freeRegs.push_back((Regs)i);

	while (!varStack.empty()) 
	{
		Variable* temp = varStack.top();
		bool interference = false;

		for (auto it = colored.begin(); it != colored.end(); it++)
		{
			if (im[temp->GetPos()][(*it)->GetPos()] == __INTERFERENCE__) 
				interference = true;
		}

		if (!interference)
		{
			if (freeRegs.empty())
				return false;

			temp->SetAssignment(freeRegs.front());
		}
		else
		{
			if (!freeRegs.empty())
			{
				freeRegs.pop_front();
				temp->SetAssignment(freeRegs.front());
			}
			else
			{
				return false;
			}
		}

		colored.push_back(temp);
		varStack.pop();
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