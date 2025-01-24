#include "parser.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *input;
int error = 0;

void skip_whitespace(void)
{
	while (isspace(*input))
	{
		input++;
	}
}

int parse_number(void)
{
	skip_whitespace();
	int num = 0;
	int sign = 1;

	if (*input == '-')
	{
		sign = -1;
		input++;
	}
	else if (*input == '+')
	{
		input++;
	}

	if (!isdigit(*input))
	{
		fprintf(stderr, "Expected number\n");
		error = ERROR_PARSE_ERROR;
		return 0;
	}

	while (isdigit(*input))
	{
		num = num * 10 + (*input - '0');
		input++;
	}
	return num * sign;
}

int power(int base, int exp)
{
	if (exp < 0)
	{
		fprintf(stderr, "Negative exponent not supported for integer power\n");
		error = ERROR_UNSUPPORTED_OPERATION;
		return 0;
	}
	int result = 1;
	for (int i = 0; i < exp; i++)
	{
		result *= base;
	}
	return result;
}

int parse_expr(void)
{
	int value = parse_term();

	while (!error)
	{
		skip_whitespace();
		if (*input == '+')
		{
			input++;
			value += parse_term();
		}
		else if (*input == '-')
		{
			input++;
			value -= parse_term();
		}
		else
		{
			break;
		}
	}

	return value;
}

int parse_term(void)
{
	int value = parse_factor();

	while (!error)
	{
		skip_whitespace();
		if (*input == '*')
		{
			input++;
			value *= parse_factor();
		}
		else if (*input == '/')
		{
			input++;
			int divisor = parse_factor();
			if (divisor == 0)
			{
				fprintf(stderr, "Division by zero\n");
				error = ERROR_DIVISION_BY_ZERO;
				return 0;
			}
			value /= divisor;
		}
		else if (*input == '%')
		{
			input++;
			int divisor = parse_factor();
			if (divisor == 0)
			{
				fprintf(stderr, "Division by zero\n");
				error = ERROR_DIVISION_BY_ZERO;
				return 0;
			}
			value %= divisor;
		}
		else if (*input == '|' && *(input + 1) != '|')
		{
			input++;
			value |= parse_factor();
		}
		else if (*input == '>' && *(input + 1) == '>')
		{
			input += 2;
			int shift = parse_factor();
			if (shift < 0 || shift >= 32)
			{
				fprintf(stderr, "Invalid right shift amount: %d\n", shift);
				error = ERROR_UNSUPPORTED_OPERATION;
				return 0;
			}
			value >>= shift;
		}
		else if (*input == '<' && *(input + 1) == '<')
		{
			input += 2;
			int shift = parse_factor();
			if (shift < 0 || shift >= 32)
			{
				fprintf(stderr, "Invalid left shift amount: %d\n", shift);
				error = ERROR_UNSUPPORTED_OPERATION;
				return 0;
			}
			value <<= shift;
		}
		else if (*input == '^')
		{
			input++;
			value ^= parse_term();
		}
		else if (*input == '&')
		{
			input++;
			value &= parse_factor();
		}
		else
		{
			break;
		}
	}

	return value;
}

int parse_factor(void)
{
	skip_whitespace();

	int sign = 1;

	if (*input == '-')
	{
		sign = -1;
		input++;
	}
	else if (*input == '+')
	{
		input++;
	}
	else if (*input == '~')
	{
		input++;
		return ~parse_factor();
	}

	int value = parse_base();

	skip_whitespace();
	if (*input == '*' && *(input + 1) == '*')
	{
		input += 2;
		int exponent = parse_factor();
		value = power(value, exponent);
	}

	return value * sign;
}

int parse_base()
{
	skip_whitespace();

	if (*input == '(')
	{
		input++;
		int value = parse_expr();
		skip_whitespace();

		if (*input != ')')
		{
			fprintf(stderr, "Mismatched parentheses\n");
			error = ERROR_PARSE_ERROR;
			return 0;
		}

		input++;
		return value;
	}
	else
	{
		return parse_number();
	}
}
