//PeepCC Alex Roa John Henry Mejia


//================================================
// includes and defines
//================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "infix-to-postfix.h"
#include "peepcc.h"
#include "stack.h"



int flagsGlobal[256];
int hmlGlobal[256];
tableEntry symbolTableGlobal[256];



/**
 * Pass 1
 * Tokenizes a symbol table, and runs appropriate commands. 
 * 
*/
void pass1(tableEntry symbolTable[], FILE *infp){
    // initialize local variables
    int flags[256];
    int ii;
    int pointerOther = 0;
    
    //Variables start at 0xFF and go down. 
    int variableAllocator = 0xFF;

    //This is the final HML array that we are going to output to memory. 
    int hml[256];

    // initialize all values in flags array to be -1
    for(ii = 0; ii < 256; ii++){
        flags[ii] = -1;
        hml[ii] = 0;
    }
    
    // read in file input as a line
    char instruction[256];
    
    ii = 0;
    // parses the tokens for the instructions
    while(fgets(instruction, 256, infp) != NULL){
        // Gets rid of next line from file input
        instruction[strcspn(instruction, "\n")] = '\0';
        // Tokenizes the input 
        char * token = strtok(instruction, " ");
        // adds the line number to the symbol table
        symbolTable[ii].symbol = atoi(token);
        symbolTable[ii].type = 'L';
        symbolTable[ii].location = pointerOther;
        // gets the next token in the line
        // this token would be the command
        token = strtok(NULL, " ");
        ii += 1;
        // checks if this is input command
        if(strcmp(token, "input") == 0){
            // gets variable token
            token = strtok(NULL, " ");
            int jj;
            int location = -1;
            for(jj = 0; jj < ii+1; jj++){
                if(symbolTable[jj].symbol == (int)(token[0])){
                    location = symbolTable[jj].location;
                    break;
                }
            }
            if(location != -1){
                hml[pointerOther] = 0x5000 + location;
            }
            else{
                // inserts variable token into symbol table
                symbolTable[ii].symbol = (int)token[0];
                symbolTable[ii].type = 'V';
                symbolTable[ii].location = variableAllocator;
                ii += 1;
                // adds instruction to hml array
                hml[pointerOther] = 0x5000 + variableAllocator;
                // decrease variable location variable
                variableAllocator = variableAllocator - 1;
            }
            pointerOther += 1;
        }
        // checks if command is print
        else if(strcmp(token, "print") == 0){
            // gets variable token
            token = strtok(NULL, " ");
            int jj;
            int varLocation = 0;
            // finds the location of the variable
            for(jj = 0; jj < ii+1; jj++){
                if(symbolTable[jj].symbol == (int)(token[0])){
                    varLocation = symbolTable[jj].location;
                }
            }
            // adds the hml code into hml array
            hml[pointerOther] = 0x5100 + varLocation;
            pointerOther += 1;
        }
        // checks if token is goto
        else if(strcmp(token, "goto") == 0){
            // gets line number token
            token = strtok(NULL, " ");
            int jj;
            // finds if the line number exists
            for(jj = ii; jj > -1; jj--){
                // if line number has not been loaded
                // set the flag in the flags array
                if(symbolTable[jj].symbol == atoi(token)){
                    hml[pointerOther] = 0x3000 + symbolTable[jj].location;
                    break;
                }
            }
            if(hml[pointerOther] == 0){
                flags[pointerOther] = atoi(token);
                hml[pointerOther] = 0x3000;
            }
            pointerOther += 1;
        }
        // checks if command is if
        else if(strcmp(token, "if") == 0){
            token = strtok(NULL, " ");
            int jj;
            int location = 0;
            // Will give the branch instructions
            int branch_type[2];
            branch_type[0] = 0;
            branch_type[1] = 0;
            // looks for the location of the variable
            for(jj = 0; jj < ii+1; jj++){
                if(symbolTable[jj].symbol == (int)token[0]){
                    location = symbolTable[jj].location;
                }
            }
            // the HML code for loadings a variable
            hml[pointerOther] = 0x4000 + location;
            pointerOther += 1;
            // get the next token, comparison token
            // initialize the branch_type array to determine the branch instruction
            token = strtok(NULL, " ");
            if(strcmp(token, "==") == 0){
                branch_type[0] = 0x3300;
            }
            else if(strcmp(token, "<=") == 0){
                branch_type[0] = 0x3100;
                branch_type[1] = 0x3300;
            }
            else if(strcmp(token, ">=") == 0){
                branch_type[0] = 0x3200;
                branch_type[1] = 0x3300;
            }
            else if(strcmp(token, ">") == 0){
                branch_type[0] = 0x3200;
            }
            else if(strcmp(token, "<") == 0){
                branch_type[0] = 0x3100;
            }
            // get the location of the next variable
            //location = 0;
            int foundSymbol = 0;
            token = strtok(NULL, " ");
            for(jj = 0; jj < ii+1; jj++){
                if(symbolTable[jj].symbol == (int)token[0]){
                    location = symbolTable[jj].location;
                    foundSymbol =1;
                }
            }
            if ((!foundSymbol) & !isalpha(token[0])){
                //If there is a constant that we haven't found:

                symbolTable[ii].symbol = atoi(token);
                symbolTable[ii].type = 'C';
                symbolTable[ii].location = variableAllocator;
                ii += 1;

                hml[variableAllocator] = atoi(token);
                variableAllocator--;
            }
            // subtract the variable from the variable in the accumulator
            hml[pointerOther] = 0x1100 + location;
            pointerOther += 1;
            token = strtok(NULL, " ");
            // get the goto line number and find its location or existence 
            token = strtok(NULL, " ");
            location = -1;
            for(jj = 0; jj < ii+1; jj++){
                if(symbolTable[jj].symbol == atoi(token)){
                    location = symbolTable[jj].location;
                }
            }
            
            // adds the branching instructions into the HML code array
            for(jj = 0; jj < 2; jj++){
                if(branch_type[jj] != 0){
                    if(location == -1){
                        hml[pointerOther] = branch_type[jj];
                        flags[pointerOther] = atoi(token);
                        pointerOther += 1;
                    }
                    else{
                        hml[pointerOther] = branch_type[jj] + location;
                        pointerOther += 1;
                    }
                }
            }
        }
        // checks if command is let
        else if(strcmp(token, "let") == 0){
            token = strtok(NULL, " ");
            int jj;
            int location = -1;
            // get the location of the variable
            for(jj = 0; jj < ii+1; jj ++){
                if(symbolTable[jj].symbol == (int)token[0]){
                    location = symbolTable[jj].location;
                    break;
                }
            }

            if (location == -1)
            {
                symbolTable[ii].symbol = (int) token[0];
                symbolTable[ii].type = 'V';
                symbolTable[ii].location = variableAllocator;
                ii += 1;
                location = variableAllocator;
                variableAllocator--;
                
            }


            // get the equation after the equal sign
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            char space[2] = " ";
            char infixExpr[256] = "";
            while(token != NULL){
                strncat(infixExpr, token, sizeof(&token));
                strncat(infixExpr, space, sizeof(&space));
                token = strtok(NULL, " ");
            }
            // convert the infix to postfix equation
            char postfixExpr[256];
            char postfixCopy[256];

            
            convertToPostfix(infixExpr, postfixExpr);

            strncpy(postfixCopy, postfixExpr, 256);
            



            char* exprToken = strtok(postfixExpr, " ");
            StackNodePtr item;

           
            int temp[256] = { -1};
            while (exprToken)
            {
                

                if (isalpha(exprToken[0]))
                {
                    int inSymbolTable = 0;
                    for(jj = 0; jj < ii+1; jj++){
                        if(symbolTable[jj].symbol == (int)(exprToken[0])){
                            inSymbolTable = 1;
                        }
                         

                    }
                    if(!inSymbolTable)
                    {
                        symbolTable[ii].symbol = (int)exprToken[0];
                        symbolTable[ii].type = 'V';
                        symbolTable[ii].location = variableAllocator;
                        ii += 1;
                        variableAllocator--;
                    }
                        
                }
                if (isdigit(exprToken[0]))
                {
                    int inSymbolTable = 0;
                    for(jj = 0; jj < ii+1; jj++){
                        if(symbolTable[jj].symbol == atoi(exprToken)){
                            inSymbolTable = 1;
                        }
                         

                    }
                    if(!inSymbolTable)
                    {
                        symbolTable[ii].symbol = atoi(exprToken);
                        symbolTable[ii].type = 'C';
                        symbolTable[ii].location = variableAllocator;

                        hml[variableAllocator] = atoi(exprToken);
                        ii += 1;
                        variableAllocator--;
                    }
                        
                }

                push(&item, exprToken[0]);
                //printStack(item);
                


                char getval = stackTop(item);
                if(isOperator(getval))
                {
                    //printf("found an operator\n");
                    char op = pop(&item); //This is the operator
                    
                    char ex2 = pop(&item);
                    char ex1 = pop(&item);



                    //Load first value into accumulator

                    //(Buggy)

                    hml[pointerOther] = 0x4000; //Add the location of ex1
                    int locaVar = 0 ;
                    if (isalpha(ex1))
                    {
                        
                        for(jj = 0; jj < ii+1; jj ++){
                            if(symbolTable[jj].symbol == (int) ex1) {
                              
                               locaVar = symbolTable[jj].location;
                            }
                        }
                        hml[pointerOther] += locaVar;
                        
                    } else if (isdigit(ex1))
                    {
                        int locaVar = 0 ;
                        for(jj = 0; jj < ii+1; jj ++){
                            if(symbolTable[jj].symbol == (int)(ex1 - '0')) {
                              
                               locaVar = symbolTable[jj].location;
                            }
                        }
                        hml[pointerOther] += locaVar;
                        
                    } else {
                        for(int k = 255; k >= 0; k--)
                        {
                            if (temp[k] != -1)
                            {
                                locaVar = k;
                                temp[k] = -1;
                                break;
                            }
                        }
                        
                    }

                    pointerOther++;



                    //Get location of ex2
                    int locEx2;

                    if (isalpha(ex2))
                        for(jj = 0; jj < ii+1; jj ++){
                        if(symbolTable[jj].symbol == (int) ex2) {
                            
                            locEx2 = symbolTable[jj].location;
                        }
                    } else if (isdigit(ex2)) {
                        for(jj = 0; jj < ii+1; jj ++){
                            if(symbolTable[jj].symbol == (int)(ex2 - '0')) {
                                
                                locEx2 = symbolTable[jj].location;
                            }
                        }
                    } else {
                        for(int k = 255; k >= 0; k--)
                        {
                            if (temp[k] != -1)
                            {
                                locEx2 = k;
                                temp[k] = -1;
                                break;
                            }
                        }
                        
                    }
                    



                    switch (op)
                    {
                        case '/':
                            hml[pointerOther] = 0x1300; //Add the location of ex2
                            
                            break;
                        case '*':
                            hml[pointerOther] = 0x1200; //Add the location of ex2
                            
                            break;
                        case '-':
                            hml[pointerOther] = 0x1100; //Add the location of ex2
                            
                            break;
                        case '+':
                            hml[pointerOther] = 0x1000; //Add the location of ex2
                
                            break;
                    }

                    //Add the location of ex2
                    hml[pointerOther] += locEx2;


                    pointerOther++;


                    hml[pointerOther] = 0x4100; //Store the value of the acc into temporary location (how to get?)


                    hml[pointerOther] += variableAllocator;


                    
                    for (int k = 0; k < 256; k++)
                    {
                        if (temp[k] != -1)
                        {
                            temp[k] = variableAllocator;
                            break;
                        }
                    }

                    //set your array for "$" values

                    variableAllocator--;



                    pointerOther++;
                 
                    push(&item, '$');

                }

                exprToken = strtok(NULL, " ");
                
            }
            
            //Load value from temporary location 
            hml[pointerOther] = 0x4000; //Load temp value into accumulator
            hml[pointerOther] += temp[0];
            pointerOther++;

            //Store value in 
            hml[pointerOther] = 0x4100; //Store the value of the acc into location of variable
            hml[pointerOther] += location;
            pointerOther++;

            //break;
        }


        // checks if command is end
        else if(strcmp(token, "end") == 0){

            hml[pointerOther] = 0xFF00;
            pointerOther++;
            break;
        }

    }
    // for(ii = 0; ii < 20; ii++){
    //     printf("-------> %d ", ii);
    //     printf("Symbol: %d Type: %c Location: %d\n", symbolTable[ii].symbol,symbolTable[ii].type, symbolTable[ii].location);
    // }
    // for(ii = 0; ii < 256; ii++){
    //     printf("-------> code: 0x%04hX\n", hml[ii]);
    // }

    for (int i = 0; i< 256; i++ )
    {
        hmlGlobal[i] = hml[i];
        flagsGlobal[i] = flags[i];
        symbolTableGlobal[i].location = symbolTable[i].location;
        symbolTableGlobal[i].symbol = symbolTable[i].symbol;
        symbolTableGlobal[i].type = symbolTable[i].type;
    }

    return;
}


