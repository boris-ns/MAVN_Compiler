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

	Variable();
	Variable(std::string name);
	Variable(std::string name, int pos, VariableType type, int val=0);
	~Variable();

	/* Getters & setters*/
	int GetPos();
	int GetValue();
	Regs GetAssignment();
	std::string GetName();
	VariableType GetType();
	void SetAssignment(Regs r);

	friend std::ostream& operator<<(std::ostream& out, const Variable& v);

private:
	int value;            // value variable contains if it is memory var.
	int m_position;       // position in interference matrix
	Regs m_assignment;    // assigned register
	std::string m_name;
	VariableType m_type;
};


/* This type represents list of variables from program code. */
typedef std::list<Variable*> Variables;


/* This class represents one instruction in program code. */
class Instruction
{
public:
	Instruction();
	Instruction(int pos, InstructionType type, Variables& dst, Variables& src,
				const std::string& labelName, const std::string& currentLabel);
	~Instruction();

	/* Getters */
	InstructionType GetType();
	std::string GetLabelName();
	std::string GetLabel();
	
	/* Prints instruction with all variable lists. */
	void PrintInstruction();

	std::vector<Instruction*> m_succ; // Successors
	std::vector<Instruction*> m_pred; // Predcessors

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;

	/* Prints instruction in MIPS format. */
	friend std::ostream& operator<<(std::ostream& out, Instruction& i);

private:

	/* Fills use and def lists with variables from dst and src. */
	void FillUseDefVariables();

	int m_position;
	std::string label;      // Name of label where instruction belongs
	std::string labelName;  // Label name for jump instructions
	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;
};

/* This class represents instruction that contains number token.
   Primary use for lw and sw instructions. */
class RelInstruction : public Instruction
{
public:
	RelInstruction(int pos, InstructionType type, Variables& dst, Variables& src,
			int num, const std::string& labelName, const std::string& currentLabel);
	~RelInstruction();

	/* Getters */
	int GetNumValue();

private:
	int numValue;
};


/* This type represents list of instructions from program code. */
typedef std::vector<Instruction*> Instructions;

/* Checks if variableList contains variable. */
bool ContainsVariable(Variable& variable, Variables& variableList);

/* Does liveness analysis and created in and out variable lists. */
void LivenessAnalysis(Instructions& instructions);

/* Prints all instructions. */
void PrintInstructions(Instructions& instructions);

/* Prints all variables. */
void PrintVariables(const Variables& vars);