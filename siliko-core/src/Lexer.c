/* Lexer.c: Lexical analyzer
 * Copyright 2012-2021 Vincent Damewood
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <SilikoCore/DataSource.h>
#include <SilikoCore/Lexer.h>

struct SilikoLexer
{
	SilikoDataSource *Source;
	SilikoToken Token;
};
typedef struct SilikoLexer SilikoLexer;


/* Values taken from Google Calculator 2011-07-06 */
#define EULER 2.71828183
#define PI    3.14159265

static int isOperator(int character)
{
	return
	(
		character == '+'
		|| character == '-'
		|| character == '/'
		|| character == '*'
		|| character == '^'
		|| character == ','
		|| character == '('
		|| character == ')'
	);
}

static int isIdCharacter(int character)
{
	return (isalnum(character) || character == '_');
}

enum SilikoDfaState
{
	DFA_ERROR = -1,
	DFA_START = 0,	/* "" */
	DFA_E,		/* "e" */
	DFA_PI_1,	/* "p" */
	DFA_PI_2,	/* "pi" */
	DFA_DICE,	/* "d" */
	DFA_ID,		/* ([a-ce-z][a-z0-9]*) | (d[a-z][a-z0-9]*) */
	DFA_INTEGER,	/* [0-9]+ */
	DFA_FLOAT,	/* [0-9]+\.[0-9]+ */
	DFA_TERM_INTEGER,
	DFA_TERM_FLOAT,
	DFA_TERM_E,
	DFA_TERM_PI,
	DFA_TERM_CHAR,
	DFA_TERM_STRING,
	DFA_TERM_EOL,
	DFA_END
};
typedef enum SilikoDfaState SilikoDfaState;

struct Lexeme
{
	char *Buffer;
	size_t Current;
	size_t End;
};
typedef struct Lexeme Lexeme;

static int Append(Lexeme *Lex, char NewChar)
{
	if (Lex->Current == Lex->End)
	{
		char *Temp;
		Lex->End++;
		if (!(Temp = realloc(Lex->Buffer, Lex->End)))
			return 0;
		else
			Lex->Buffer = Temp;
	}

	Lex->Buffer[Lex->Current++] = NewChar;
	return -1;
}

