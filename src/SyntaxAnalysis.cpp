#include <iostream>
#include <algorithm>
#include "SyntaxAnalysis.h"
#include "Tree.h"

using namespace std;

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex) : lexicalAnalysis(lex), errorFound(false)
{
	tokenIterator = lexicalAnalysis.getTokenList().begin();
}

/* Proverava da li je mem. promenljiva deklarisana. */
bool SyntaxAnalysis::ContainsMemoryVar(Variable& var)
{
	for (Variables::iterator it = memoryVariables.begin(); it != memoryVariables.end(); it++)
	{
		if ((*it)->getName() == var.getName())
			return true;
	}

	return false;
}

/* Proverava da li je reg. promenljiva deklarisana. */
bool SyntaxAnalysis::ContainsRegisterVar(Variable& var)
{
	for (Variables::iterator it = registerVariables.begin(); it != registerVariables.end(); it++)
	{
		if ((*it)->getName() == var.getName())
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

	if (!ContainsMemoryVar(variable))
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

	if (!ContainsRegisterVar(variable))
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

	if (ContainsMemoryVar(*variable))
	{
		errorFound = true;
		cout << "Memorijska promenljiva: " << variableName << " je vec deklarisana." << endl;
		return;
	}
	if (variableName.at(0) != 'm') 
	{
		errorFound = true;
		cout << "Memorijska promenljiva: " << variableName << " mora da pocne sa slovom 'm'." << endl;
		return;
	}

	memoryVariables.push_back(variable);
}

/* Proverava da li je promenljiva vec deklarisana i da li
pocinje na slovo 'r'. Smesta promenljivu u listu. */
void SyntaxAnalysis::AddRegVarToList(Token& t)
{
	string variableName = t.getValue();
	Variable* variable = new Variable(variableName);

	if (ContainsRegisterVar(*variable))
	{
		errorFound = true;
		cout << "Registarska promenljiva: " << variableName << " je vec deklarisana." << endl;
		return;
	}
	if (variableName.at(0) != 'r')
	{
		errorFound = true;
		cout << "Registarska promenljiva: " << variableName << " mora da pocne sa slovom 'r'." << endl;
		return;
	}

	registerVariables.push_back(variable);
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
	if (errorFound)
		return;

	s();
	eat(T_SEMI_COL);
	l();
}


void SyntaxAnalysis::s()
{
	if (errorFound)
		return;

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
		eat(T_ID);
		break;

	case T_ID: // id : e
		eat(T_ID);
		eat(T_COL);
		e();
		break;

	default: // e
		e();
	}
}


void SyntaxAnalysis::l()
{
	if (errorFound)
		return;

	if (currentToken.getType() == T_END_OF_FILE)
		eat(T_END_OF_FILE);
	else
		q();
}


void SyntaxAnalysis::e()
{
	if (errorFound)
		return;

	switch (currentToken.getType())
	{
	case T_ADD: // add rid,rid,rid
		eat(T_ADD);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		break;

	case T_ADDI: // addi rid,rid,num
		eat(T_ADDI);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		break;

	case T_SUB: // sub rid,rid,rid
		eat(T_SUB);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		break;

	case T_LA: // la rid,mid
		eat(T_LA);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		CheckMemVariableExistance(currentToken);
		eat(T_M_ID);
		break;

	case T_LW: // lw rid, num(rid)
		eat(T_LW);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_R_PARENT);
		break;

	case T_LI: // li rid,num
		eat(T_LI);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		break;

	case T_SW: // sw rid,num(rid)
		eat(T_SW);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_R_PARENT);
		break;

	case T_B: // b id
		eat(T_B);
		eat(T_ID);
		break;
	
	case T_BLTZ: // bltz rid,id
		eat(T_BLTZ);
		CheckRegVariableExistance(currentToken);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_ID);
		break;

	case T_NOP: // nop
		eat(T_NOP);
		break;

	default:
		errorFound = true;
	}
}
