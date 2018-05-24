#include "IR.h"
#include <iostream>

using namespace std;

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
		it != instructions.end();
		it++)
	{
		(*it)->PrintInstruction();
		cout << "--------------------------------------" << endl;
	}
}

/////////// Variable

Variable::Variable(std::string name, int pos, VariableType type)
	: m_type(type), m_name(name), m_position(pos), m_assignment(no_assign)
{
}

std::string Variable::getName() 
{ 
	return m_name;
}

int Variable::GetPos()
{
	return m_position;
}

Variable::VariableType Variable::GetType()
{
	return m_type;
}

void Variable::SetAssignment(Regs r)
{
	m_assignment = r;
}

std::ostream& operator<<(std::ostream& out, const Variable& v)
{
	out << v.m_name;

	return out;
}

/////////// Instruction

Instruction::Instruction()
	: m_position(0), m_type(I_NO_TYPE)
{
}

Instruction::Instruction(int pos, InstructionType type, Variables& dst, Variables& src, const std::string& labelName, const std::string& currentLabel)
	: m_position(pos), m_type(type), m_dst(dst), m_src(src), label(currentLabel)
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

void PrintVariables(const Variables& vars)
{
	for (Variables::const_iterator it = vars.begin(); it != vars.end(); it++)
		cout << *(*it) << " ";
}

void Instruction::PrintInstruction()
{
	cout << "Instruction " << m_position << endl;

	cout << "SRC: ";
	PrintVariables(m_src);

	cout << endl << "DEST: ";
	PrintVariables(m_dst);

	cout << endl << "USE: ";
	PrintVariables(m_use);

	cout << endl << "DEF: ";
	PrintVariables(m_def);

	cout << endl << "IN: ";
	PrintVariables(m_in);

	cout << endl << "OUT: ";
	PrintVariables(m_out);

	cout << endl;
}

ostream& operator<<(ostream& out, const Instruction& i)
{
	switch (i.m_type)
	{
	case I_ADD: // add rid,rid,rid
		out << "add " << 
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

	return out;
}

InstructionType Instruction::getType()
{
	return m_type;
}

std::string Instruction::getLabelName()
{ 
	return labelName; 
}

std::string Instruction::GetLabel()
{
	return label;
}

/////////// RelInstruction

RelInstruction::RelInstruction(int pos, InstructionType type, Variables& dst, Variables& src, int num, const std::string& labelName, const std::string& currentLabel)
	: Instruction(pos, type, dst, src, labelName, currentLabel), numValue(num)
{
}

RelInstruction::~RelInstruction()
{
}

int RelInstruction::GetNumValue()
{
	return numValue;
}