typedef struct  { 
    int symbol;
    char type; // 'C', 'L' or 'V'
    int location; // 00 to FF  
} tableEntry;



void pass1(tableEntry symbolTable[], FILE *infp);
void pass2(tableEntry symbolTable[], int hml[], int flags[]);