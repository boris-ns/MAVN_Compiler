#include "IR.h"

bool ContainsVariable(Variable& variable, Variables& variableList)
{
	for (Variables::const_iterator it = variableList.begin();
		it != variableList.end();
		it++)
	{
		if (variable.getName() == (*it)->getName())
			return true;
	}

	return false;
}

void LivenessAnalysis(Instructions& instructions)
{
	bool again = true;
	while (again)
	{
		again = false;

		for (Instructions::reverse_iterator it = instructions.rbegin();
			it != instructions.rend();
			it++)
		{
			Variables newOut, newIn;
			Variables& out = (*it)->m_out;
			Variables& in = (*it)->m_in;
		
			// Kreiranje newOut
			Instructions& successors = (*it)->m_succ;
			for (Instructions::iterator succIt = successors.begin();
				succIt != successors.end();
				succIt++)
			{
				Variables& succIn = (*succIt)->m_in;
				newOut.insert(newOut.end(), succIn.begin(), succIn.end());
			}

			newOut.sort();
			newOut.unique();
		
			// Kreiranje newIn
			Variables& use = (*it)->m_use;
			Variables& def = (*it)->m_def;
			Variables outMinusDef;

			for (Variables::iterator varIt = out.begin();
				varIt != out.end();
				varIt++)
			{
				if (!ContainsVariable(*(*varIt), def))
					outMinusDef.push_back(*varIt);
			}

			newIn = use;
			newIn.insert(newIn.end(), outMinusDef.begin(), outMinusDef.end());
			newIn.sort();
			newIn.unique();

			if (newIn != in || newOut != out)
				again = true;

			in = newIn;
			out = newOut;
		}
	}
}

void PrintInstructions(Instructions& instructions)
{
	for (Instructions::iterator it = instructions.begin();
		it != instructions.begin();
		it++)
	{
		(*it)->PrintInstruction();
	}
}

InterferenceGraph& BuildInterferenceGraph(Instructions& instructions)
{
	ig.variables = &getVariables();

	InterferenceMatrix interference_matrix;
	interference_matrix.resize(ig.variables->size());
	
	for (int i = 0; i < interference_matrix.size(); ++i)
		interference_matrix.at(i).resize(interference_matrix.size());
	
	ig.matrix = interference_matrix;

	Instructions::iterator iter = instructions.begin();
	for (iter; iter != instructions.end(); ++iter)
	{
		if ((*iter)->type != InstructionType::move)
		{
			Variables::iterator var_iter = (*iter)->def.begin();
			for (var_iter; var_iter != (*iter)->def.end(); ++var_iter)
			{
				Variables::iterator var_iter_ = (*iter)->out.begin();
				for (var_iter_; var_iter_ != (*iter)->out.end(); ++var_iter_)
				{
					if ((*var_iter)->pos != (*var_iter_)->pos)
					{
						ig.matrix.at((*var_iter)->pos).at((*var_iter_)->pos) = __INTERFERENCE__;
						ig.matrix.at((*var_iter_)->pos).at((*var_iter)->pos) = __INTERFERENCE__;
					}
				}
			}
		}
	}

	return ig;
}

/////////// Variable

Variable::Variable(std::string name, int pos)
	: m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign)
{
}

std::string Variable::getName() 
{ 
	return m_name;
}


/////////// Instruction

Instruction::Instruction()
	: m_position(0), m_type(I_NO_TYPE)
{
}

Instruction::Instruction(int pos, InstructionType type, Variables& dst, Variables& src, const std::string& labelName)
	: m_position(pos), m_type(type), m_dst(dst), m_src(src) 
{
	FillUseDefVariables();
}

void Instruction::FillUseDefVariables()
{
	switch (m_type)
	{
	case I_ADD: // add rid,rid,rid
	case I_SUB: // sub rid,rid,rid
	case I_ADDI: // addi rid,rid,num
		m_def.insert(m_def.end(), m_dst.begin(), m_dst.end());
		m_use.insert(m_use.end(), m_src.begin(), m_src.end());
		break;

	case I_LA: // la rid,mid
	case I_LW: // lw rid,num(rid)
	case I_LI: // li rid,num
	case I_SW:
		m_def.insert(m_def.end(), m_dst.begin(), m_dst.end());
		break;

	case I_BLTZ: // bltz rid,id
		m_use.insert(m_use.end(), m_src.begin(), m_src.end());
		break;

	case I_B:   // b id
	case I_NOP: // nop
		break;
	}
}

void Instruction::PrintInstruction()
{

}

InstructionType Instruction::getType()
{
	return m_type;
}

std::string Instruction::getLabelName()
{ 
	return labelName; 
}