# General
Compiler for a simple interpretative language, Gelang. The language folder contains details about the language itself, including its grammar and rules. It also provides information about the custom assembly that the source files are compiled into.

# Compiling:
Make sure you are in the `../Gelang` directory. Then run the command `make`. The executable for the compiler path is `./build/ge`. You will also need to compile the virtual machine, you can do it with `g++ ./src/virtualMachine/vm.cpp -I ./src/headers/ -o ./build/vm`.

# Runing:
Use the following command to run the compiler:
`./build/ge <inputFilePath> <outputFilePath> [-g]`
where
- `<inputFilePath>` is the path to Gelang source code
- `<outputFilePath>` is the path to the compiled code in GeAssembly
- `-g` flag enables labels in the generated assembly
To run the assemly on the virtual machine run `./build/ve <outputFilePath>`.

# Errors:
## Variables
- Attempting to use a variable as an array
- Attempting to use an array as a variable
- Declaring an array with a lower bound greater than its upper bound
- Accessing an array with a constant index outside its declared bounds

## READ instruction
- Attempting to read into a number

## WRITE instruction
- Printing a variable that has not been initialized

## := instruction
- Using uninitialized variables in an expression

## IF statments
- Using uninitialized variables in the condition

## WHILE loops
- Using uninitialized variables in the condition

## UNTIL loops
- Using uninitialized variables in the condition

## FOR loops
- Modifying the iterator of a FOR loop using the := instruction
- Modifying the iterator of a FOR loop using the READ command
- Modifying the iterator of a FOR loop through a procedure call with it as an argument
- Using uninitialized variables for the lower or upper bounds of a FOR loop
- Declaring a variable with the same name as the loop iterator


# Warrnings:
## WHILE loops
- Detecting an infinite WHILE loop

## UNTIL loops
- Detecting an infinite UNTIL loop



# Optimilizations:
## Expressions
- Expressions known at compile time are automatically evaluated
- Expressions are simplified using algebraic identities whenever possible

## IF statments
- If the condition is known at compile time (e.g., a tautology or a contradiction), it is not checked, and the appropriate section of the instruction is executed. However, its label is still generated

## WHILE loops
-If the condition is always false (a contradiction), the loop is not compiled. However, its label is still generated

## UNTIL loops
- If the condition is always true (a tautology), the loop's commands are inlined, and the condition is not checked

## FOR loops
- If the loop is degenerate (i.e., the lower bound is greater than the upper bound, or vice versa in the case of a DOWN loop), it is not compiled. However, its label is still generated