#include "SyntaxAnalysis.h"
#include <iostream>

using namespace std;


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

	switch (currentToken.getType())
	{
	case T_MEM:
		eat(T_MEM);
		eat(T_M_ID);
		eat(T_NUM);
		break;

	case T_REG:
		eat(T_REG);
		eat(T_R_ID);
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