void SilikoLexerNext(SilikoLexer *Lexer)
{
	SilikoDfaState dfaState = DFA_START;
	Lexeme Lex = {NULL, 0, 4};

	if (Lexer->Token.Type == SILIKO_TOK_ID)
		free(Lexer->Token.Id);

	if (Lexer->Token.Type == SILIKO_TOK_EOL || Lexer->Token.Type == SILIKO_TOK_ERROR)
		return;

	if (!(Lex.Buffer = malloc(Lex.End)))
	{
		Lexer->Token.Type = SILIKO_TOK_ERROR;
		Lexer->Token.Integer = 0;
		return;
	}

	while (dfaState != DFA_END)
	switch (dfaState)
	{
	case DFA_END:
		break;
	case DFA_START:
		if (isOperator(SilikoDataSourceGet(Lexer->Source)))
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			Append(&Lex, '\0');
			SilikoDataSourceAdvance(Lexer->Source);
			dfaState = DFA_TERM_CHAR;
		}
		else if (SilikoDataSourceGet(Lexer->Source) == 'd')
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
			dfaState = DFA_DICE;
		}
		else if (SilikoDataSourceGet(Lexer->Source) == 'e')
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
			dfaState = DFA_E;
		}
		else if (SilikoDataSourceGet(Lexer->Source) == 'p')
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
			dfaState = DFA_PI_1;
		}
		else if (isdigit(SilikoDataSourceGet(Lexer->Source)))
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
			dfaState = DFA_INTEGER;
		}
		else if (isalpha(SilikoDataSourceGet(Lexer->Source)))
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
			dfaState = DFA_ID;
		}
		else if (isspace(SilikoDataSourceGet(Lexer->Source)))
		{
			SilikoDataSourceAdvance(Lexer->Source);
		}
		else if (SilikoDataSourceGet(Lexer->Source) == '\0')
		{
			dfaState = DFA_TERM_EOL;
		}
		else
		{
			dfaState = DFA_ERROR;
		}
		break;
	case DFA_DICE:
		if (isalpha(SilikoDataSourceGet(Lexer->Source)))
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
			dfaState = DFA_ID;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_CHAR;
		}
		break;
	case DFA_E:
		if (isalnum(SilikoDataSourceGet(Lexer->Source)))
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
			dfaState = DFA_ID;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_E;
		}
		break;
	case DFA_PI_1:
		if (SilikoDataSourceGet(Lexer->Source) == 'i')
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
			dfaState = DFA_PI_2;
		}
		else if (isIdCharacter(SilikoDataSourceGet(Lexer->Source)))
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
			dfaState = DFA_ID;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_STRING;
		}
		break;
	case DFA_PI_2:
		if (isIdCharacter(SilikoDataSourceGet(Lexer->Source)))
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
			dfaState = DFA_ID;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_PI;
		}
		break;
	case DFA_ID:
		if (isalnum(SilikoDataSourceGet(Lexer->Source)))
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_STRING;
		}
		break;
	case DFA_INTEGER:
		if (SilikoDataSourceGet(Lexer->Source) == '.')
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
			dfaState = DFA_FLOAT;
		}
		else if (isdigit(SilikoDataSourceGet(Lexer->Source)))
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_INTEGER;
		}
		break;
	case DFA_FLOAT:
		if (isdigit(SilikoDataSourceGet(Lexer->Source)))
		{
			Append(&Lex, SilikoDataSourceGet(Lexer->Source));
			SilikoDataSourceAdvance(Lexer->Source);
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_FLOAT;
		}
		break;
	case DFA_TERM_INTEGER:
		Lexer->Token.Type = SILIKO_TOK_INTEGER;
		Lexer->Token.Integer = strtoll(Lex.Buffer, NULL, 10);
		free(Lex.Buffer);
		dfaState = DFA_END;
		break;
	case DFA_TERM_FLOAT:
		Lexer->Token.Type = SILIKO_TOK_FLOAT;
		Lexer->Token.Float = atof(Lex.Buffer);
		free(Lex.Buffer);
		dfaState = DFA_END;
		break;
	case DFA_TERM_E:
		Lexer->Token.Type = SILIKO_TOK_FLOAT;
		Lexer->Token.Float = EULER;
		free(Lex.Buffer);
		dfaState = DFA_END;
		break;
	case DFA_TERM_PI:
		Lexer->Token.Type = SILIKO_TOK_FLOAT;
		Lexer->Token.Float = PI;
		free(Lex.Buffer);
		dfaState = DFA_END;
		break;
	case DFA_TERM_CHAR:
		Lexer->Token.Type = Lex.Buffer[0];
		Lexer->Token.Integer = 0;
		free(Lex.Buffer);
		dfaState = DFA_END;
		break;
	case DFA_TERM_STRING:
		Lexer->Token.Type = SILIKO_TOK_ID;
		Lexer->Token.Id = Lex.Buffer;
		dfaState = DFA_END;
		break;
	case DFA_TERM_EOL:
		Lexer->Token.Type = SILIKO_TOK_EOL;
		Lexer->Token.Integer = 0;
		free(Lex.Buffer);
		dfaState = DFA_END;
		break;
	case DFA_ERROR:
		Lexer->Token.Type = SILIKO_TOK_ERROR;
		Lexer->Token.Integer = 0;
		free(Lex.Buffer);
		dfaState = DFA_END;
		break;
	}
}

SilikoLexer *SilikoLexerNew(SilikoDataSource *InputSource)
{
	SilikoLexer *rVal = NULL;

	if (!(rVal = malloc(sizeof(SilikoLexer))))
		return NULL;

	rVal->Source = InputSource;
	rVal->Token.Type = SILIKO_TOK_UNSET;
	rVal->Token.Integer = 0;
	SilikoLexerNext(rVal);

	return rVal;
}

void SilikoLexerDelete(SilikoLexer *Lexer)
{
	if (Lexer)
	{
		if (Lexer->Token.Type == SILIKO_TOK_ID)
			free(Lexer->Token.Id);
		SilikoDataSourceDelete(Lexer->Source);
		free(Lexer);
	}
}

SilikoToken SilikoLexerGetToken(SilikoLexer *Lexer)
{
	return Lexer->Token;
}
