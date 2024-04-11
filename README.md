This is a SIC/XE asssembler which supports literals, expressions and program blocks along with noraml asseembly of SIC/XE instructions.

This guide provides instructions on how to use the SIC/XE assembler program.

1. Ensure you have the following files in the current directory:  
o assembler.h 
o utility_functions.h 
o data.h 
o Handling.cpp 
2. Name your SIC/XE program file as "<program_name>.txt" and place it in the same directory. Ensure 
that program is written in upper case. 
Running the Assembler: 
1. Open a terminal in the directory containing the files. 
2. Run the "Handling.cpp" file using a command like g++ Handling.cpp -o handler (replace 
g++ with your C++ compiler if needed). 
3. Execute the generated assembler program (e.g., ./handler). 
4. Enter the file name <program_name>.txt for which assembling needs to be done. 
Output Files: 
Command line show status of assembly. 
The program will generate three output files and some intermediate files: 
Output files 
• object_program.txt: Stores the final object program in machine code format. 
• Listing_file.txt: Displays the original assembly code with additional information like  
line no., addresses (location counter), block numbers, object code for each line. 
• Data_tables.txt: Contains data generated during the first pass of the assembler like 
Symbol Table and Block Table 
Intermediate files  
• error.txt: Contains all errors occurred during pass1 or pass2 while assembling. 
• intermediate.txt: Contains the data used by pass2, generated during pass1.
