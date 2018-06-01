#include <iostream>
#include <algorithm>
#include <regex>
#include "SyntaxAnalysis.h"

using namespace std;

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex, Instructions& instr) 
	: lexicalAnalysis(lex), errorFound(false), instructions(instr), 
	  instructionCounter(0), variableCounter(0)
{
	tokenIterator = lexicalAnalysis.getTokenList().begin();
}

SyntaxAnalysis::~SyntaxAnalysis()
{
}

/* Checks if mem. variable is already defined. */
Variable* SyntaxAnalysis::ContainsMemoryVar(Variable& var)
{
	for (Variables::iterator it = memoryVariables.begin();
		it != memoryVariables.end(); 
		it++)
	{
		if ((*it)->GetName() == var.GetName())
			return (*it);
	}

	return NULL;
}

/* Checks if mem. variable is already defined. */
Variable* SyntaxAnalysis::ContainsMemoryVar(const std::string& varName)
{
	for (Variables::iterator it = memoryVariables.begin();
		it != memoryVariables.end(); 
		it++)
	{
		if ((*it)->GetName() == varName)
			return (*it);
	}

	return NULL;
}

/* Checks if reg. variable is already defined. */
Variable* SyntaxAnalysis::ContainsRegisterVar(Variable& var)
{
	for (Variables::iterator it = registerVariables.begin();
		it != registerVariables.end(); 
		it++)
	{
		if ((*it)->GetName() == var.GetName())
			return (*it);
	}

	return NULL;
}

/* Checks if reg. variable is already defined. */
Variable* SyntaxAnalysis::ContainsRegisterVar(const std::string& varName)
{
	for (Variables::iterator it = registerVariables.begin();
		it != registerVariables.end();
		it++)
	{
		if ((*it)->GetName() == varName)
			return (*it);
	}

	return NULL;
}

/* Checks if label is already defined. */
bool SyntaxAnalysis::ContainsLabel(const std::string& id)
{
	for (Labels::iterator it = labels.begin();
		it != labels.end();
		it++)
	{
		if (it->first == id)
			return true;
	}

	return false;
}

/* Checks if function is already defined. */
bool SyntaxAnalysis::ContainsFunction(const std::string& id)
{
	for (list<string>::iterator it = functions.begin();
		it != functions.end();
		it++)
	{
		if (*it == id)
			return true;
	}

	return false;
}

/* Creates mem. var. and adds it to the list if t starts
   with 'm' and if variable isn't already defined. */
void SyntaxAnalysis::AddMemVarToList(Token& t, Token& value)
{
	string variableName = t.getValue();
	Variable* variable = new Variable(variableName, 0, 
				Variable::MEM_VAR, stoi(value.getValue()));

	if (!regex_match(variableName, regex("m[0-9]+")))
	{
		errorFound = true;
		cout << "Memorijska promenljiva: " << variableName 
			<< " nije dobro definisana." << endl;
		return;
	}
	if (ContainsMemoryVar(*variable) != NULL)
	{
		errorFound = true;
		cout << "Memorijska promenljiva: " << variableName 
			<< " je vec deklarisana." << endl;
		return;
	}

	memoryVariables.push_back(variable);
}

/* Creates reg. var. and adds it to the list if t starts
   with 'r' and if variable isn't already defined. */
void SyntaxAnalysis::AddRegVarToList(Token& t)
{
	string variableName = t.getValue();
	Variable* variable = new Variable(variableName, variableCounter, 
				Variable::REG_VAR);
	++variableCounter;

	if (!regex_match(variableName, regex("r[0-9]+")))
	{
		errorFound = true;
		cout << "Registarska promenljiva: " << variableName 
			<< " nije dobro definisana." << endl;
		return;
	}
	if (ContainsRegisterVar(*variable) != NULL)
	{
		errorFound = true;
		cout << "Registarska promenljiva: " << variableName 
			<< " je vec deklarisana." << endl;
		return;
	}

	registerVariables.push_back(variable);
}

