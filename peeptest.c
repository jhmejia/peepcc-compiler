//================================================
// includes and defines
//================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "infix-to-postfix.h"
#include "peepcc.h"

//================================================
// Function Prototypes
//================================================
void get_instruction(tableEntry symbolTable[], FILE *infp);

int main(int argc, char *argv[]){

    FILE *infp;
    tableEntry symbolTable[256];
    // Checking the arguments
    // if there is only one argument then it is going
    // to read user input
    if (argc == 2){
        infp = fopen(argv[1], "r"); // opens file in read mode
        if(infp == NULL){
            printf("FILE COULD NOT BE OPENED.");
        }
        else{
            // put instructions into memory
            get_instruction(symbolTable, infp);
        }
        // close file
        fclose(infp);
    }
    // this will error out the program
    else{
        printf("MUST GIVE FILE INPUT.\n");
    }
    return 0;
}

// loads user inputted instructions into memory
void get_instruction(tableEntry symbolTable[], FILE *infp){
    // initialize local variables
    int flags[256];
    int ii;
    int instructionCtr = 0;
    // int variableCtr = 0;
    int variableAllocator = 0xFF;
    int hml[256];

    // initialize all values in flags array to be -1
    for(ii = 0; ii < 256; ii++){
        flags[ii] = -1;
        hml[ii] = 0;
    }
    
    // read in file input as a line
    char instruction[256];
    // symbol table counter
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
        symbolTable[ii].location = instructionCtr;
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
                hml[instructionCtr] = 0x5000 + location;
            }
            else{
                // inserts variable token into symbol table
                symbolTable[ii].symbol = (int)token[0];
                symbolTable[ii].type = 'V';
                symbolTable[ii].location = variableAllocator;
                ii += 1;
                // adds instruction to hml array
                hml[instructionCtr] = 0x5000 + variableAllocator;
                // decrease variable location variable
                variableAllocator = variableAllocator - 1;
            }
            instructionCtr += 1;
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
            hml[instructionCtr] = 0x5100 + varLocation;
            instructionCtr += 1;
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
                    hml[instructionCtr] = 0x3000 + symbolTable[jj].location;
                    break;
                }
            }
            if(hml[instructionCtr] == 0){
                flags[instructionCtr] = 1;
                hml[instructionCtr] = 0x3000;
            }
            instructionCtr += 1;
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
            hml[instructionCtr] = 0x4000 + location;
            instructionCtr += 1;
            // get the next token, comparison token
            // initialize the branch_type array to determine the branch instruction
            token = strtok(NULL, " ");
            if(strcmp(token, "==") == 0){
                branch_type[0] = 0x3300;
            }
            else if(strcmp(token, "<=") == 0){
                branch_type[0] = 0x3100;
                branch_type[0] = 0x3300;
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
            token = strtok(NULL, " ");
            for(jj = 0; jj < ii+1; jj++){
                if(symbolTable[jj].symbol == (int)token[0]){
                    location = symbolTable[jj].location;
                }
            }
            // subtract the variable from the variable in the accumulator
            hml[instructionCtr] = 0x1100 + location;
            instructionCtr += 1;
            token = strtok(NULL, " ");
            // get the goto line number and find its location or existence 
            token = strtok(NULL, " ");
            for(jj = 0; jj < ii+1; jj++){
                if(symbolTable[jj].symbol == atoi(token)){
                    location = symbolTable[jj].location;
                }
            }
            location = -1;
            // adds the branching instructions into the HML code array
            for(jj = 0; jj < 2; jj++){
                if(branch_type[jj] != 0){
                    if(location == -1){
                        hml[instructionCtr] = branch_type[jj];
                        flags[instructionCtr] = 1;
                        instructionCtr += 1;
                    }
                    else{
                        hml[instructionCtr] = branch_type[jj] + location;
                        instructionCtr += 1;
                    }
                }
            }
        }
        // checks if command is let
        else if(strcmp(token, "let") == 0){
            token = strtok(NULL, " ");
            int jj;
            int location;
            // get the location of the variable
            for(jj = 0; jj < ii+1; jj ++){
                if(symbolTable[jj].symbol == (int)token[0]){
                    location = symbolTable[jj].location;
                    break;
                }
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
            convertToPostfix(infixExpr, postfixExpr);
            printf("%s\n", postfixExpr);
        }
        // checks if command is end
        else if(strcmp(token, "end") == 0){
            break;
        }

    }
    for(ii = 0; ii < 20; ii++){
        printf("-------> %d ", ii);
        printf("Symbol: %d Type: %c Location: %d\n", symbolTable[ii].symbol,symbolTable[ii].type, symbolTable[ii].location);
    }
    for(ii = 0; ii < 14; ii++){
        printf("-------> code: 0x%X\n", hml[ii]);
    }
    return;
}