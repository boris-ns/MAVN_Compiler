#pragma once

#include "LexicalAnalysis.h"
#include "Token.h"
#include "IR.h"
#include "Tree.h"

class SyntaxAnalysis
{
public:

	SyntaxAnalysis(LexicalAnalysis& lex);

	/* Method which performs lexical analysis */
	bool Do();

	/* Prints the token that caused the syntax error */
	void printSyntaxError(Token& token);

	/**
	* Eats the current token if its type is "t"
	* otherwise reports syntax error
	*
	* param[in] - t - the expected token type
	*/
	void eat(TokenType t);

	/* Returns the next token from the token list */
	Token getNextToken();

private:

	Variables memoryVariables;
	Variables registerVariables;
	Instructions instructions;

	// @TODO: da li labele i funkcije posmatramo kao istu stvar ?
	// @TODO: dodati proveru za postojanje labele i funkcije kod b i bltz
	std::list<std::string> labels;
	std::list<std::string> functions;

	Variable* ContainsMemoryVar(Variable& var);
	Variable* ContainsMemoryVar(const std::string& varName);
	Variable* ContainsRegisterVar(Variable& var);
	Variable* ContainsRegisterVar(const std::string& varName);

	bool ContainsLabel(const std::string& id);
	bool ContainsFunction(const std::string& id);

	void CheckMemVariableExistance(Token& t);
	void CheckRegVariableExistance(Token& t);
	
	void AddMemVarToList(Token& t);
	void AddRegVarToList(Token& t);
	void AddFunctionToList(Token& t);
	void AddLabelToList(Token& t);

	void CreateInstruction(InstructionType type, vector<Token>& dst, vector<Token>& src);

	/* Reference to lexical analysis module */
	LexicalAnalysis& lexicalAnalysis;

	/* Iterator to the token list which represents the output of the lexical analysis */
	TokenList::iterator tokenIterator;

	/* Current token that is being analyzed */
	Token currentToken;

	/* Nonterminal Q */
	void q();

	/* Nonterminal S */
	void s();

	/* Nonterminal L */
	void l();

	/* Nonterminal E */
	void e();

	/* Syntax error indicator */
	bool errorFound;
};

