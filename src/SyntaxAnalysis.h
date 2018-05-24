#pragma once

#include "LexicalAnalysis.h"
#include "Token.h"
#include "IR.h"

class SyntaxAnalysis
{
public:

	SyntaxAnalysis(LexicalAnalysis& lex, Instructions& instr);

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

	void FillSuccessors();
	void FillPredecessor();
	Variables& GetRegVariables();
	Variables& GetMemoryVariables();
	std::list<std::string>& GetFunctions();
	std::map<std::string, int>& GetLabels();

	void CreateMIPSFile(const std::string& filePath);

private:

	/* Iterator to the token list which represents the output of the lexical analysis */
	TokenList::iterator tokenIterator;
	LexicalAnalysis& lexicalAnalysis; /* Reference to lexical analysis module */
	Token currentToken;               /* Current token that is being analyzed */
	bool errorFound;                  /* Syntax error indicator */
	int instructionCounter;           /* Counts instructions */
	int variableCounter;              /* Counts variables */
	std::string currentLabel;

	Instructions& instructions;
	Variables memoryVariables;
	Variables registerVariables;

	// @TODO: dodati proveru za postojanje labele i funkcije kod b i bltz
	// Mapa sadrzi ime labele i poziciju instrukcije koja sledi nakon tabele
	std::map<std::string, int> labels;
	std::list<std::string> functions;

	Variable* ContainsMemoryVar(Variable& var);
	Variable* ContainsMemoryVar(const std::string& varName);
	Variable* ContainsRegisterVar(Variable& var);
	Variable* ContainsRegisterVar(const std::string& varName);

	bool ContainsLabel(const std::string& id);
	bool ContainsFunction(const std::string& id);

	void CheckMemVariableExistance(Token& t);
	void CheckRegVariableExistance(Token& t);
	
	void AddMemVarToList(Token& t, Token& value);
	void AddRegVarToList(Token& t);
	void AddFunctionToList(Token& t);
	void AddLabelToList(const std::string& labelName, int pos);

	void CreateInstruction(InstructionType type, vector<Token>& dst, vector<Token>& src);

	void q(); /* Nonterminal Q */
	void s(); /* Nonterminal S */
	void l(); /* Nonterminal L */
	void e(); /* Nonterminal E */
};

