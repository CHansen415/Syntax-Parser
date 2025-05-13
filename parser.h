#ifndef PARSER_H
#define PARSER_H
void parser();
void stmt();
void cmpr();
void oper();
void expr();
void term();
void factor();

extern int nextToken;
extern int exitCode;
extern char lexeme[100];
extern char toke[100];
extern int line_count;
#endif
