#include "IR.h"

/////////// Variable

Variable::Variable(std::string name, int pos)
	: m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign)
{
}

bool Variable::operator==(const Variable& var)
{
	return (m_name == var.m_name) ? true : false;
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

InstructionType Instruction::getType()
{
	return m_type;
}

std::string Instruction::getLabelName()
{ 
	return labelName; 
}