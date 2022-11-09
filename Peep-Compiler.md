# The Peep Compiler For Hatchling
Based on a design by Deitel.
## Specifics
1. This project is worth 200 points.
2. It is expected that you will work equally with your partner.
3. You must use the C programming language.
4. You must use procedures and functions.
5. You must document your code.
6. You must create and use a Makefile to build your project.  It should also have a `clean` target to remove the object files and executable.
7. Push changes to your GitHub repo often.  Use `git pull` to sync changes between computers.
8. Testing, testing, testing.  Your compiler will be tested on `babbage.cs.tcu.edu`.
9. Name of your executable must be `peepcc`.  The name of your source code file is `peepcc.c`
10. Must include `stack.h` and `stack.c`.
11. Must include `infix-to-postfix.h` and `infix-to-postfix.c`.
12. Must include a test program (`test-i2p`) for infix-to postfix conversion and evaluation.


## Introduction
You have been introduced to the Hatchling Machine Language (HML) and created the Hatching computer simulator to execute programs written in HML. Now, we will build a compiler that converts programs written in a high-level programming language to HML. This ties together the entire programming process. We will write programs in this new high-level language, compile the programs on the compiler we build, and run the programs on the simulator we built.

## The Peep Programming Language
### Commands
Before we begin building the compiler, we discuss a simple, yet powerful, high-level language similar to early versions of the popular language BASIC. We call the language Peep. Every Peep statement consists of a line number and a Peep instruction. Line numbers must appear in ascending order. Each instruction begins with one of the following Peep commands: `rem`, `input`, `let`, `print`, `goto`, `if...goto`, or `end` 

| **Command** | **Example Statement**     | **Description**                                                                                                                                                    |
|-------------|---------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `rem`       | `50 rem this is a remark` | Any text following the command **rem** is for documentation purposes only and is ignored by the compiler.                                                          |
| `input`     | `30 input x`              | Display a question mark prompt to prompt the user to enter an integer.  Read that integer from the keyboard and store the integer in **x**.                        |
| `let`       | `80 let u = 4 * (j - 56)` | Assign **u** the value of **4 * (j - 56)**.  Note that an arbitrarily complex expression can appear to the right of the equal sign.                                |
| `print`     | `10 print w`              | Display the value of **w**.                                                                                                                                        |
| `goto`      | `70 goto 45`              | Transfer control to line 45.                                                                                                                                       |
| `if/goto`   | `35 if y == z goto 80`    | Compare **y** and **z** for equality and transfer  program control to line **80** if the condition is true; otherwise, continue execution with the next statement. |
| `end`       | `99 end`                  | Terminate program execution.                                                                                                                                       |

All commands except end can be used repeatedly. Peep evaluates only integer expressions using the `+`, `-`, `*`, `/`, and `%` operators. These operators have the same precedence as in C. Parentheses can be used to change the order of evaluation of an expression.


### Characters, Variables, and Integers
Our Peep compiler recognizes only lowercase letters. All characters in a Peep file should be lowercase (uppercase letters result in a syntax error unless they appear in a rem statement in which case they are ignored). A variable name is a single letter. Peep does not allow descriptive variable names, so variables should be explained in remarks to indicate their use in the program. Peep uses only integer variables. Peep does not have variable declarations—merely mentioning a variable name in a program causes the variable to be declared and initialized to zero automatically. The syntax of Peep does not allow string manipulation (reading a string, writing a string, comparing strings, etc.). If a string is encountered in a Peep program (after a command other than rem), the compiler generates a syntax error. Our compiler will assume that Peep programs are entered correctly. 

### Control Flow and Relational Operaators
Peep uses the conditional `if...goto` statement and the unconditional `goto` statement to alter the flow of control during program execution. If the condition in the `if...goto` statement is true, control is transferred to a specific line of the program. The following relational and equality operators are valid in an `if...goto` statement: `<`, `>`, `<=`, `>=`, `==` or `!=`. The precedence of these operators is the same as in C.

## Example 1
The first program reads two integers from the keyboard, stores the values in variables `a` and `b`, and computes and prints their sum (stored in variable `c`).

```basic
10 rem determine and print the sum of two integers
15 rem
20 rem input the two integers
30 input a
40 input b
50 rem add integers and store result in c
60 let c = a + b
65 rem
70 rem print the result
80 print c
90 rem terminate program execution
99 end
```

