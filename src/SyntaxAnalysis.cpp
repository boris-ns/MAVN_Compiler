#include <iostream>
#include <algorithm>
#include <regex>
#include "SyntaxAnalysis.h"
#include "Tree.h"

using namespace std;

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex, Instructions& instr) 
	: lexicalAnalysis(lex), errorFound(false), instructions(instr), instructionCounter(0), variableCounter(0)
{
	tokenIterator = lexicalAnalysis.getTokenList().begin();
}

/* Proverava da li je mem. promenljiva deklarisana. */
Variable* SyntaxAnalysis::ContainsMemoryVar(Variable& var)
{
	for (Variables::iterator it = memoryVariables.begin(); it != memoryVariables.end(); it++)
	{
		if ((*it)->getName() == var.getName())
			return (*it);
	}

	return NULL;
}

/* Proverava da li je reg. promenljiva deklarisana. */
Variable* SyntaxAnalysis::ContainsRegisterVar(Variable& var)
{
	for (Variables::iterator it = registerVariables.begin(); it != registerVariables.end(); it++)
	{
		if ((*it)->getName() == var.getName())
			return (*it);
	}

	return NULL;
}

/* Proverava da li je mem. promenljiva deklarisana. */
Variable* SyntaxAnalysis::ContainsMemoryVar(const std::string& varName)
{
	for (Variables::iterator it = memoryVariables.begin(); it != memoryVariables.end(); it++)
	{
		if ((*it)->getName() == varName)
			return (*it);
	}

	return NULL;
}

/* Proverava da li je reg. promenljiva deklarisana. */
Variable* SyntaxAnalysis::ContainsRegisterVar(const std::string& varName)
{
	for (Variables::iterator it = registerVariables.begin(); it != registerVariables.end(); it++)
	{
		if ((*it)->getName() == varName)
			return (*it);
	}

	return NULL;
}

/* Proverava da li je labela prethodno definisana. */
bool SyntaxAnalysis::ContainsLabel(const std::string& id)
{
	for (map<string, int>::iterator it = labels.begin();
		it != labels.end();
		it++)
	{
		if (it->first == id)
			return true;
	}

	return false;
}

/* Proverava da li je funkcija prethodno definisana. */
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

/* Proverava da li se promenljiva koja se moze napraviti od
   tokena nalazi u listi memorijskih promenljivih */
void SyntaxAnalysis::CheckMemVariableExistance(Token& t)
{
	string variableName = t.getValue();
	Variable variable(variableName);

	if (ContainsMemoryVar(variable) == NULL)
	{
		cout << "Memorijska promenljiva " << variableName << " nije deklarisana." << endl;
		errorFound = true;
	}
}

/* Proverava da li se promenljiva koja se moze napraviti od
   tokena nalazi u listi registarskih promenljivih */
void SyntaxAnalysis::CheckRegVariableExistance(Token& t)
{
	string variableName = t.getValue();
	Variable variable(variableName);

	if (ContainsRegisterVar(variable) == NULL)
	{
		cout << "Registarska promenljiva " << variableName << " nije deklarisana." << endl;
		errorFound = true;
	}
}

/* Proverava da li je promenljiva vec deklarisana i da li
   pocinje na slovo 'm'. Smesta promenljivu u listu. */
void SyntaxAnalysis::AddMemVarToList(Token& t)
{
	string variableName = t.getValue();
	Variable* variable = new Variable(variableName);

	if (!regex_match(variableName, regex("m[0-9]+")))
	{
		errorFound = true;
		cout << "Memorijska promenljiva: " << variableName << " nije dobro definisana." << endl;
		return;
	}
	if (ContainsMemoryVar(*variable) != NULL)
	{
		errorFound = true;
		cout << "Memorijska promenljiva: " << variableName << " je vec deklarisana." << endl;
		return;
	}

	memoryVariables.push_back(variable);
}

/* Proverava da li je promenljiva vec deklarisana i da li
pocinje na slovo 'r'. Smesta promenljivu u listu. */
void SyntaxAnalysis::AddRegVarToList(Token& t)
{
	string variableName = t.getValue();
	Variable* variable = new Variable(variableName, variableCounter);
	++variableCounter;

	if (!regex_match(variableName, regex("r[0-9]+")))
	{
		errorFound = true;
		cout << "Registarska promenljiva: " << variableName << " nije dobro definisana." << endl;
		return;
	}
	if (ContainsRegisterVar(*variable) != NULL)
	{
		errorFound = true;
		cout << "Registarska promenljiva: " << variableName << " je vec deklarisana." << endl;
		return;
	}

	registerVariables.push_back(variable);
}

void SyntaxAnalysis::AddFunctionToList(Token& t)
{
	string funcName = t.getValue();

	if (!regex_match(funcName, regex("[A-Za-z]+[A-Za-z0-9]*")))
	{
		cout << "Funkcija " << funcName << " nema ime u skladu sa pravilima." << endl;
		errorFound = true;
		return;
	}
	if (ContainsFunction(funcName))
	{
		cout << "Funkcija " << funcName << " je vec definisana." << endl;
		errorFound = true;
		return;
	}

	functions.push_back(funcName);
}