/**
 * Pass 2
 * This function goes through a symbol table and a flags array and changes the hml code to reflect new flags. 
 * 
*/
void pass2(tableEntry symbolTable[], int hml[], int flags[])
{
    for (int ii = 0; ii < 256; ii++)
    {
        if (flagsGlobal[ii] != -1)
        {
            //printf("Changing flag %d: %d\n", ii, flagsGlobal[ii]);
            for(int jj = 0; jj < 256; jj++){
                if(symbolTable[jj].symbol == flagsGlobal[ii]){
                    int location = symbolTable[jj].location;
                    hmlGlobal[ii] += location;
                    break;
                }
            }
        }
    }



}


/**
 * printToFile()
 * 
 * outputs "output.hml"
 * returns: nothing. 
 * 
*/
void printToFile(){
    FILE *fp;
    
    fp = fopen("output.hml", "w");
    for(int i = 0; i < 256; i ++)
    {
        fprintf(fp, "%02hX %04hX\n", i, hmlGlobal[i]);
    }
    
    fclose(fp); //Don't forget to close the file when finished
}


int main(int argc, char *argv[]){

    FILE *infp;
    tableEntry symbolTable[256];
   
    if (argc == 2){
        infp = fopen(argv[1], "r"); // opens file in read mode
        if(infp == NULL){
            printf("File could not be opened: exiting!");
            exit(EXIT_FAILURE);
        }
        else{
            // put instructions into memory
            pass1(symbolTable, infp);

            printf("Pass 1 Complete.\n");

            pass2(symbolTableGlobal, flagsGlobal, hmlGlobal);

            printf("Pass 2 Complete.\n");

            printToFile();

            printf("Outputted file to output.hml\n");

            // for(int ii = 0; ii < 256; ii++){
            //     printf("-------> code: 0x%04hX\n", hmlGlobal[ii]);
            // }
            // for(int ii = 0; ii < 16; ii++){
            //     for(int jj = 0; jj < 16; jj++)
            //     {
            //         printf("%d ", flagsGlobal[ii*16 + jj]);
            //     }
            //         printf("\n");
            // }
        }
        // close file
        fclose(infp);
    }
   
    else{
        printf("No file added to compile: exiting!\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}