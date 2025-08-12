/* InfixParser.c: Infix notation parser
 * Copyright 2012-2025 Vincent Damewood
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

#include <stdlib.h>

#include <SilikoCore/InfixParser.h>
#include <SilikoCore/SyntaxTree.h>
#include <SilikoCore/Lexer.h>

static SilikoSyntaxTreeNode *GetExpression(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExpressionRest(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetTerm(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetTermRest(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExponent(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExponentRest(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetRoll(SilikoLexer *lexe);
static SilikoSyntaxTreeNode *GetRollRest(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetAtom(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetNumber(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetUnsignedNumber(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetFunctionCall(SilikoLexer *lexer);
static void GetArguments(SilikoLexer *lexer, SilikoSyntaxTreeNode *rVal);

static SilikoSyntaxTreeNode *GetExpression(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;

	leftValue = GetTerm(lexer);

	if (leftValue == NULL)
		return NULL;

	if (!(rest = GetExpressionRest(lexer)))
	{
		SilikoSyntaxTreeDelete(leftValue);
		return NULL;
	}

	if (SilikoSyntaxTreeGetType(rest) == SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (SilikoSyntaxTreeGraftLeft(rest, leftValue))
		return rest;

	SilikoSyntaxTreeDelete(rest);
	SilikoSyntaxTreeDelete(leftValue);
	return SilikoSyntaxTreeNewError();
}

static SilikoSyntaxTreeNode *GetExpressionRest(SilikoLexer *lexer)
{
	char *operation = NULL;
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	SilikoSyntaxTreeNode *branchNode = NULL;

	switch (SilikoLexerGetToken(lexer).Type)
	{
	case '+':
		operation = "add";
		break;
	case '-':
		operation = "subtract";
		break;
	default:
		return SilikoSyntaxTreeNewNothing();
	}

	SilikoLexerNext(lexer);
	if (!(leftValue = GetTerm(lexer))
		|| !(rest = GetExpressionRest(lexer))
		|| !(branchNode = SilikoSyntaxTreeNewBranch(operation)))
	{
		SilikoSyntaxTreeDelete(leftValue);
		SilikoSyntaxTreeDelete(rest);
		return NULL;
	}

	SilikoSyntaxTreePushRight(branchNode, NULL);
	SilikoSyntaxTreePushRight(branchNode, leftValue);

	if (SilikoSyntaxTreeGetType(rest) != SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeGraftLeft(rest, branchNode);
		return rest;
	}

	SilikoSyntaxTreeDelete(rest);
	return branchNode;
}

static SilikoSyntaxTreeNode *GetTerm(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;

	leftValue = GetExponent(lexer);

	if (leftValue == NULL)
		return NULL;

	if (!(rest = GetTermRest(lexer)))
	{
		SilikoSyntaxTreeDelete(leftValue);
		return NULL;
	}

	if (SilikoSyntaxTreeGetType(rest) == SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (SilikoSyntaxTreeGraftLeft(rest, leftValue))
		return rest;

	SilikoSyntaxTreeDelete(rest);
	SilikoSyntaxTreeDelete(leftValue);
	return SilikoSyntaxTreeNewError();
}

static SilikoSyntaxTreeNode *GetTermRest(SilikoLexer *lexer)
{
	char *operation;
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	SilikoSyntaxTreeNode *branchNode = NULL;

	switch (SilikoLexerGetToken(lexer).Type)
	{
		case '*':
			operation = "multiply";
			break;
		case '/':
			operation = "divide";
			break;
		default:
			return SilikoSyntaxTreeNewNothing();
	}

	SilikoLexerNext(lexer);
	if (!(leftValue = GetExponent(lexer))
		|| !(rest = GetTermRest(lexer))
		|| !(branchNode = SilikoSyntaxTreeNewBranch(operation)))
	{
		SilikoSyntaxTreeDelete(leftValue);
		SilikoSyntaxTreeDelete(rest);
		return NULL;
	}

	SilikoSyntaxTreePushRight(branchNode, NULL);
	SilikoSyntaxTreePushRight(branchNode, leftValue);

	if (SilikoSyntaxTreeGetType(rest) != SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeGraftLeft(rest, branchNode);
		return rest;
	}

	SilikoSyntaxTreeDelete(rest);
	return branchNode;
}

static SilikoSyntaxTreeNode *GetExponent(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	SilikoSyntaxTreeNode * rVal = NULL;

	if (!(leftValue = GetRoll(lexer)))
		return NULL;

	if (!(rest = GetExponentRest(lexer)))
	{
		SilikoSyntaxTreeDelete(leftValue);
		return NULL;
	}

	if (SilikoSyntaxTreeGetType(rest) == SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (!(rVal = SilikoSyntaxTreeNewBranch("power")))
	{
		SilikoSyntaxTreeDelete(leftValue);
		SilikoSyntaxTreeDelete(rest);
		return NULL;
	}

	SilikoSyntaxTreePushRight(rVal, leftValue);
	SilikoSyntaxTreePushRight(rVal, rest);
	return rVal;
}

static SilikoSyntaxTreeNode *GetExponentRest(SilikoLexer *lexer)
{
	if (SilikoLexerGetToken(lexer).Type == '^')
	{
		SilikoLexerNext(lexer);

		switch (SilikoLexerGetToken(lexer).Type)
		{
		case SILIKO_TOK_INTEGER:
		case SILIKO_TOK_FLOAT:
		case '-':
		case SILIKO_TOK_ID:
		case '(':
			return GetExponent(lexer);
		default:
			return SilikoSyntaxTreeNewError();
		}
	}
	else
	{
		return SilikoSyntaxTreeNewNothing();
	}
}

static SilikoSyntaxTreeNode *GetRoll(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	SilikoSyntaxTreeNode *rVal;

	if (!(leftValue = GetAtom(lexer)))
		return NULL;

	if (!(rest = GetRollRest(lexer)))
	{
		SilikoSyntaxTreeDelete(leftValue);
		return NULL;
	}


	if (SilikoSyntaxTreeGetType(rest) == SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (!(rVal = SilikoSyntaxTreeNewBranch("dice")))
	{
		SilikoSyntaxTreeDelete(leftValue);
		SilikoSyntaxTreeDelete(rest);
		return NULL;
	}

	SilikoSyntaxTreePushRight(rVal, leftValue);
	SilikoSyntaxTreePushRight(rVal, rest);
	return rVal;
}

static SilikoSyntaxTreeNode *GetRollRest(SilikoLexer *lexer)
{
	long long int value;

	if(SilikoLexerGetToken(lexer).Type == 'd')
	{
		SilikoLexerNext(lexer);
		if (SilikoLexerGetToken(lexer).Type != SILIKO_TOK_INTEGER)
			return SilikoSyntaxTreeNewError();

		value = SilikoLexerGetToken(lexer).Integer;
		SilikoLexerNext(lexer);
		return SilikoSyntaxTreeNewFromInteger(value);
	}
	else
	{
		return SilikoSyntaxTreeNewNothing();
	}
}

static SilikoSyntaxTreeNode *GetAtom(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *value;

	switch(SilikoLexerGetToken(lexer).Type)
	{
	case '-':
	case SILIKO_TOK_INTEGER:
	case SILIKO_TOK_FLOAT:
		return GetNumber(lexer);
	case '(':
		SilikoLexerNext(lexer);
		if (!(value = GetExpression(lexer)))
			return NULL;

		if (SilikoLexerGetToken(lexer).Type != ')')
		{
			SilikoSyntaxTreeDelete(value);
			return SilikoSyntaxTreeNewError();
		}
		SilikoLexerNext(lexer);
		return value;
	case SILIKO_TOK_ID:
		return GetFunctionCall(lexer);
	default:
		return SilikoSyntaxTreeNewError();
	}
}

static SilikoSyntaxTreeNode *GetNumber(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode * UnsignedNumber = NULL;

	switch (SilikoLexerGetToken(lexer).Type)
	{
	case SILIKO_TOK_INTEGER:
	case SILIKO_TOK_FLOAT:
		return GetUnsignedNumber(lexer);
	case '-':
		SilikoLexerNext(lexer);
		if (!(UnsignedNumber = GetUnsignedNumber(lexer)))
			return NULL;

		if (!SilikoSyntaxTreeNegate(UnsignedNumber))
		{
			SilikoSyntaxTreeDelete(UnsignedNumber);
			return SilikoSyntaxTreeNewError();
		}

		return UnsignedNumber;
	default:
		return SilikoSyntaxTreeNewError();
	}
}

static SilikoSyntaxTreeNode *GetUnsignedNumber(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *rVal = NULL;

	switch (SilikoLexerGetToken(lexer).Type)
	{
	case SILIKO_TOK_INTEGER:
		rVal = SilikoSyntaxTreeNewFromInteger(SilikoLexerGetToken(lexer).Integer);
		SilikoLexerNext(lexer);
		break;
	case SILIKO_TOK_FLOAT:
		rVal = SilikoSyntaxTreeNewFromFloat(SilikoLexerGetToken(lexer).Float);
		SilikoLexerNext(lexer);
		break;
	default:
		rVal = SilikoSyntaxTreeNewError();
	}

	return rVal;
}

static SilikoSyntaxTreeNode *GetFunctionCall(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *rVal = NULL;

	if (SilikoLexerGetToken(lexer).Type != SILIKO_TOK_ID)
		return SilikoSyntaxTreeNewError();

	rVal = SilikoSyntaxTreeNewBranch(SilikoLexerGetToken(lexer).Id);
	SilikoLexerNext(lexer);

	if (SilikoLexerGetToken(lexer).Type != '(')
	{
		SilikoSyntaxTreePushRight(rVal, SilikoSyntaxTreeNewError());
		return rVal;
	}
	SilikoLexerNext(lexer);

	GetArguments(lexer, rVal);

	if (SilikoLexerGetToken(lexer).Type != ')')
	{
		SilikoSyntaxTreePushRight(rVal, SilikoSyntaxTreeNewError());
		return rVal;
	}
	SilikoLexerNext(lexer);

	return rVal;
}

static void GetArguments(SilikoLexer *lexer, SilikoSyntaxTreeNode *rVal)
{
	while(-1)
	{
		SilikoSyntaxTreeNode *Expression = GetExpression(lexer);
		SilikoSyntaxTreePushRight(rVal, Expression);

		if (SilikoSyntaxTreeIsError(Expression) || SilikoLexerGetToken(lexer).Type == ')')
		{
			break;
		}
		else if (SilikoLexerGetToken(lexer).Type != ',')
		{
			SilikoSyntaxTreePushRight(rVal, SilikoSyntaxTreeNewError());
			break;
		}
		SilikoLexerNext(lexer);
	}
}


SilikoSyntaxTreeNode *SilikoParseInfix(SilikoDataSource *Input)
{
	SilikoSyntaxTreeNode *rVal = NULL;
	SilikoLexer *lexer = NULL;

	if (!(lexer = SilikoLexerNew(Input)))
		return NULL;

	rVal = GetExpression(lexer);

	if (SilikoLexerGetToken(lexer).Type != SILIKO_TOK_EOL)
	{
		SilikoSyntaxTreeDelete(rVal);
		rVal = SilikoSyntaxTreeNewError();
	}
	SilikoLexerDelete(lexer);
	return rVal;
}
