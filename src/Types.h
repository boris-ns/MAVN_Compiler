#pragma once

#include "Constants.h"

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <stack>

/**
* Supported token types.
*/
enum TokenType
{
	T_NO_TYPE,

	T_ID,			// abcd...
	T_M_ID,			// m123...
	T_R_ID,			// r123...
	T_NUM,			// 123...
	T_WHITE_SPACE,

	// reserved words
	T_MEM,			// _mem
	T_REG,			// _reg
	T_FUNC,			// _func
	T_ADD,			// add
	T_ADDI,			// addi
	T_ADDU,			// addu
	T_SUB,			// sub
	T_LA,			// la
	T_LH,			// lh
	T_LI,			// li
	T_LW,			// lw
	T_SW,			// sw
	T_BLTZ,			// bltz
	T_B,			// b
	T_NOP,			// nop

	// operators
	T_COMMA,		//,
	T_L_PARENT,		//(
	T_R_PARENT,		//)
	T_COL,			//:
	T_SEMI_COL,		//;

	// utility
	T_COMMENT,
	T_END_OF_FILE,
	T_ERROR,
};


/**
 * Instruction type.
 */
enum InstructionType
{
	I_NO_TYPE = 0,
	I_ADD,
	I_ADDI,
	I_ADDU,
	I_SUB,
	I_LA,
	I_LH,
	I_LI,
	I_LW,
	I_SW,
	I_BLTZ,
	I_B,
	I_NOP
};

/**
 * Reg names.
 */
enum Regs
{
	no_assign = 0,
	t0,
	t1,
	t2,
	t3,
	t4,
	t5,
	t6
};

/* Prints register to the output. */
std::ostream& operator<<(std::ostream& out, Regs r);