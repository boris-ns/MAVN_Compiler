#pragma once

#include "Types.h"
#include "RegisterAllocation.h"

/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

	// @TODO: Sta sa m_position = -1 ?
	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
	Variable(std::string name) : m_name(name), m_type(NO_TYPE), m_position(-1), m_assignment(no_assign) {}
	Variable(std::string name, int pos) : m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign) {}

	bool operator==(const Variable& var)
	{
		return (m_name == var.m_name) ? true : false;
	}

	std::string getName() { return m_name; }

private:
	VariableType m_type;
	std::string m_name;
	int m_position;
	RegsType m_assignment;
};


/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;

/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	Instruction (int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src) {}

	// @TODO: Correct ?
	Instruction(std::string asmString, Regs* dest, Regs* src) :
		m_asmString(asmString), m_dest_regs(dest), m_src_regs(src) {}

private:
	int m_position;
	InstructionType m_type;
	
	std::string m_asmString;

	Regs* m_dest_regs;
	Regs* m_src_regs;

	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;
