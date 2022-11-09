//=============================================
// Includes and Defines
//=============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "infix-to-postfix.h"
#include "stack.h"

//=============================================
// Functions
//=============================================

// int main(int argc, char *argv[]) {
//     // initialize an empty infix array 
//     char infix[256]; 
//     // initialize an empty postflix array
//     char postfix[256]; 
//     scanf("%[^\n]%*c", infix);
//     //printf("%s\n", infix);
//     convertToPostfix(infix, postfix);
//     int i = 0;
//     while (isOperator(postfix[i]) == 1 || isdigit(postfix[i]) != 0 || postfix[i] == ' ' || isalpha(postfix[i])){
//         printf("%c ", postfix[i]);
//         i++;
//     }
//     printf("\n");
//     int result = evaluatePostfixExpression(postfix);
//     printf("The result is %d", result);   
//     printf("\n");
// }

// Convert the infix expression to postfix notation.
/*void convertToPostfix(char infix[], char postfix[]){
    // local variables
    int iindex=0;
    int pindex=0;
    StackNodePtr ptrnode = NULL; 

    push(&ptrnode, '(');
    strncat(infix, ")", sizeof(&infix) + 1);

    char* token = strtok(infix, " ");
    //char space[2] = " ";

    // iterates through stack while it is not empty
    while(isEmpty(ptrnode) != 1) {
        //char c = infix[iindex]; 

        // checks to see if the current infix char is a '('
        if (token[0] == '(') { //infix[iindex] changed from c
            // pushes the char onto the stack
            push(&ptrnode, token[0]);
            iindex++;
            token = strtok(NULL, " ");
        }

        // checks to see if the current infix char is an operator
        else if (isOperator(token[0]) == 1) { //infix[iindex] changed from c
            // checks if the operator has a higher precedence than current infix char
            while((isOperator(stackTop(ptrnode)) == 1) && ((precedence(token[0], stackTop(ptrnode)) != -1))) {
                // inserta operator in postfix expression while incrementing postfix index
                postfix[pindex] = pop(&ptrnode);
                pindex++;
                postfix[pindex] = ' ';
                pindex++; 
            }
            // pushes operator onto the stack
            push(&ptrnode, token[0]);
            iindex++;
            token = strtok(NULL, " ");
        }

        // checks to see if the current infix char is a ')'
        else if (token[0] == ')') {
            iindex++;
            // iterates through stack for the expression between parantheses
            while(stackTop(ptrnode) != '(') {
                // adds chars in the stack to the postfix expression while incrementing postfix index
                postfix[pindex] = pop(&ptrnode);
                pindex++;
                postfix[pindex] = ' ';
                pindex++; 
            }
            // pops off the '(' from the stack
            pop(&ptrnode);
            token = strtok(NULL, " ");
        }

        // checks to see if the char in the stack is a digit
        else {
            // assigns the infix index to the postfix index while incrementing the postfix index
            int i;
            for (i = 0; i<strlen(token); i++){
                postfix[pindex] = token[i];
                pindex++;
                postfix[pindex] = ' ';
                pindex++; 
            }
            token = strtok(NULL, " ");
        }
    }
}*/
void convertToPostfix(char infix[], char postfix[]){
    // local variables
    // int iindex=0;
    int pindex=0;
    StackNodePtr ptrnode = NULL; 

    push(&ptrnode, '(');
    strncat(infix, " )", sizeof(&infix) + 1);

    //printf("%s", infix);
    
    char* token = strtok(infix, " ");

    // iterates through stack while it is not empty
    while(isEmpty(ptrnode) != 1) {
    // while (token != NULL) {
        //printf("%c", token[0]);
        //char c = infix[iindex]; 

        // checks to see if the current infix char is a '('
        if (token[0] == '(') { //infix[iindex] changed from c
            // pushes the char onto the stack
            push(&ptrnode, token[0]);
            // iindex++;
        }

        // checks to see if the current infix char is an operator
        else if (isOperator(token[0]) == 1) { //infix[iindex] changed from c
            // checks if the operator has a higher precedence than current infix char
            while((isOperator(stackTop(ptrnode)) == 1) && ((precedence(token[0], stackTop(ptrnode)) != -1))) {
                // inserta operator in postfix expression while incrementing postfix index
                postfix[pindex] = pop(&ptrnode);
                pindex++;
                postfix[pindex] = ' '; 
                pindex++;
            }
            // pushes operator onto the stack
            push(&ptrnode, token[0]);
            // iindex++;
        }

        // checks to see if the current infix char is a ')'
        else if(token[0] == ')') {
            // iindex++;
            // iterates through stack for the expression between parantheses
            while(stackTop(ptrnode) != '(') {
                // adds chars in the stack to the postfix expression while incrementing postfix index
                postfix[pindex] = pop(&ptrnode);
                pindex++;
                postfix[pindex] = ' '; 
                pindex++;
            }
            // pops off the '(' from the stack
            pop(&ptrnode);
        }

        // checks to see if the char in the stack is a digit
        else {
            if (isalpha(token[0])){
                // assigns the infix index to the postfix index while incrementing the postfix index
                postfix[pindex] = token[0];
                // iindex++;
                pindex++; 
                postfix[pindex] = ' '; 
                pindex++;
            }
            else {
                for (int i = 0; i < strlen(token); i++){
                    //printf("\n%s", "I've ENTERED");
                    //printf("%s\n", token); 
                    // assigns the infix index to the postfix index while incrementing the postfix index
                    postfix[pindex] = token[i];
                    // iindex++;
                    pindex++; 
                }
                postfix[pindex] = ' '; 
                pindex++;
            }
        }
        token = strtok(NULL, " ");
    }
}

