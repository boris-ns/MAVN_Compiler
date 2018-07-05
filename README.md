# MAVN Compiler
This compiler translates higher level Mips assembly code to regular Mips 32bit assembly. <br />
Higher level Mips assembly introduces concepts of functions and register variables, meaning that you cannot access real registers.
For now there are only 12 instructions that compiler supports. In folder tests/mavn_files you can find some examples of input programs.
Also there are some compiled files in folder tests/compiled_files. <br />
Compiler has implemented lexical analysis with state machine, syntax analysis with recursive descent algorithm, liveness analysis, interference matrix building and register allocation. <br />
You can change number of real registers for allocation (__REG_NUMBER__) in Constants.h. Compiler also gives you error messages if there is an error with input file or 
some algorithm can't be executed.

## How to use
First you'll need to compile this program. I used Visual Studio for development, so you can add cmd-line arguments through it, or call the program from command line
like this <br />
```program.exe <input_file.mavn> <output_file.s>```
