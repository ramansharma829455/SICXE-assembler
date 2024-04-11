This is a SIC/XE asssembler which supports literals, expressions and program blocks along with noraml asseembly of SIC/XE instructions.

# SIC/XE Assembler Guide

This guide provides instructions on how to use the SIC/XE assembler program.

## Before You Begin

1. Ensure you have the following files in the current directory:
   - `assembler.h`
   - `utility_functions.h`
   - `data.h`
   - `Handling.cpp`

2. Name your SIC/XE program file as `<program_name>.txt` and place it in the same directory. Ensure that the program is written in uppercase.

## Running the Assembler

1. Open a terminal in the directory containing the files.
2. Compile the `Handling.cpp` file using a command like `g++ Handling.cpp -o handler` (replace `g++` with your C++ compiler if needed).
3. Execute the generated assembler program (e.g., `./handler`).
4. Enter the file name `<program_name>.txt` for which assembling needs to be done.

## Output Files

The program will generate three output files and some intermediate files:

### Output Files

- `object_program.txt`: Stores the final object program in machine code format.
- `Listing_file.txt`: Displays the original assembly code with additional information like line no., addresses (location counter), block numbers, object code for each line.
- `Data_tables.txt`: Contains data generated during the first pass of the assembler like Symbol Table and Block Table.

### Intermediate Files

- `error.txt`: Contains all errors occurred during pass1 or pass2 while assembling.
- `intermediate.txt`: Contains the data used by pass2, generated during pass1.

Command line shows status of assembly.

## Example

```bash
$ g++ Handling.cpp -o handler
$ ./handler
Enter the file name <program_name>.txt: example_program.txt
