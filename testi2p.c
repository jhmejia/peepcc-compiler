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

    printf("\n\n=================================================\n");
    printf("Infix to Postfix Testing! \n");
    printf("=================================================\n");

    strncpy(infix, "j * j + 10 / 5 * ( 5 - 4 )",  256) ;

    printf("Our infix is: %s\n", infix);

    convertToPostfix(infix, postfix);
    
    printf("Our changed postfix is: %s\n", postfix);
    

    printf("\n");
    printf("\n");

    strncpy(postfix, "10 10 + 5 / 4 *" ,  256) ;
    printf("Now we will evaluate the expression %s:\n", postfix);


    int result = evaluatePostfixExpression(postfix);
    printf("The result is %d", result);   
   printf("\n");
}