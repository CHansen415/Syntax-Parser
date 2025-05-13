/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "front.h"
#include "parser.h"
#include <stdlib.h>
/* Global Variable */
int nextToken;
int exitCode;
char lexeme[100];
char toke[100];
int line_count;
/* Local Variables */
static int charClass;
//static char lexeme [100];
static char nextChar;
static int lexLen;
static FILE *in_fp;
//static char toke[100];

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();

/******************************************************/
/* main driver */
int main(int argc, char *argv[]) 
{
    printf("Cooke Parser :: R11706114\n");
    /* Open the input data file and process its contents */
    if(argc < 2){
        exit(2);
        printf("ERROR - user did not provide file\n");
    }
    else if ((in_fp = fopen(argv[1], "r")) == NULL) {
        exit(3);
        printf("ERROR - cannot open %s \n", argv[1]);
    } 
    else {
        line_count = 1;
        getChar();
        while(nextToken != EOF) {
            lex();
            parser();
        } 
        printf("Syntax Validated\n");
	exit(0);
        }
    }


/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the 
 * token */
static int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            getChar();
            if(nextChar == '+'){
                addChar();
                nextToken = INC_OP;
            }
            else{
            fseek(in_fp, -1, SEEK_CUR);
            nextToken = ADD_OP;
            }
            break;

        case '-':
            addChar();
            getChar();
            if(nextChar == '-'){
                addChar();
                nextToken = DEC_OP;
            }
            else{
            fseek(in_fp, -1, SEEK_CUR);
            nextToken = SUB_OP;
            }
            break;
        case '*':
            addChar();
            getChar();
            if(nextChar == '*'){
                addChar();
                nextToken = POW_OP;
            }
            else{
            fseek(in_fp, -1, SEEK_CUR);
            nextToken = MULT_OP;
            }
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        case ':':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                nextToken = ASSIGN_OP;
            }
            else{
                fseek(in_fp, -1, SEEK_CUR);
                nextToken = COLON;
            }
            break;
        case '<':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                nextToken = LEQUAL_OP;
            }
            else if(nextChar == '>'){
                addChar();
                nextToken = NEQUAL_OP;
            }
            else{
                fseek(in_fp, -1, SEEK_CUR);
                nextToken = LESSER_OP;
            }
            break;
        case '>':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                nextToken = GEQUAL_OP;
            }
            else{
                fseek(in_fp, -1, SEEK_CUR);
                nextToken = GREATER_OP;
            }
            break;
        case ';':
            addChar();
            nextToken = SEMICOLON;
            break;
        case '=':
            addChar();
            nextToken = EQUAL_OP;
            break;
        case '%':
            addChar();
            nextToken = UNKNOWN;
            break;
        case '!':
            addChar();
            nextToken = UNKNOWN;
            break;
        case '$':
            addChar();
            nextToken = UNKNOWN;
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its 
 * character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;		
        else charClass = UNKNOWN;
	
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace 
 * character */
static void getNonBlank() {
    while (isspace(nextChar)) {
	if(nextChar == '\n'){
	++line_count;
	}
	    getChar();	
}
}
/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            for(int i = 0; i < lexLen; i++){
                lexeme[i] = tolower(lexeme[i]);
            }
            if(strcmp(lexeme, "read") == 0 ){
                nextToken = KEY_READ;
            }
            else if(strcmp(lexeme, "print")==0){
                nextToken = KEY_PRINT;
            }
            else if(strcmp(lexeme, "if")== 0){
                nextToken = KEY_IF;
            }
            else if(strcmp(lexeme, "else")==0){
                nextToken = KEY_ELSE;
            }
            else if(strcmp(lexeme, "begin")== 0){
                nextToken = KEY_BEGIN;
            }
            else if(strcmp(lexeme, "end") == 0){
                nextToken = KEY_END;
            }
            else{
            nextToken = IDENT;
            }
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
          /*lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;*/
            break;
    } /* End of switch */

    switch(nextToken){
        case 10:
           strcpy(toke, "INT_LIT");
            break;
        case 11:
            strcpy(toke,"IDENT");
            break;
        case 20:
            strcpy(toke,"ASSIGN_OP");
            break;
        case 21:
            strcpy(toke,"ADD_OP");
            break;
        case 22:
            strcpy(toke,"SUB_OP");
            break;
        case 23:
            strcpy(toke, "MULT_OP");
            break;
        case 24:
            strcpy(toke, "DIV_OP");
            break;
        case 25:
            strcpy(toke, "LEFT_PAREN");
            break;
        case 26: 
            strcpy(toke,"RIGHT_PAREN");
            break;
        case 27: 
            strcpy(toke, "LESSER_OP");
            break;
        case 28:
            strcpy(toke, "GREATER_OP");
            break;
        case 29:
            strcpy(toke, "NEQUAL_OP");
            break;
        case 30:
            strcpy(toke, "LEQUAL_OP");
            break;
        case 31:
            strcpy(toke, "GEQUAL_OP");
            break;
        case 32:
            strcpy(toke, "POW_OP");
            break;
        case 33:
            strcpy(toke, "INC_OP");
            break;
        case 34:
            strcpy(toke, "DEC_OP");
            break;
        case 35:
            strcpy(toke, "SEMICOLON");
            break;
        case 36:
            strcpy(toke, "COLON");
            break;
        case 37:
            strcpy(toke,"KEY_READ");
            break;
        case 38:
            strcpy(toke ,"KEY_PRINT");
            break;
        case 39:
            strcpy(toke, "KEY_END");
            break;
        case 40:
            strcpy(toke,"KEY_BEGIN");
            break;
        case 41:
            strcpy(toke,"KEY_IF");
            break;
        case 42:
            strcpy(toke,"KEY_ELSE");
            break;
        case 43:
            strcpy(toke, "EQUAL_OP");
            break;
        case 99:
            strcpy(toke, "UNKNOWN");
            break;
        case -1:
            strcpy(toke, "");
            strcpy(lexeme, "");
            break;
    }

    printf("%s	%s\n", lexeme, toke);
    return nextToken;
} /* End of function lex */