### Example 2
Here is another example that determines and prints the larger of two integers. The integers are input from the keyboard and stored in `s` and `t`. The `if...goto` statement tests the condition `s >= t`. If the condition is true, control is transferred to line 90 and `s` is output; otherwise, `t` is output and control is transferred to the `end` statement in line 99 where the program terminates.

```basic
10 rem determin the larger of two integers
20 input s
30 input t
32 rem
35 rem test if s >= t
40 if s >= t goto 90
45 rem
50 rem t is greater than s, so print t
60 print t
70 goto 99
75 rem
80 rem s is greater than or equal to t, so print s
90 print s
99 end
```

### Example 3: Looping and Repetition Structures
Peep does not provide a repetition structure (such as C’s `for`, `while` or `do...while`). However, Peep can simulate each of C’s repetition structures using the `if...goto` and `goto` statements. Here is an example of a sentinel-controlled loop to calculate the squares of several integers. Each integer is input from the keyboard and stored in variable `j`. If the value entered is the sentinel **-9999**, control is transferred to line 99 where the program terminates. Otherwise, `k` is assigned the square of `j`, `k` is output to the screen and control is passed to line 20 where the next integer is input.

```basic
10 rem calculate the squares of several integers
20 input j
23 rem
25 rem test for sentinel value
30 if j == -9999 goto 99
33 rem
35 rem calculate square of j and assign result to k
40 let k = j * j
50 print k
53 rem
55 rem loop to get next j
60 goto 20
99 end
```

## Building the Peep Compiler
### Introduction
Now that the Peep language has been presented, we discuss how to build our Peep compiler. First, we consider the process by which a Peep program is converted to HML and executed by the Hatching simulator. A file containing a Peep program is read by the compiler and converted to HML code. The HML code is output to a file on disk, in which HML instructions appear one per line. The HML file is then loaded into the Hatching simulator, and the results are sent to a to the screen (standard output). 


### Compiler Design
The compiler performs two passes of the Peep program to convert it to HML. The first pass constructs a ***symbol table*** in which every line number, variable name and constant of the Peep program is stored with its type and corresponding location in the final HML code (the symbol table is discussed in detail below). The first pass also produces the corresponding HML instruction(s) for each Peep statement. As we will see, if the Peep program contains statements that transfer control to a line later in the program, the first pass results in an HML program containing some incomplete instructions. The second pass of the compiler locates and completes the unfinished instructions, and outputs the HML program to a file.


### First Pass
### Tokenizing and the Symbol Table
The compiler begins by reading one statement of the Peep program into memory. The line must be separated into its individual tokens (i.e., "pieces" of a statement) for processing and compilation (standard library function `strtok()` can be used to facilitate this task). Recall that every statement begins with a line number followed by a command. As the compiler breaks a statement into tokens, if the token is a line number, a variable, or a constant, it’s placed in the symbol table. A line number is placed in the symbol table only if it’s the first token in a statement. The `symbolTable` is an array of `tableEntry` structures representing each symbol in the program. There is no restriction on the number of symbols that can appear in the program. Therefore, the `symbolTable` for a particular program could be large. Make the `symbolTable` a 256-element array for now. You can increase or decrease its size once the program is working.

The `tableEntry` structure definition is as follows:
```c
struct tableEntry { 
    int symbol;
    char type; /* 'C', 'L' or 'V' */
    int location; /* 00 to FF */ 
};
```

Each `tableEntry` structure contains three members. Member `symbol` is an integer containing the ASCII representation of a variable (remember that variable names are single characters), a line number, or a constant. Member `type` is one of the following characters indicating the symbol’s type: `C` for constant, `L` for line number, or `V` for variable. Member `location` contains the Hatching memory location (`0x00` to `0xFF`) to which the symbol refers. Hatching memory is an array of 256 `short int`egers in which HML instructions and data are stored. For a line number, the `location` is the element in the Hatching memory array at which the HML instructions for the Peep statement begin. For a variable or constant, the location is the element in the Hatching memory array in which the variable or constant is stored. Variables and constants are allocated from the ***end*** of Hatching’s memory backwards. The first variable or constant is stored in location at `0xFF`, the next in location at `0xFE`, etc.

