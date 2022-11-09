#include <stdio.h>
#include "stack.h"
#include <stdlib.h>


char pop(StackNodePtr *topPtr)
{
    StackNodePtr top = *topPtr;
    char data = top->data;
    *topPtr = (*topPtr)->nextPtr;
    free(top);
    
    return data;
}
char stackTop(StackNodePtr topPtr)
{
    if (isEmpty(topPtr) == 0 ){
        return topPtr->data;
    } else {
        return '\0';
    }

}
void push(StackNodePtr *topPtr, char value)
{
    StackNodePtr toPush = malloc(sizeof(StackNodePtr));
    if (toPush != NULL) {
        toPush->data = value;
        toPush->nextPtr = *topPtr;
        *topPtr =toPush;
    }

}
int isEmpty(StackNodePtr topPtr)
{
    return (topPtr == NULL);
}
void printStack(StackNodePtr topPtr)
{
    while(topPtr != NULL) {
        printf("%c\n", topPtr->data);
        topPtr=topPtr->nextPtr;
    }
    printf("NULL \n");

}


