# Infix to Postfix Conversion and Evaluation
From Deitel "C How to Program"

## Introduction
Stacks are used by compilers to help in the process of evaluating expressions and generating machine-language code. In this and the next exercise, we investigate how compilers evaluate arithmetic expressions consisting only of constants, operators and parentheses.

Humans generally write expressions like `3 + 4` and `7 / 9` in which the operator (`+` or `/` here) is written between its operands—this is called infix notation. Computers “prefer” postfix notation in which the operator is written to the right of its two operands. The preceding infix expressions would appear in postfix notation as `3 4 +` and `7 9 /`, respectively.

To evaluate a complex infix expression, some compilers first convert the expression to postfix notation, and then evaluate the postfix version. Each of these algorithms requires only a single left-to-right pass of the expression. Each algorithm uses a stack in support of its operation, and in each the stack is used for a different purpose.

## Infix to Postfix Conversion Program
Write a program that converts an ordinary infix arithmetic expression (assume a valid expression is entered) with single-digit integers such as

`(6 + 2) * 5 - 8 / 4` 

to a postfix expression. The postfix version of the preceding infix expression is

`6 2 + 5 * 8 4 / -`

The program should read the expression into character array (aka a c-string) `infix` and use the stack functions implemented in this chapter to help create the postfix expression in character array `postfix`. The algorithm for creating a postfix expression is as follows:

1. Push a left parenthesis `'('` onto the stack.
2. Append a right parenthesis `')'` to the end of `infix`.
3. While the stack is not empty, read `infix` from left to right and do the following:
    - If the current character in `infix` is a digit, copy it to the next element of `postfix`.
    - If the current character in `infix` is a left parenthesis, push it onto the stack.
    - If the current character in `infix` is an operator,
        - Pop operators (if there are any) at the top of the stack while they have equal or higher precedence than the current operator, and insert the popped operators in `postfix`.
        - Push the current character in `infix` onto the stack.
    - If the current character in `infix` is a right parenthesis
        - Pop operators from the top of the stack and insert them in `postfix` until a left parenthesis is at the top of the stack.
        - Pop (and discard) the left parenthesis from the stack.

The following arithmetic operations are allowed in an expression:
1. \+ addition
2. \- subtraction
3. \* multiplication
4. / division

The stack should be maintained with the following declarations:
```c
struct stackNode {
    char data;
    struct stackNode *nextPtr;
};
typedef struct stackNode StackNode;
typedef StackNode *StackNodePtr;
```
## Infix to Postfix Conversion Program
Write a program that will convert an infix expression to postfix.  The program should consist of a `main()` function and eight other functions with the following function headers:

Convert the infix expression to postfix notation.
```c
void convertToPostfix(char infix[], char postfix[])
```
Determine whether `c` is an operator.
```c
int isOperator(char c)
```
Determine whether the precedence of `operator1` is less than, equal to, or greater than the precedence of `operator2`. The function returns -1, 0 and 1, respectively.
```c
int precedence(char operator1, char operator2)
```
Push a value on the stack.
```c
void push(StackNodePtr *topPtr, char value)
```
Pop a value off the stack.
```c
char pop(StackNodePtr *topPtr)
```
Return the top value of the stack without popping the stack.
```c
char stackTop(StackNodePtr topPtr)
```
Determine whether the stack is empty.
```c
int isEmpty(StackNodePtr topPtr)
```
Print the stack.
```c
void printStack(StackNodePtr topPtr)
```

## Postfix Evaluation
Write a program that evaluates a postfix expression (assume it’s valid)
such as

`6 2 + 5 * 8 4 / -`

The program should read a postfix expression consisting of single digits and operators into a char-acter array. Using the stack functions implemented earlier in this chapter, the program should scan the expression and evaluate it. The algorithm is as follows:
1. Append the `NULL` character (`'\0'`) to the end of the postfix expression. When the `NULL` character is encountered, no further processing is necessary.
2. While `'\0'` has not been encountered, read the expression from left to right.
    - If the current character is a digit,
      - Push its integer value onto the stack (the integer value of a digit character is its value in the computer’s character set minus the value of `'0'` in the computer’s character set).
    - Otherwise, if the current character is an operator,
      - Pop the two top elements of the stack into variables `x` and `y`.
      - Calculate `y operator x`.
      - Push the result of the calculation onto the stack.
3. When the `NULL` character is encountered in the expression, pop the top value of the stack.  This is the result of the postfix expression.

[Note: In 2) above, if the operator is `'/'`, the top of the stack is `2`, and the next element in the stack is `8`, then pop `2` into `x`, pop `8` into `y`, evaluate `8 / 2`, and push the result, `4`, back onto the stack. This note also applies to the other binary operators.]

The program should consist of main and six other functions with the following function headers:

Evaluate the postfix expression.
```c
int evaluatePostfixExpression(char *expr)
```
Evaluate the expression `op1 operator op2`.
```c
int calculate(int op1, int op2, char operator)
```