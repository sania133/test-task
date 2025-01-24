#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s \"expression\"\n", argv[0]);
		return ERROR_OTHER;
	}
	int error = 0;
	input = argv[1];
	int result = parse_expr();
	skip_whitespace();
	if (*input != '\0')
	{
		fprintf(stderr, "Unexpected input: %s\n", input);
		return ERROR_PARSE_ERROR;
	}
	if (error != 0)
	{
		return error;
	}
	else
	{
		printf("%d\n", result);
		return 0;
	}
}
