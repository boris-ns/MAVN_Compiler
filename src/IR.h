#pragma once

#include <vector>
#include "Types.h"
#include "RegisterAllocation.h"

/* This class represents one variable from program code. */
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

	Variable(std::string name, int pos);

	bool operator==(const Variable& var);
	std::string getName();

private:
	VariableType m_type;
	std::string m_name;
	int m_position;
	RegsType m_assignment;
};


/* This type represents list of variables from program code. */
typedef std::list<Variable*> Variables;


/* This class represents one instruction in program code. */
class Instruction
{
public:
	Instruction();
	Instruction(int pos, InstructionType type, Variables& dst, Variables& src, const std::string& labelName);

	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;

	InstructionType getType();
	std::string getLabelName();

private:
	void FillUseDefVariables();

	int m_position;
	std::string labelName;  // Label name for jump instructions
	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;
	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
};


/* This type represents list of instructions from program code. */
typedef std::vector<Instruction*> Instructions;