/* Adds function to the list of functions and map of labels. */
void SyntaxAnalysis::AddFunctionToList(Token& t)
{
	string funcName = t.getValue();

	if (!regex_match(funcName, regex("[A-Za-z]+[A-Za-z0-9]*")))
	{
		cout << "Funkcija " << funcName 
			<< " nema ime u skladu sa pravilima." << endl;
		errorFound = true;
		return;
	}
	if (ContainsFunction(funcName))
	{
		cout << "Funkcija " << funcName 
			<< " je vec definisana." << endl;
		errorFound = true;
		return;
	}

	functions.push_back(funcName);
	AddLabelToList(funcName, instructionCounter);
}

/* Adds label to the map of labels. */
void SyntaxAnalysis::AddLabelToList(const std::string& labelName, int pos)
{
	if (ContainsLabel(labelName))
	{
		cout << "Labela " << labelName << " je vec definisana." << endl;
		errorFound = true;
		return;
	}

	labels.push_back(pair<string, int>(labelName, pos));
	currentLabel = labelName;
}

/* Creates instruction from destination and source tokens.
   Also adds instruction to the list. */
void SyntaxAnalysis::CreateInstruction(InstructionType type, 
					vector<Token>& dst, vector<Token>& src)
{
	Variables* destVars = new Variables;
	Variables* srcVars = new Variables;
	string labelName = "";
	Instruction* instr;

	switch (type)
	{
	case I_ADD:  // add rid,rid,rid
	case I_ADDU: // addu rid,rid,rid
	case I_SUB:  // sub rid,rid,rid
		destVars->push_back(ContainsRegisterVar(dst[0].getValue()));
		srcVars->push_back(ContainsRegisterVar(src[0].getValue()));
		srcVars->push_back(ContainsRegisterVar(src[1].getValue()));
		instr = new Instruction(instructionCounter, type, *destVars, *srcVars, 
								labelName, currentLabel);
		break;

	case I_ADDI: // addi rid,rid,num
		destVars->push_back(ContainsRegisterVar(dst[0].getValue()));
		srcVars->push_back(ContainsRegisterVar(src[0].getValue()));
		instr = new RelInstruction(instructionCounter, type, *destVars, *srcVars,
								stoi(src[1].getValue()), labelName, currentLabel);
		break;

	case I_LA: // la rid,mid
		destVars->push_back(ContainsRegisterVar(dst[0].getValue()));
		srcVars->push_back(ContainsMemoryVar(src[0].getValue()));
		instr = new Instruction(instructionCounter, type, *destVars, *srcVars, 
								labelName, currentLabel);
		break;

	case I_LW: // lw rid,num(rid)
	case I_LH: // lh rid,num(rid)
	case I_SW: // sw rid,num(rid)
		destVars->push_back(ContainsRegisterVar(dst[0].getValue()));
		srcVars->push_back(ContainsRegisterVar(src[1].getValue()));
		instr = new RelInstruction(instructionCounter, type, *destVars, *srcVars,
								stoi(src[0].getValue()), labelName, currentLabel);
		break;

	case I_LI: // li rid,num
		destVars->push_back(ContainsRegisterVar(dst[0].getValue()));
		instr = new RelInstruction(instructionCounter, type, *destVars, *srcVars, 
								stoi(src[0].getValue()), labelName, currentLabel);
		break;

	case I_BLTZ: // bltz rid,id
		srcVars->push_back(ContainsRegisterVar(src[0].getValue()));
		labelName = dst[0].getValue();
		instr = new Instruction(instructionCounter, type, *destVars, *srcVars, 
								labelName, currentLabel);
		break;

	case I_B:   // b id
		labelName = dst[0].getValue();
		instr = new Instruction(instructionCounter, type, *destVars, *srcVars, 
								labelName, currentLabel);
		break;

	case I_NOP: // nop
		instr = new Instruction(instructionCounter, type, *destVars, *srcVars, 
								labelName, currentLabel);
		break;
	}

	instructions.push_back(instr);
	++instructionCounter;
}

