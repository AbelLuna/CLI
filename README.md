# SIC (Simplified Instructional Computer) Computer Simulation
This is a program written in C and meant to run on LINUX. It is a hypothetical computer system introduced in 
_System Software: An Introduction to Systems Programming_, by Leland Beck. The purpose of this program is to serve as an educational tool 
in how system programming works without using a real-world system which has complex behaviors.

## How to use 
Upon opening the program, the user will have the following commands:  
1. `load <filename>` Will load the specified file. 
2. `execute` Will call the computer simulation program to execute the program that was previously loaded in memory.
3. `debug` Will allow you to execute in debug mode.
4. `dump <start> <end>` Will dump the contents in memory, <start> and <end> are hexadecimal values dictating the start and end of memory that you want to dump.
5. `help` Will print a list of available commands.
6. `assemble <filename>` Will assemble an SIC assembly language program into a load module and store it in a file.
7. `directory` Will list files stored in the current directory. Note: `dir` can also be used as an alternative.
8. `exit` Will exit from the simulator.  

## How it works  
| ![Imgur](http://i.imgur.com/FxFPlyJ.png) 
| --- |
| **Workflow.** |  

The program needs a source file with SIC machine code in order to assemble it. More information can be found here: https://en.wikipedia.org/wiki/Simplified_Instructional_Computer  

Once you have assembled the source code, you can then load it into memory using the `load <filename>` command. After this is done,
you can then execute the program by using `execute`. Note: If there are errors in the source code, the program will not give you errors,
but it will not work when you execute it.  

The program cannot have the following in the source code:
1. Duplicate labels
2. Illegal label (A label cannot be a directive, menumonic, etc.)
3. Illegal operation.
4. Missing or illegal operand on data storage directive.
5. Missing or illegal operand on START directive.
6. Missing or illegal operand on END directive.
7. Too many symbols in source program.
8. Program too long. (Program exceeds bounds; greater than 0xFFFF)  
If any of the mentioned errors are found, it will be flagged but will still run.

## Output
Depending on the source code that the user loaded onto memory, the program can do various amount of things.
