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
#include "InterferenceGraph.h"

using namespace std;

void main(int argc, char** argv)
{
	string inputFile, outputFile;
	
	// Command line arguments handling
	if (argc == 3)
	{
		inputFile = argv[1];
		outputFile = argv[2];
	}
	else 
	{
		cout << "Wrong number of command line arguments." << endl;
		cout << "Run the program like this: LexicalAnalysis.exe "
			 << "<input_mavn_file> <output_mips_file>" << endl;
		return;
	}

	/*inputFile = "../examples/label_name_same_as_func.mavn";
	outputFile = "../examples/not_defined_label.s";*/

	try
	{
		bool retVal = false;

		// Run lexical analysis
		LexicalAnalysis lex;
		
		if (!lex.readInputFile(inputFile))
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
			cout << "Syntax analysis finished successfully!" << endl;
		else
			throw runtime_error("\nException! Syntax analysis failed!\n");

		// Finishing instruction creation
		syntax.FillSuccessors();
		syntax.FillPredecessor();

		cout << endl << "------------------------------------" << endl;
		cout << endl << "Instruction list" << endl << endl;
		PrintInstructions(instructions);

		// Do liveness analysis
		LivenessAnalysis(instructions);

		cout << endl << "Instruction list after liveness analysis" << endl << endl;
		PrintInstructions(instructions);

		// Create interference graph, do resource allocation
		InterferenceGraph ig(syntax.GetRegVariables());
		ig.BuildInterferenceGraph(instructions);
		ig.BuildVariableStack();

		// Print interference matrix
		cout << endl << "Interference matrix:" << endl;
		ig.PrintInterferenceMatrix();

		if (ig.ResourceAllocation())
			cout << endl << "Resource allocation successful!" << endl;
		else
			throw runtime_error("\nException! Resource allocation failed!!\n");

		syntax.CreateMIPSFile(outputFile);
		
		cout << endl << "Output MIPS file is created at location " << outputFile << endl;
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
	}
}