#pragma once

#include <iostream>
#include <vector>
#include "Types.h"

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

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
	Variable(std::string name) : m_name(name), m_type(NO_TYPE), m_position(-1), m_assignment(no_assign) {}

	Variable(std::string name, int pos, VariableType type);
	std::string getName();
	int GetPos();
	VariableType GetType();
	void SetAssignment(Regs r);

	friend std::ostream& operator<<(std::ostream& out, const Variable& v);

private:
	VariableType m_type;
	std::string m_name;
	int m_position;
	Regs m_assignment;
};


/* This type represents list of variables from program code. */
typedef std::list<Variable*> Variables;


/* This class represents one instruction in program code. */
class Instruction
{
public:
	Instruction();
	Instruction(int pos, InstructionType type, Variables& dst, Variables& src, const std::string& labelName, const std::string& currentLabel);

	InstructionType getType();
	std::string getLabelName();
	std::string GetLabel();
	void PrintInstruction();

	std::vector<Instruction*> m_succ;
	std::vector<Instruction*> m_pred;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;

	friend std::ostream& operator<<(std::ostream& out, const Instruction& i);

private:
	void FillUseDefVariables();

	int m_position;
	std::string labelName;  // Label name for jump instructions
	std::string label;
	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;
};

class RelInstruction : public Instruction
{
public:
	RelInstruction(int pos, InstructionType type, Variables& dst, Variables& src, int num, const std::string& labelName, const std::string& currentLabel);
	~RelInstruction();

	int GetNumValue();

private:
	int numValue;
};


/* This type represents list of instructions from program code. */
typedef std::vector<Instruction*> Instructions;

bool ContainsVariable(Variable& variable, Variables& variableList);
void LivenessAnalysis(Instructions& instructions);
void PrintInstructions(Instructions& instructions);