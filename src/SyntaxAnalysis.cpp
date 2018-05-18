#include <iostream>
#include <algorithm>
#include "SyntaxAnalysis.h"
#include "Tree.h"

using namespace std;

bool SyntaxAnalysis::ContainsMemoryVar(Variable& var)
{
	for (Variables::iterator it = memoryVariables.begin(); it != memoryVariables.end(); it++)
	{
		if ((*it)->getName() == var.getName())
			return true;
	}

	return false;
}

bool SyntaxAnalysis::ContainsRegisterVar(Variable& var)
{
	for (Variables::iterator it = registerVariables.begin(); it != registerVariables.end(); it++)
	{
		if ((*it)->getName() == var.getName())
			return true;
	}

	return false;
}


SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex) : lexicalAnalysis(lex), errorFound(false)
{
	tokenIterator = lexicalAnalysis.getTokenList().begin();
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

	string variableName;
	Variable* variable;

	switch (currentToken.getType())
	{
	case T_MEM: // _mem mid num
		eat(T_MEM);

		// Ubacujemo memorijsku promenljivu u listu
		variableName = currentToken.getValue();
		variable = new Variable(variableName);
		eat(T_M_ID);
		eat(T_NUM);

		if (ContainsMemoryVar(*variable))
		{
			errorFound = true;
			cout << "Memorijska promenljiva: " << variableName << " je vec deklarisana." << endl;
			return;
		}
		else if (variableName.at(0) == 'r')
		{
			errorFound = true;
			cout << "Memorijska promenljiva: " << variableName << " mora da pocne sa slovom 'm'." << endl;
			return;
		}
		memoryVariables.push_back(variable);
		syntaxTree.push_back(new DeclStm(variable)); // @TODO: da li ce morati poseban declstm za reg. i mem.?
		break;

	case T_REG: // _reg rid
		eat(T_REG);

		// Ubacujemo registarsku promenljivu u listu
		variableName = currentToken.getValue();
		variable = new Variable(variableName);
		eat(T_R_ID);

		if (ContainsRegisterVar(*variable))
		{
			errorFound = true;
			cout << "Registarska promenljiva: " << variableName << " je vec deklarisana." << endl;
			return;
		}
		else if (variableName.at(0) == 'r')
		{
			errorFound = true;
			cout << "Registarska promenljiva: " << variableName << " mora da pocne sa slovom 'r'." << endl;
			return;
		}

		registerVariables.push_back(variable);
		syntaxTree.push_back(new DeclStm(variable));
		break;

	case T_FUNC:
		eat(T_FUNC);
		eat(T_ID);
		break;

	case T_ID:
		eat(T_ID);
		eat(T_COL);
		e();
		break;

	default:
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
	case T_ADD:
		eat(T_ADD);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		break;

	case T_ADDI:
		eat(T_ADDI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		break;

	case T_SUB:
		eat(T_SUB);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		break;

	case T_LA:
		eat(T_LA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_M_ID);
		break;

	case T_LW:
		eat(T_LW);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		eat(T_R_ID);
		eat(T_R_PARENT);
		break;

	case T_LI:
		eat(T_LI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		break;

	case T_SW:
		eat(T_SW);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		eat(T_R_ID);
		eat(T_R_PARENT);
		break;

	case T_B:
		eat(T_B);
		eat(T_ID);
		break;
	
	case T_BLTZ:
		eat(T_BLTZ);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_ID);
		break;

	case T_NOP:
		eat(T_NOP);
		break;

	default:
		errorFound = true;
	}
}
