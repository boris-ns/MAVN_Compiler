//==============================================================================
// Name        : main.cpp
// Author      : Boris Sulicenko
// Date created: 17.05.2018.
// Description : MAVN compiler. Translates high level mips assembly to 
//               low level mips assembly.
//==============================================================================

#include <iostream>
#include <exception>
#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"

using namespace std;

void main()
{
	try
	{
		std::string fileName = ".\\..\\examples\\simple.mavn";
		bool retVal = false;

		// Run lexical analysis
		LexicalAnalysis lex;
		
		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");
		
		lex.initialize();
		retVal = lex.Do();

		if (retVal)
		{
			cout << "Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}

		cout << endl << "------------------------------------"
			 << endl << "Starting syntax analysis" << endl;

		// Run syntax analysis
		Instructions instructions;
		SyntaxAnalysis syntax(lex, instructions);
		retVal = syntax.Do();

		if (retVal)
		{
			cout << "Syntax analysis finished successfully!" << endl;
		}
		else
		{
			throw runtime_error("\nException! Syntax analysis failed!\n");
		}

		// Finishing instruction creation
		syntax.FillSuccessors();
		syntax.FillPredecessor();

		LivenessAnalysis(instructions);
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
	}
}