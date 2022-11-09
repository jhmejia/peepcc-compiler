struct stackNode {
    char data;
    struct stackNode *nextPtr;
};
typedef struct stackNode StackNode;
typedef StackNode *StackNodePtr;

char pop(StackNodePtr *topPtr);
char stackTop(StackNodePtr topPtr);
void push(StackNodePtr *topPtr, char value);
int isEmpty(StackNodePtr topPtr);
void printStack(StackNodePtr topPtr);