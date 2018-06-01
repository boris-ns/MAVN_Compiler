#pragma once

#include "LexicalAnalysis.h"
#include "Token.h"
#include "IR.h"

/* Vector of pairs - name of label and position of first instr. */
typedef std::vector<std::pair<std::string, int>> Labels;

/* Class for syntax analysis, fill labels, functions, 
   register and memory variables, creating instruction. */
class SyntaxAnalysis
{
public:

	SyntaxAnalysis(LexicalAnalysis& lex, Instructions& instr);
	~SyntaxAnalysis();

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

	/* Methods for creating succesors and predcessors. */
	void FillSuccessors();
	void FillPredecessor();

	/* Creates MIPS assembly file, from instructions and variables. */
	void CreateMIPSFile(const std::string& filePath);

	/* Getters */
	Variables& GetRegVariables();
	Variables& GetMemoryVariables();

private:

	/* Iterator to the token list which represents the output of the lexical analysis */
	TokenList::iterator tokenIterator;
	LexicalAnalysis& lexicalAnalysis;  // Reference to lexical analysis module
	Token currentToken;                // Current token that is being analyzed/
	bool errorFound;                   // Syntax error indicator
	int instructionCounter;            // Counts instructions
	int variableCounter;               // Counts variables
	std::string currentLabel;          // Holds name of current label
									   
	Instructions& instructions;        // List of all instructions
	Variables memoryVariables;         // List of memory variables
	Variables registerVariables;       // List of register variables

	Labels labels;						// Holds name of labels and functions,
										// and position of it's first instruction
	std::list<std::string> functions;	// Hold all function names

	/* Methods that check if mem/reg variables are already defined. */
	Variable* ContainsMemoryVar(Variable& var);
	Variable* ContainsMemoryVar(const std::string& varName);
	Variable* ContainsRegisterVar(Variable& var);
	Variable* ContainsRegisterVar(const std::string& varName);

	/* Methods that check if label/function are already defined. */
	bool ContainsLabel(const std::string& id);
	bool ContainsFunction(const std::string& id);
	
	/* Adds mem/reg variables to the list of mem/reg vars. */
	void AddMemVarToList(Token& t, Token& value);
	void AddRegVarToList(Token& t);

	/* Adds function to the list and also labels map. */
	void AddFunctionToList(Token& t);

	/* Adds label to the map with position of it's first instruction. */
	void AddLabelToList(const std::string& labelName, int pos);

	/* Creates instruction from destination and source tokens.
	   Also adds instruction to the list. */
	void CreateInstruction(InstructionType type, vector<Token>& dst, vector<Token>& src);

	/* Gets position of instruction from list of labels. */
	int GetInstrPositionFromLabel(std::string label);

	void q(); /* Nonterminal Q */
	void s(); /* Nonterminal S */
	void l(); /* Nonterminal L */
	void e(); /* Nonterminal E */
};