The symbol table plays an integral part in converting Peep programs to HML. An HML instruction is a four-digit hexadecimal integer that comprises two parts—the operation code and the operand. The operation code is determined by commands in Peep. For example, the simple command `input` corresponds to HML operation code `0x50` (READ), and the Peep com- mand `print` corresponds to HML operation code `0x51` (WRTE). The operand is a memory location containing the data on which the operation code performs its task (e.g., operation code `0x50` reads a value from the keyboard and stores it in the memory location specified by the operand). The compiler searches `symbolTable` to determine the Hatching memory location for each symbol so the corresponding location can be used to complete the HML instructions.

The compilation of each Peep statement is based on its command. For example, after the line number in a `rem` statement is inserted in the symbol table, the remainder of the statement is ignored by the compiler, because a remark is for documentation purposes only. The `input`, `print`, `goto`, and `end` statements correspond to the HML `READ`, `WRTE`, `B` (to a specific location), and `HALT` instructions. Statements containing these Peep commands are converted directly to HML [Note: A `goto` statement may contain an unresolved reference if the specified line number refers to a statement further into the Peep program file; this is sometimes called a forward reference.]

When a `goto` statement is compiled with an unresolved reference, the HML instruction must be flagged to indicate that the second pass of the compiler must complete the instruction. The `flags` are stored in 256-element array `flags` of type `int` in which each element is initialized to `-1`. If the memory location to which a line number in the Peep program refers is not yet known (i.e., it’s not in the symbol table), the line number is stored in array flags in the element with the same subscript as the incomplete instruction. The operand of the incomplete instruction is set to `0x00` temporarily. For example, an unconditional branch instruction (making a forward reference) is left as `0x3000` until the second pass of the compiler. The second pass of the compiler will be described shortly.

Compilation of `if...goto` and `let` statements is more complicated than other statements—they are the only statements that produce more than one HML instruction. For an `if...goto` `statement`, the compiler produces code to test the condition and to branch to another line if necessary. The result of the branch could be an unresolved reference. Each of the relational and equality operators can be simulated using HML’s branch zero and branch negative instructions (or possibly a combination of both).

For a `let` statement, the compiler produces code to evaluate an arbitrarily complex arithmetic expression consisting of integer variables and/or constants. Expressions should separate each operand and operator with spaces. 

### Infix to Postfix Conversion
Use the infix-to-postfix conversion algorithm and the postfix evaluation algorithm used by compilers to evaluate expressions. ***Before proceeding with your compiler, you will need to complete this task.*** When a compiler encounters an expression, it converts the expression from infix notation to postfix notation, then evaluates the postfix expression.

How is it that the compiler produces the machine language to evaluate an expression containing variables? The postfix evaluation algorithm contains a **"hook"** that allows our compiler to generate HML instructions rather than actually evaluating the expression. To enable this "hook" in the compiler, the postfix evaluation algorithm must be modified to search the symbol table for each symbol it encounters (and possibly insert it), determine the symbol’s corresponding memory location, and push the memory location on the stack instead of the symbol. When an operator is encountered in the postfix expression, the two memory locations at the top of the stack are popped and machine language for effecting the operation is produced using the memory locations as operands. The result of each subexpression is stored in a temporary location in memory and pushed back onto the stack so the evaluation of the postfix expression can continue. When postfix evaluation is complete, the memory location containing the result is the only location left on the stack. This is popped and HML instructions are generated to assign the result to the variable at the left of the let statement.

### Second Pass
The second pass of the compiler performs two tasks: resolve any unresolved references and output the HML code to a file. Resolution of references occurs as follows:
1. Search the `flags` array for an unresolved reference (i.e., an element with a value other than -1).
2. Locate the structure in array `symbolTable` containing the symbol stored in the `flags` array (be sure that the type of the symbol is `'L'` for line number).
3. Insert the memory location from structure member location into the instruction with the unresolved reference (remember that an instruction containing an unresolved reference has operand `0x00`).
4. Repeat Steps 1–3 until the end of the flags array is reached.

After the resolution process is complete, the entire array containing the HML code is output to a disk file with one HML instruction per line. This file can be read by the Hatching for execution.

