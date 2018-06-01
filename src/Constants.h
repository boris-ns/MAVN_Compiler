#pragma once

/**
 * State symbol definitions
 */
#define IDLE_STATE				0
#define START_STATE				1
#define INVALID_STATE			(-2)

/**
 * Number of states in FSM
 */
#define NUM_STATES				47

/**
 * Number of supported characters
 */
#define NUM_OF_CHARACTERS		47

/**
 * Use this when instruction interference to other instruction.
 */
#define __INTERFERENCE__				1

/**
 * Use this when instruction is no interference to other instruction.
 */
#define __EMPTY__						0

/**
 * Number of regs in processor.
 */
#define __REG_NUMBER__					5

/**
 * Use these to print liveness analysis dump.
 */
#define __DUMPS__						1
#define __NO_DUMPS__					0

/**
 * Alignment definitions for nice printing
 */
#define LEFT_ALIGN				20
#define RIGHT_ALIGN				25