// Determine whether `c` is an operator.
int isOperator(char c){
    // returns 1 (TRUE) if the char is an operater
    if (c=='*' || c=='/' || c=='+' || c=='-') {
        return 1;
    }
    // returns 0 (FALSE) otherwise
    return 0;
}

// Determine whether the precedence of `operator1` is less than, equal to, or greater than the precedence of `operator2`. 
// The function returns -1, 0 and 1, respectively.
int precedence(char operator1, char operator2){
    if (operator1 == '+' || operator1 == '-') {
        // If there are two operators that are +/- they have the same precendence thus returning 0
        if (operator2 == '+' || operator2 == '-') {
            return 0; 
        }
        // Since +/- has a greater precendence than * or /, returns 1
        else {
            return 1; 
        }
    }
    if (operator1 == '*' || operator1 == '/') {
        // Since */- has lesser precendence than +/-, it returns -1
        if (operator2 == '+' || operator2 == '-') {
            return -1;
        }
        // If the two operators are both * or - it returns 0 because they have the same precendence
        else {
            return 0; 
        }
    }
    return 0; 
}

int evaluatePostfixExpression(char *expr){
    // local variables
    int current;
    StackNodePtr ptrnode = NULL;

    // adding NULL char at end of expression to note when it is the end of the expression
    strncat(expr, "\0", sizeof(&expr)+1);
    //char* token = strtok(expr, " ");

    // checking that the current char of the expression has not reached the null character
    for(current = 0; expr[current] != '\0'; current++){
        // checks if current char of expression is a digit
        if(isdigit(expr[current]) || isalpha(current)){
            // pushes the digit onto the stack
            push(&ptrnode, expr[current]);
        }
        // checks to see if current char of expression is an operator
        if(isOperator(expr[current])){
            // gets top value on stack
            char c = pop(&ptrnode);

            // helper variables
            int h1;
            int h2;

            // checks if top value on stack is a '('
            if(c == '('){
                // creates char array to hold expression in parantheses
                char helperCharArr[256];
                // pops next value on stack
                c = pop(&ptrnode);
                // adds expression to helper char array and pops next value while it doesnt reach closing parantheses
                for (int ii = 0; c != ')'; ii++){
                    helperCharArr[ii] = c;
                    c = pop(&ptrnode);
                }
                // converts to int and assigns to helper int
                h1 = atoi(helperCharArr);
                c = pop(&ptrnode);
                
                // checks if there is an inner '(' within parantheses expression
                if(c == '('){
                    // creates char array to holder inner parantheses expression
                    char helperCharArr2[256];
                    // pops next value on stack
                    c = pop(&ptrnode);
                    // adds expression to helper char array and pops next value while it doesnt reach closing parantheses
                    for (int jj = 0; c != ')'; jj++){
                        helperCharArr2[jj] = c;
                        c = pop(&ptrnode);
                    }
                    // converts to int and assigns to helper int
                    h2 = atoi(helperCharArr2);
                }
                else {
                    h2 = c - '0';
                }
            }

            // if top value of stack is not an operator
            else {
                // assigns helper int to top value on stack
                h1 = c - '0';
                // pops next value
                c = pop(&ptrnode);

                // checks if next value is '('
                // checks if there is an inner '(' within parantheses expression
                if(c == '('){
                    // creates char array to holder inner parantheses expression
                    char helperCharArr3[256];
                    // pops next value on stack
                    c = pop(&ptrnode);
                    // adds expression to helper char array and pops next value while it doesnt reach closing parantheses
                    for (int kk = 0; c != ')'; kk++){
                        helperCharArr3[kk] = c;
                        c = pop(&ptrnode);
                    }
                    // converts to int and assigns to helper int
                    h2 = atoi(helperCharArr3);
                }
                else {
                    h2 = c - '0';
                }
            }

            char resultExpr[256];
            sprintf(resultExpr, "%d", calculate(h1, h2, expr[current]));

            // pushes beginning parantheses on stack if the expression is more than one value
            if(strlen(resultExpr) > 1){
                push(&ptrnode, ')');
            }

            // pushes values onto stack in reverse order of the resultExpr array
            for(int ll = strlen(resultExpr) - 1; ll > -1; ll--){
                push(&ptrnode, resultExpr[ll]);
            }
            
            // pushes closing parantheses on stack if the expression is more than one value
            if(strlen(resultExpr) > 1){
                push(&ptrnode, '(');
            }
        }
    }

    // empty char array for final evaluated expression
    char finalEval[256];
    int n = 0;
    // checks that stack is not empty
    while(!isEmpty(ptrnode)){
        // gets top value on stack
        char cEval = pop(&ptrnode);
        // adds the evaluated char to the final expression if it is a digit
        if(isdigit(cEval)) {
            finalEval[n] = cEval;
            n++;
        }
    }
    // converts char array into an int that is equal to final evaluated expression
    return atoi(finalEval);
}



// Evaluate the expression `op1 operator op2`.
int calculate(int op1, int op2, char operator){
    // local variable
    int result;

    // switch case used to evaulate which operator is being used to calculate the expression
    switch(operator)
            {
            case '*':
                result = op2 * op1;
                break;

            case '/':
                result = op2 / op1;
                break;

            case '+':
                result = op2 + op1;
                break;

            case '-':
                result = op2 - op1;
                break;
            default:
                printf("NOT A VALID OPERATOR");
                return 0;
            }
    return result;
}