## A Complete Example
The following example illustrates a complete conversion of a Peep program to HML as it will be performed by the Peep compiler. Consider a Peep program that inputs an integer and sums the values from 1 to that integer. The program and the HML instructions produced by the first pass are illustrated below. The symbol table constructed by the first pass is also shown below.

| **Simple Program**      | **HML Location<br>and Instruction**                           | **Description**                                                                                                                                                      |
|-------------------------|---------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `5 rem sum 1 to x`      | none                                                          | `rem` ignored                                                                                                                                                        |
| `10 input x`            | `00 50FF`                                                     | read `x` into location `0xFF`                                                                                                                                        |
| `15 rem check y == x`   | none                                                          | `rem` ignored                                                                                                                                                        |
| `20 if y == x goto 60`  | `01 40FE`<br>`02 11FF`<br>`03 3300`                           | load `y`(`0xFE`) into accumulator<br>sub `x`(`0xFF`) from accumulator<br>branch zero to unresolved location                                                          |
| `25 rem increment y`    | none                                                          | `rem` ignored                                                                                                                                                        |
| `30 let y = y + 1`      | `04 40FE`<br>`05 10FD`<br>`06 41FC`<br>`07 40FC`<br>`08 41FE` | load `y` into accumulator<br>add `1`(`0xFD`) to accumulator<br>store in temporary location `0xFC`<br>load from temporary location `0xFC`<br>store accumulator in `y` |
| `35 rem add y to total` | none                                                          | `rem` ignored                                                                                                                                                        |
| `40 let t = t + y`      | `09 40FB`<br>`0A 10FE`<br>`0B 41FA`<br>`0C 40FA`<br>`0D 41FB` | load `t`(`0xFB`) into accumulator<br>add `y` to accumulator<br>store in temporary location `0xFA`<br>load from temporary location `0xFA`<br>store accumulator in `t` |
| `45 rem loop y`         | none                                                          | `rem` ignored                                                                                                                                                        |
| `50 goto 20`            | `0E 3001`                                                     | branch to location `0x01`                                                                                                                                            |
| `55 rem output result`  | none                                                          | `rem` ignored                                                                                                                                                        |
| `60 print t`            | `0F 51FB`                                                     | output `t` to screen                                                                                                                                                 |
| `99 end`                | `10 FF00`                                                     | terminate execution                                                                                                                                                  |


| **Symbol** | **Type** | **Location** |
|:----------:|:--------:|:------------:|
|      5     |     L    |     0x00     |
|     10     |     L    |     0x00     |
|     `x`    |     V    |     0xFF     |
|     15     |     L    |     0x01     |
|     20     |     L    |     0x01     |
|     `y`    |     V    |     0xFE     |
|     25     |     L    |     0x04     |
|     30     |     L    |     0x04     |
|      1     |     C    |     0xFD     |
|     35     |     L    |     0x09     |
|     40     |     L    |     0x09     |
|     `t`    |     V    |     0xFB     |
|     45     |     L    |     0x0E     |
|     50     |     L    |     0x0E     |
|     55     |     L    |     0x0F     |
|     60     |     L    |     0x0F     |
|     99     |     L    |     0x10     |

Most Peep statements convert directly to single HML instructions. The exceptions in this program are `rem`arks, the `if...goto` statement in line 20, and the `let` statements. Remarks do not translate into machine language. However, the line number for a remark is placed in the symbol table in case the line number is referenced in a `goto` statement or an `if...goto` statement. Line 20 of the program specifies that if the condition `y == x` is true, program control is transferred to line 60. Because line 60 appears later in the program, the first pass of the compiler has not as yet placed 60 in the symbol table (line numbers are placed in the symbol table only when they appear as the first token in a statement). Therefore, it’s not possible at this time to determine the operand of the HML branch zero instruction at location 03 in the array of HML instructions. The compiler places 60 in location 03 of the flags array to indicate that the second pass completes this instruction.

We must keep track of the next instruction location in the HML array because there is not a one-to-one correspondence between Peep statements and HML instructions. For example, the `if...goto` statement of line 20 compiles into three HML instructions. Each time an instruction is produced, we must increment the instruction counter to the next location in the HML array. The size of Hatching’s memory could present a problem for Peep programs with many statements, variables and constants. It’s conceivable that the compiler will run out of memory. To test for this case, your program should contain a data counter to keep track of the location at which the next variable or constant will be stored in the HML array. If the value of the instruction counter is larger than the value of the data counter, the HML array is full. In this case, the compilation process should terminate and the compiler should print an error message indicating that it ran out of memory during compilation.

