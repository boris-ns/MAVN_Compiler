#include "InterferenceGraph.h"

using namespace std;

InterferenceGraph::InterferenceGraph(Variables& vars)
	: regVariables(vars)
{
}

InterferenceGraph::~InterferenceGraph()
{
}

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
					im[defPos][outPos] = __NO_INTERFERENCE__;
					im[outPos][defPos] = __NO_INTERFERENCE__;
				}
			}
		}
	}
}

void InterferenceGraph::ResizeInterferenceMatrix(size_t size)
{
	im.resize(size);

	for (int i = 0; i < im.size(); ++i)
		im[i].resize(size);
}

void InterferenceGraph::BuildVariableStack()
{
	for (Variables::iterator it = regVariables.begin();
		it != regVariables.end();
		it++)
	{
		varStack.push(*it);
	}
}

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

void InterferenceGraph::ResourceAllocation()
{
	while (!varStack.empty())
	{
		vector<Regs> freeRegs = { t0, t1, t2, t3 };
		Variable* var = varStack.top();
		varStack.pop();

		int varPos = var->GetPos();
		Regs currentReg = freeRegs.back();
		var->SetAssignment(currentReg);
		freeRegs.pop_back();

		for (int i = 0; i < im[varPos].size(); ++i)
		{
			if (im[varPos][i] == __INTERFERENCE__)
			{
				// moramo dodeliti novi registar
				if (freeRegs.empty())
				{
					cout << endl << "Greska prilikom alokacije resursa!" << endl;
					return;
				}

				ApplyRegToVariable(i, freeRegs.back());
				freeRegs.pop_back();
			}
			else
			{
				// mozemo dodeliti isti registar kao sto ima i var
				ApplyRegToVariable(i, currentReg);
			}
		}
	}

	cout << endl << "Alokacija resursa je zavrsena." << endl;
}
