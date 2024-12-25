# Compiling:
Make sure you are in the `../Gelang` directory. The run the command `make`. The executable for the compiler path is `./build/ge`

# Runing:
`./build/ge <inputFilePath> <outputFilePath> [-g]`
where
- `<inputFilePath>` is the path to Gelang source code
- `<outputFilePath>` is the path to the compiled code in GeAssembly

`-g` flag enables labels in the generated assembly

# Errors:
## Variables
- when trying to use a variable as an array
- when trying to use an array as a variable
- when an array with bigger lower bound than upper bound is declared
- when trying to access an array as constant number index outside of its bounds

## READ instruction
- when trying to read into a number

## WRITE instruction
- when variable being printed out is not initilized

## := instruction
- when variables used in expression are not initilized

## IF statments
- when variables used in condition are not initilized

## WHILE loops
- when variables used in condition are not initilized

## UNTIL loops
- when variables used in condition are not initilized

## FOR loops
- when an iterator of a FOR loop is being modified by := instruction
- when an iterator of a FOR loop is being modified by READ command
- when an iterator of a FOR loop is being modified by procedure call with it as an argument 
- when lower and upper bounds of a FOR loop are not initilized
- when a variable with the same id as the iterator already exists


# Warrnings:
## WHILE loops
- when an infinite WHILE loop is detected

## UNTIL loops
- when an infinite UNTIL loop is detected



# Optimilizations:
## Expressions
- when an experssion is known at compile time it will be automatically calculated
- when an expression can be reduced due to algebraic identeties it will be simplified

## IF statments
- when the statment's logical value is known at compile time due to it's being a tautology or a contradiction it is not checked and the appropriate section of the instruction is executed. However its label is still generated

## WHILE loops
- when the condition is a contradiction the loop is not compiled. However its label is still generated

## UNTIL loops
- when the condition is a tautlogy commands of the loop are inlined and the condition is not checked

## FOR loops
- when the loop is degenerate, that is lower > upper bound (or lower < upper bound in the case of DOWN vewrsion) the loop is not compiled. However its label is still generated