## Step-by-Step View of the Compilation Process

Let’s now walk through the compilation process for the Peep program shown above. The compiler reads the first line of the program
```basic
5 rem sum 1 to x
```
into memory. The first token in the statement (the line number) is determined using `strtok()`. The token returned by `strtok()` is converted to an integer using `atoi()`, so the symbol 5 can be located in the symbol table. If the symbol is not found, it’s inserted in the symbol table. Since we’re at the beginning of the program and this is the first line, no symbols are in the table yet. So, `5` is inserted into the symbol table as type L (line number) and assigned the first location in HML array (`0x00`). Although this line is a remark, a space in the symbol table is allocated for the line number (in case it’s referenced by a `goto` or an `if...goto`). No HML instruction is generated for a `rem` statement, so the instruction counter is not incremented.

The statement
```basic
10 input x
``` 
is tokenized next.  The line number 10 is placed in the symbol table as type L and assigned the first location in the HML array (00 because a remark began the program, so the instruction counter is currently `0x00`). The command input indicates that the next token is a variable (only a variable can appear in an input statement). Because `input` corresponds directly to an HML operation code, the compiler simply has to determine the location of `x` in the HML array. Symbol `x` is not found in the symbol table. So, it’s inserted into the symbol table as the ASCII representation of `x`, given type `V`, and assigned location `0xFF` in the HML array (data storage begins at `0xFF` and is allocated backwards). HML code can now be generated for this statement. Operation code `0x50` (the HML READ operation code) is multiplied by 256, and the location of `x` (as determined in the symbol table) is added to complete the instruction. The instruction is then stored in the HML array at location  `0x00`. The instruction counter is incremented by `1` because a single HML instruction was produced.

The statement
```basic
15 rem check y == x
```
is tokenized next. The symbol table is searched for line number `15` (which is not found). The line number is inserted as type `L` and assigned the next location in the array, `0x01` (remember that rem statements do not produce code, so the instruction counter is not incremented).

The statement
```basic
20 if y == x goto 60
```
is tokenized next. Line number 20 is inserted in the symbol table and given type `L` with the next location in the HML array `0x01`. The command if indicates that a condition is to be evaluated. The variable `y` is not found in the symbol table, so it’s inserted and given the type `V` and the HML location `0xFE`. Next, HML instructions are generated to evaluate the condition. Since there is no direct equivalent in HML for the `if...goto`, it must be simulated by performing a calculation using `x` and `y` and branching based on the result. If `y` is equal to `x`,the result of subtracting `x` from `y` is zero, so the branch zero instruction can be used with the result of the calculation to simulate the `if...goto` statement. The first step requires that `y` be loaded (from HML location `0xFE`) into the accumulator. This produces the instruction `01 40FE`. Next, `x` is subtracted from the accumulator. This produces the instruction `02 11FF`. The value in the accumulator may be zero, positive or negative. Since the operator is `==`, we want to branch zero. First, the symbol table is searched for the branch location (`60` in this case), which is not found. So, `60` is placed in the flags array at location `03`, and the instruction `03 3300` is generated (we cannot add the branch location because we have not assigned a location to line `60` in the HML array yet). The instruction counter is incremented to `04`.

The compiler proceeds to the statement
```basic        
25 rem   increment y
```
The line number 25 is inserted in the symbol table as type L and assigned HML location `04`. The instruction counter is not incremented.

