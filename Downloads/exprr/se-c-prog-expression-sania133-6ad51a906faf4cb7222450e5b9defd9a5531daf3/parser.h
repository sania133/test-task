#ifndef PARSER_H
#define PARSER_H

#define ERROR_UNSUPPORTED_OPERATION 1
#define ERROR_DIVISION_BY_ZERO 2
#define ERROR_PARSE_ERROR 3
#define ERROR_OTHER 4

extern const char *input;

void skip_whitespace();
int parse_number();
int power(int base, int exp);
int parse_expr();
int parse_term();
int parse_factor();
int parse_base();

#endif