/* Gets position of instruction from list of labels. */
int SyntaxAnalysis::GetInstrPositionFromLabel(std::string label)
{
	for (Labels::iterator it = labels.begin();
		it != labels.end();
		it++)
	{
		if (it->first == label)
			return it->second;
	}

	return -1;
}

/* Fills list of successors for every instruction. 
   Starts from the beginning and goes to n - 1.
   Also adds predecessors for BRANCH instructions.*/
void SyntaxAnalysis::FillSuccessors()
{
	for (Instructions::iterator it = instructions.begin();
		it != instructions.end() - 1;
		it++)
	{
		// Instruction B has only 1 succ
		if ((*it)->GetType() != I_B)
			(*it)->m_succ.push_back(*(it + 1));

		// If instr. is BRANCH type then we need to add one more successor
		if ((*it)->GetType() == I_B || (*it)->GetType() == I_BLTZ)
		{
			int instrPosition = GetInstrPositionFromLabel((*it)->GetLabelName());
			Instruction* instrToJump = instructions.at(instrPosition);
			(*it)->m_succ.push_back(instrToJump);

			// Add predecessor for instr. we jump to
			instrToJump->m_pred.push_back(*it);
		}
	}
}

/* Fills list of predcessors for every instruction. 
   Starts from the end ang goes to second instruction (n-1). */
void SyntaxAnalysis::FillPredecessor()
{
	for (Instructions::reverse_iterator it = instructions.rbegin();
		it != instructions.rend() - 1;
		it++)
	{
		(*it)->m_pred.push_back(*(it + 1));
	}
}

/* Method which performs lexical analysis */
bool SyntaxAnalysis::Do()
{
	// get the first token
	currentToken = getNextToken();
	// begin analysis from start nonterminal symbol
	q();

	// check if anything has been analyzed:
	if (--tokenIterator == lexicalAnalysis.getTokenList().begin())
		return false;
	else
		return !errorFound;
}

/* Prints the token that caused the syntax error */
void SyntaxAnalysis::printSyntaxError(Token& token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}

/**
* Eats the current token if its type is "t"
* otherwise reports syntax error
*
* param[in] - t - the expected token type
*/
void SyntaxAnalysis::eat(TokenType t)
{
	if (errorFound)
		return;

	if (currentToken.getType() == t)
	{
		currentToken.printTokenValue();
		if (t != T_END_OF_FILE)
			currentToken = getNextToken();
	}
	else
	{
		printSyntaxError(currentToken);
		errorFound = true;
	}
}

/* Returns the next token from the token list */
Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}

/* Implementation of nonterminal Q */
void SyntaxAnalysis::q()
{
	s();
	eat(T_SEMI_COL);
	l();
}

/* Implementation of nonterminal S */
void SyntaxAnalysis::s()
{
	string name;
	Token t;

	switch (currentToken.getType())
	{
	case T_MEM: // _mem mid num
		eat(T_MEM);
		t = currentToken;
		eat(T_M_ID);
		AddMemVarToList(t, currentToken);
		eat(T_NUM);
		break;

	case T_REG: // _reg rid
		eat(T_REG);
		AddRegVarToList(currentToken);
		eat(T_R_ID);
		break;

	case T_FUNC: // _func id;
		eat(T_FUNC);
		AddFunctionToList(currentToken);
		eat(T_ID);
		break;

	case T_ID: // id : e
		name = currentToken.getValue();
		eat(T_ID);
		AddLabelToList(name, instructionCounter);
		eat(T_COL);
		e();
		break;

	default: // e
		e();
	}
}

/* Implementation of nonterminal L */
void SyntaxAnalysis::l()
{
	if (currentToken.getType() == T_END_OF_FILE)
		eat(T_END_OF_FILE);
	else
		q();
}

/* Implementation of nonterminal E.
   From here we fill vectors of source and destination 
   tokens and call create instruction method. */
