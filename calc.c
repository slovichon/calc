#include <stdio.h>
#include "buffer.h"

#define STACK_SIZE 100

#define TT_EOF	0
#define TT_OP 	1
#define TT_NUM	2
#define TT_VAR	3
#define TT_NOP	4
#define TT_EVAL	5

#define FALSE	0
#define TRUE	1

int next_token(FILE *in, void **val);
char *xstrdup(const char *);

double stack[STACK_SIZE];

int main(int argc, char *argv[])
{
	char ch;
	char vars[26 * 2 + 1];
	int level = 0;
	void *token, *lasttoken;
	int type;

	while ((type = next_token(stdin, &token)) != TT_EOF) {
		switch (type) {
			case TT_OP:
				break;
			case TT_NUM:
//				push(atod(token));
				break;
			case TT_VAR:
				break;
			case TT_EOF:
				return 0;
		}
//		free(lasttoken);
//		lasttoken = token;
		if (token != NULL)
			free(token);
	}
//	if (token)
//		free(token);

	return 0;
}

int next_token(FILE *in, void **val)
{
	static bool esc = FALSE;
	int ch, nextch, count = 0;
	while (((ch = getc(in)) != EOF) && isspace(ch));
	if (ch == EOF) {
		*val = NULL;
		return TT_EOF;
	}

	if (esc) {
		esc = FALSE;
		*val = NULL;
		return TT_NOP;
	}

	/* escape */
	if (ch == '\\') {
		esc = TRUE;
	}

	/* newline */
	if (ch == '\n') {
		*val = NULL;
		return TT_EVAL;
	}

	/* variable */
	if (isalpha(ch)) {
		Buffer *p = Buffer_init(1);
		do {
			Buffer_addch(p, tolower(ch));
			ch = getc(in);
		} while (ch && isalpha(ch));
		ungetc(ch, in);
		*val = Buffer_get(p);
bark("read a variable: %s", Buffer_get(p));
		Buffer_long_free(&p, TRUE);
		return TT_VAR;
	}

	/* number */
	if ((ch == '-') || (ch == '.') || isdigit(ch)) {
		bool neg = FALSE;
		double d = 0, *dp;
		if (ch == '-') {
			nextch = getc(in);
			if (nextch == '-') {
				neg = TRUE;
			} else if (!isdigit(nextch)) {
				/*
				 * not a number, must be the
				 * - (minus) operator
				 */
				ungetc(nextch, in);
				*val = xstrdup("-");
				return TT_OP;
			}
		}

		/* integer part */
		//while (((ch = getc(in)) != EOF) && isdigit(ch)) {
		while (ch != EOF && isdigit(ch)) {
			d = d*10 + (ch - '0');
			count++;
			ch = getc(in);
		}

		/* fractional part */
		if (ch == '.') {
			while (((ch = getc(in)) != EOF) && isdigit(ch)) {
				d += (double)(ch-'0')/(10*++count);
				count++;
			}
		}

		/* exponential part */
		if ((ch == 'e') || (ch == 'E')) {
			int expneg = FALSE;
			int expcount = 0, exp = 0;
bark("reading power");
			nextch = getc(in);
			if (nextch == '-') {
bark("power is neg");
				expneg = TRUE;
				nextch = getc(in);
			}
//			while (((nextch = getc(in)) != EOF) && isdigit(nextch)) {
			while (nextch != EOF && isdigit(nextch)) {
				exp = exp*10 + (nextch-'0');
				expcount++;
				nextch = getc(in);
			}
bark("finished reading power");
			if (expcount) {
				if (expneg)
					exp = -exp;
bark("power looks to be %d", exp);
				d = d*pow(10, exp);
				ch = nextch;
			} else {
bark("not a power!");
				/* 'e' must have been a mistake */
				ungetc(nextch, in);
			}
		}
		if (neg)
			d = -d;
bark("putting back '%c' (%d)", ch, count);
		ungetc(ch, in);
		dp = (double *)malloc(sizeof(double));
		*dp = d;
		*val = dp;
bark("read a number: [%e] [%f] [%d]", d, d, (int)d);
		return TT_NUM;
	}

	if (ch == '+') {
bark("read + op");
		*val = xstrdup("+");
		return TT_OP;
	}

	if (ch == '/') {
bark("read / op");
		*val = xstrdup("/");
		return TT_OP;
	}

	if (ch == '*') {
		nextch = getc(in);
		if (nextch != EOF && nextch == '*') {
bark("read ** op");
			*val = xstrdup("**");
			return TT_OP;
		}
bark("read * op");
		ungetc(nextch, in);
		*val = xstrdup("*");
		return TT_OP;
	}

}

/*

			case '(':
				level++;
				break;

			case ')':
				level--;
				break;

			case '.':

			case '-':
				*type = OPERATOR;
				break;

			case '+':
				*type = OPERATOR;
				break;

			case '*':
				*type = OPERATOR;
				break;

			case '/':
				*type = OPERATOR;
				break;

			case '=':
			case '':
			case '':
			case '':


		}
	}

	return token;
}

*/

#include <assert.h>
#include <string.h>
char *xstrdup(const char *s)
{
	char *p;
	assert((p = strdup(s)) != NULL);
	return p;
}
