#include <stdio.h>
#include "buffer.h"

#define SIZ 100

#define TT_EOF	0
#define TT_OP 	1
#define TT_NUM	2
#define TT_VAR	3

#define FALSE	0
#define TRUE	1

int next_token(FILE *in, void **val);

int main(int argc, char *argv[])
{
	char ch;
	char vars[26 * 2 + 1];
	double val[SIZ];
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
		free(lasttoken);
		lasttoken = token;
	}
	free(token);

	return 0;
}

int next_token(FILE *in, void **val)
{
	int ch, nextch, count = 0;
	while (((ch = getc(in)) != EOF) && isspace(ch));
	if (ch == EOF) {
		*val = NULL;
		return TT_EOF;
	}

	/* variable */
	if (isalpha(ch)) {
		char *s;
		Buffer *p = Buffer_init(1);
		do {
			Buffer_addch(p, tolower(ch));
			ch = getc(in);
		} while (ch && isalpha(ch));
		ungetc(ch, in);
		*val = Buffer_get(p);
		Buffer_long_free(&p, TRUE);
		return TT_VAR;
	}

	/* number */
	if ((ch == '-') || (ch == '.') || isdigit(ch)) {
		bool neg = FALSE;
		double d = 0;
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
				*type = TT_OP;
				return "-";
			}
		}
			
		/* integer part */
		//while (((ch = getc(in)) != EOF) && isdigit(ch)) {
		while (ch != EOF && isdigit(ch))
			d = d*10 + (ch - '0');
			count++;
			ch = getc(in);
		}
			
		/* fractional part */
		if (ch == '.') {
			int count = 0;
			while (((ch = getc(in)) != EOF) && isdigit(ch)) {
				d += (ch-'0')/(10*++count);
				count++;
			}
		}

		/* exponential part */
		if ((ch == 'e') || (ch == 'E')) {
			int expneg = FALSE;
			int expcount, exp = 0;
			if (nextch == '-') {
				expeng = TRUE;
				nextch = getc(in);
			}
//			while (((nextch = getc(in)) != EOF) && isdigit(nextch)) {
			while (nextch != EOF && isdigit(nextch))
				exp = d*10 + (nextchar-'0');
				expcount++;
				nextch = getc(in);
			}
			if (expcount) {
				if (expneg)
					exp = -exp;
				d = d*pow(10, exp);
				ch = nextch;
			} else {
				/* 'e' must have been a mistake */
				ungetc(nextch);
			}
		}
		if (neg)
			d = -d;
		ungetc(ch, in);
		*val = d;
		return TT_NUM;
	}

	if (ch == '+') {
		*val = "+";
		return TT_OP;
	}

	if (ch == '/') {
		*val = "/";
		return TT_OP;
	}

	if (ch == '*') {
		nextch = getc(in);
		if (nextch != EOF && )
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