void SyntaxAnalysis::AddLabelToList(const std::string& labelName, int pos)
{
	if (ContainsLabel(labelName))
	{
		cout << "Labela " << labelName << " je vec definisana." << endl;
		errorFound = true;
		return;
	}

	labels[labelName] = pos;
}

// dst - da li mora da bude vektor ili je uvek samo 1 element ?
void SyntaxAnalysis::CreateInstruction(InstructionType type, vector<Token>& dst, vector<Token>& src)
{
	Variables* destVars = new Variables;
	Variables* srcVars = new Variables;
	string labelName = "";

	switch (type)
	{
	case I_ADD: // add rid,rid,rid
	case I_SUB: // sub rid,rid,rid
		destVars->push_back(ContainsRegisterVar(dst[0].getValue()));
		srcVars->push_back(ContainsRegisterVar(src[0].getValue()));
		srcVars->push_back(ContainsRegisterVar(src[1].getValue()));
		break;

	case I_ADDI: // addi rid,rid,num
		destVars->push_back(ContainsRegisterVar(dst[0].getValue()));
		srcVars->push_back(ContainsRegisterVar(src[0].getValue()));
		break;

	case I_LA: // la rid,mid
		destVars->push_back(ContainsRegisterVar(dst[0].getValue()));
		srcVars->push_back(ContainsMemoryVar(src[0].getValue()));
		break;

	case I_LW: // lw rid,num(rid)
	case I_SW:
		destVars->push_back(ContainsRegisterVar(dst[0].getValue()));
		//@TODO: sta sa num(rid) ????
		break;

	case I_LI: // li rid,num
		destVars->push_back(ContainsRegisterVar(dst[0].getValue()));
		break;

	case I_BLTZ: // bltz rid,id
		srcVars->push_back(ContainsRegisterVar(src[0].getValue()));
		labelName = dst[0].getValue();
		break;

	case I_B:   // b id
		labelName = dst[0].getValue();
		break;

	case I_NOP: // nop
		break;
	}

	Instruction* instr = new Instruction(instructionCounter, type, *destVars, *srcVars, labelName);
	instructions.push_back(instr);
	++instructionCounter;
}

void SyntaxAnalysis::FillSuccessors()
{
	for (Instructions::iterator it = instructions.begin();
		it != instructions.end() - 1;
		it++)
	{
		(*it)->m_succ.push_back(*(it + 1));

		// Ako je instrukcija tipa JUMP onda treba dodati jos jednog naslednika
		if ((*it)->getType() == I_B || (*it)->getType() == I_BLTZ)
		{
			int instrPosition = labels[(*it)->getLabelName()];
			Instruction* instrToJump = instructions.at(instrPosition);
			(*it)->m_succ.push_back(instrToJump);

			// Kad smo vec dosli do instr. na koju se skace, dodamo i prethodnika (trenutnu instr.)
			instrToJump->m_pred.push_back(*it);
		}
	}
}

void SyntaxAnalysis::FillPredecessor()
{
	for (Instructions::reverse_iterator it = instructions.rbegin();
		it != instructions.rend() - 1;
		it++)
	{
		(*it)->m_pred.push_back(*(it + 1));
	}
}

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


void SyntaxAnalysis::printSyntaxError(Token& token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}


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


Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}


void SyntaxAnalysis::q()
{
	s();
	eat(T_SEMI_COL);
	l();
}


void SyntaxAnalysis::s()
{
	string name;

	switch (currentToken.getType())
	{
	case T_MEM: // _mem mid num
		eat(T_MEM);
		AddMemVarToList(currentToken);
		eat(T_M_ID);
		eat(T_NUM);
		break;

	case T_REG: // _reg rid
		eat(T_REG);
		AddRegVarToList(currentToken);
		eat(T_R_ID);
		break;

	case T_FUNC: // _func id
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


void SyntaxAnalysis::l()
{
	if (currentToken.getType() == T_END_OF_FILE)
		eat(T_END_OF_FILE);
	else
		q();
}


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
		eat(T_NUM);
		CreateInstruction(I_ADDI, dst, src);
		break;

	case T_SUB: // sub rid,rid,rid
		eat(T_SUB);
		dst.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		src.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
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
		eat(T_NUM);
		eat(T_L_PARENT);
		src.push_back(currentToken);
		eat(T_R_ID);
		eat(T_R_PARENT);
		CreateInstruction(I_LW, dst, src);
		break;

	case T_LI: // li rid,num
		eat(T_LI);
		dst.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		CreateInstruction(I_LI, dst, src);
		break;

	case T_SW: // sw rid,num(rid)
		eat(T_SW);
		dst.push_back(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
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

Variables& SyntaxAnalysis::GetRegVariables()
{
	return registerVariables;
}