When the statement
```basic
30 let y = y + 1
```
is tokenized, the line number `30` is inserted in the symbol table as type `L` and assigned HML location 04. Command `let` indicates that the line is an assignment statement. First, all the symbols on the line are inserted in the symbol table (if they are not already there). The integer 1 is added to the symbol table as type `C` and assigned HML location `0xFD`. Next, the right side of the assignment is converted from infix to postfix notation. Then the postfix expression `(y 1 +)` is evaluated. Symbol `y` is located in the symbol table and its corresponding memory location is pushed onto the stack. Symbol `1` is also located in the symbol table, and its corresponding memory location is pushed onto the stack. When the operator `+` is encountered, the postfix evaluator pops the stack into the right operand of the operator and pops the stack again into the left operand of the operator, then produces the HML instructions
```text
04 40FE (load y) 
05 10FD (add 1)
```
The result of the expression is stored in a temporary location in memory (`0xFC`) with instruction `06 41FC` (store temporary)
and the temporary location is pushed on the stack. Now that the expression has been evaluated, the result must be stored in `y` (i.e., the variable on the left side of `=`). So, the temporary location is loaded into the accumulator and the accumulator is stored in `y` with the instructions
```text
07 40FC (load temporary) 
08 41FE (store y)
```
The reader will immediately notice that HML instructions appear to be redundant. We’ll discuss this issue shortly.

When the statement
```basic
35 rem add y to total
```
is tokenized, line number `35` is inserted in the symbol table as type `L` and assigned location `09`.

The statement
```basic 
40 let t = t + y
```
is similar to line `30`. The variable `t` is inserted in the symbol table as type `V` and assigned HML location `95`. The instructions follow the same logic and format as line `30`, and the instructions 
```text
09 40FB 
0A 10FE 
0B 41FA
0C 40FA
0D 41FB 
```
are generated. The result of `t + y` is assigned to temporary location `0xFA` before being assigned to `t` (`0xFB`). The instructions in memory locations `0x0B` and `0x0C` appear to be redundant. Again, we’ll discuss this shortly.

The statement
```basic
45 rem loop y
```
is a remark, so line `45` is added to the symbol table as type `L` and assigned HML location `14`.

The statement
```basic
50 goto 20
```
transfers control to line `20`. Line number `50` is inserted in the symbol table as type `L` and assigned HML location `0x0E`. The equivalent of goto in HML is the unconditional branch (`0x30`) instruction thattransfers control to a specific HML location. The compiler searches the symbol table for line `20` and finds that it corresponds to HML location `01`. The operation code (`0x30`) is multiplied by `256` and location `01` is added to it to produce the instruction `14 3001`.

The statement
```basic
55 rem output result
```
is a remark, so line `55` is inserted in the symbol table as type `L` and assigned HML location `15`.

The statement
```basic
60 print t
```
is an output statement. Line number `60` is inserted in the symbol table as type `L` and assigned HML location `15`. The equivalent of `print` in HML is operation code `0x51` (WRTE). The location of `t` is determined from the symbol table and added to the result of the operation code multiplied by 256.

The statement
```basic
99 end
```
is the final line of the program. Line number `99` is stored in the symbol table as type `L` and assigned HML location `0x10`. The end command produces the HML instruction `FF00` (`0xFF` is `halt` in HML) which is written as the final instruction in the HML memory array.

This completes the first pass of the compiler. 

### Second Pass
We now consider the second pass. The `flags` array is searched for values other than `-1`. Location `03` contains `60`, so the compiler knows that instruction `03` is incomplete. The compiler completes the instruction by searching the symbol table for `60`, determining its location and adding the location to the incomplete instruction. In this case, the search determines that line `60` corresponds to HML location `0x0F`, so the completed instruction `03 330F` is produced replacing `03 3300`. The Peep program has now been compiled successfully.

## Compiler Program Design
To build the compiler, you’ll have to perform each of the following tasks: 

1. Modify the infix-to-postfix evaluation algorithm and program you developed earlierto process multi-digit integer operands and single letter variable name operands. [Hint: Standard library function `strtok()` can be used to locate each constant and variable in an expression, and constants can be converted from strings to integers using standard library function `atoi()`.] [Note: The data representation of the postfix expression must be altered to support variable names and integer constants.]

2. Modify the postfix evaluation algorithm to process multi-digit integer operands and variable name operands. Also, the algorithm should now implement the previously discussed “hook” so that HML instructions are produced rather than directly evaluating the expression. [Hint: Standard library function `strtok()` can be used to locate each constant and variable in an expression, and constants can be converted from strings to integers using standard library function atoi.] [Note: The data representation of the postfix expression must be altered to support variable names and integer constants.]

3. Build the compiler. Incorporate parts (1) and (2) for evaluating expressions in `let` statements. Your program should contain a function that performs the first pass of the compiler and a function that performs the second pass of the compiler. Both functions can call other functions to accomplish their tasks.









