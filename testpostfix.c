#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "infix-to-postfix.h"
#include "stack.h"

int main(int argc, char *argv[]) {
    // initialize an empty infix array 
    char infix[256]; 
    // initialize an empty postflix array
    char postfix[256]; 
    scanf("%s", infix);
    convertToPostfix(infix, postfix);
    int i = 0;
    while (isOperator(postfix[i]) == 1 || isdigit(postfix[i]) != 0 ){
        printf("%c ", postfix[i]);
        i++;
    }
    
    printf("\n");
    int result = evaluatePostfixExpression(postfix);
    printf("The result is %d", result);   
    printf("\n");
}