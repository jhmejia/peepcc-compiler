#include <stdio.h>
#include "stack.h"
#include <stdlib.h>






int main(int argc, char *argv[])
{
   
    printf("\n\n=================================================\n");
    printf("Stack Testing! \n");
    printf("=================================================\n");



    StackNodePtr item; 
    
    printf("We initialized a stack. Is it empty? %d\n", isEmpty(item));
    
    printf("We are going to push some items onto the stack... \n");
    push(&item, 'g');

    push(&item, 'l');


    push(&item, 'r');
    push(&item, 'h');

    printf("Popping the item: %c\n", pop(&item) );

    printf("Is it empty? %d\n", isEmpty(item));

    printf("View top of stack: %c\n********************\nPrinting whole stack: \n********************\n", stackTop(item));

    printStack(item);
    
}