void SyntaxAnalysis::e()
{
	vector<Token> src, dst;

	switch (currentToken.getType())
	{
	case T_ADD: // add rid,rid,rid
		eat(T_ADD);
		dst.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_R_ID);
		CreateInstruction(I_ADD, dst, src);
		break;

	case T_ADDI: // addi rid,rid,num
		eat(T_ADDI);
		dst.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_NUM);
		CreateInstruction(I_ADDI, dst, src);
		break;

	case T_ADDU: // addu rid,rid,rid
		eat(T_ADDU);
		dst.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_R_ID);
		CreateInstruction(I_ADDU, dst, src);
		break;

	case T_SUB: // sub rid,rid,rid
		eat(T_SUB);
		dst.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_R_ID);
		CreateInstruction(I_SUB, dst, src);
		break;

	case T_LA: // la rid,mid
		eat(T_LA);
		dst.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_M_ID);
		CreateInstruction(I_LA, dst, src);
		break;

	case T_LW: // lw rid, num(rid)
		eat(T_LW);
		dst.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_NUM);
		eat(T_L_PARENT);
		src.push_back(currentToken);
		eat(T_R_ID);
		eat(T_R_PARENT);
		CreateInstruction(I_LW, dst, src);
		break;

	case T_LH: // lh rid,num(rid)
		eat(T_LH);
		dst.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_NUM);
		eat(T_L_PARENT);
		src.push_back(currentToken);
		eat(T_R_ID);
		eat(T_R_PARENT);
		CreateInstruction(I_LH, dst, src);
		break;

	case T_LI: // li rid,num
		eat(T_LI);
		dst.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_NUM);
		CreateInstruction(I_LI, dst, src);
		break;

	case T_SW: // sw rid,num(rid)
		eat(T_SW);
		dst.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_NUM);
		eat(T_L_PARENT);
		src.push_back(currentToken);
		eat(T_R_ID);
		eat(T_R_PARENT);
		CreateInstruction(I_SW, dst, src);
		break;

	case T_B: // b id
		eat(T_B);
		dst.push_back(currentToken);
		eat(T_ID);
		CreateInstruction(I_B, dst, src);
		break;
	
	case T_BLTZ: // bltz rid,id
		eat(T_BLTZ);
		src.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		dst.push_back(currentToken);
		eat(T_ID);
		CreateInstruction(I_BLTZ, dst, src);
		break;

	case T_NOP: // nop
		eat(T_NOP);
		CreateInstruction(I_NOP, dst, src);
		break;

	default:
		errorFound = true;
	}
}

/* Creates MIPS assembly file from instructions and variables. */
void SyntaxAnalysis::CreateMIPSFile(const std::string& filePath)
{
	ofstream outFile(filePath);

	// Fill .globl section with functions
	for (list<string>::iterator it = functions.begin(); it != functions.end(); it++)
		outFile << ".globl " << *it << "\n";

	// Fill .data section with memory variables and it's values
	outFile << "\n.data\n";

	for (Variables::iterator it = memoryVariables.begin();
		it != memoryVariables.end();
		it++)
	{
		outFile << (*it)->GetName() << ": " << ".word " 
			    << (*it)->GetValue() << "\n";
	}

	// Fill .text section with instructions and labels
	outFile << "\n.text\n";
	
	Instructions::iterator instr = instructions.begin();
	
	// Print instructons by labels
	// (Every instructions contains in which label it should be)
	for (Labels::iterator it = labels.begin();
		it != labels.end();
		it++)
	{
		outFile << it->first << ":\n";

		// Print instructions for label *it
		while (instr != instructions.end())
		{
			// Exit condition, instruction is in another label
			if ((*instr)->GetLabel() != it->first)
				break;

			outFile << "\t" << *(*instr) << "\n";
			cout << "\t" << *(*instr) << "\n";
			instr++;
		}
	}

	outFile.close();
}

/* Getter for register variables. */
Variables& SyntaxAnalysis::GetRegVariables()
{
	return registerVariables;
}

/* Getter for memory variables. */
Variables& SyntaxAnalysis::GetMemoryVariables()
{
	return memoryVariables;
}
