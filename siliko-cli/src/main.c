/* main.c: Entry point for Siliko CLI
 * Copyright 2012-2025 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined USE_UNIX
#	include <unistd.h>
#	define ISATTY() (isatty(fileno(stdin)) && isatty(fileno(stdout)))
#else
#	define ISATTY() (-1)
#endif

#if HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#else
char *readline(const char *);
void add_history(const char *);
#endif /* HAVE_READLINE */

#include <SilikoCore/FunctionCaller.h>
#include <SilikoCore/InfixParser.h>
#include <SilikoCore/StringSource.h>
#include <SilikoCore/SyntaxTree.h>
#include <SilikoCore/Value.h>

int main(int argc, char *argv[])
{
	const char *prompt;
	const char *response;

	if (ISATTY())
	{
		prompt = "> ";
		response = "= ";
	}
	else
	{
		prompt = "";
		response = "";
	}

	SilikoFunctionCaller *caller = SilikoFunctionCallerNew();
	SilikoFunctionCallerInstallAllFunctions(caller);

	char *expression = NULL;
	char *old_expression = NULL;

	while(-1)
	{
		expression = readline(prompt);

		if(!expression)
		{
			free(old_expression);
			old_expression = NULL;
			break;
		}

		if(*expression && (!old_expression || strcmp(expression, old_expression) != 0))
			add_history(expression);

		free(old_expression);
		old_expression = expression;

		SilikoSyntaxTreeNode *tree = SilikoParseInfix(
			SilikoStringSourceNew(expression));
		struct SilikoValue value = SilikoSyntaxTreeEvaluate(tree, caller);
		SilikoSyntaxTreeDelete(tree);

		switch (value.Status)
		{
		case (SILIKO_VAL_INTEGER):
			printf("%lli\n", value.Integer);
			break;
		case (SILIKO_VAL_FLOAT):
			printf("%f\n", value.Float);
			break;
		case(SILIKO_VAL_MEMORY_ERR):
			puts("Out of memory");
			break;
		case SILIKO_VAL_SYNTAX_ERR:
			puts("Syntax error.");
			break;
		case SILIKO_VAL_ZERO_DIV_ERR:
			puts("Division by zero");
			break;
		case SILIKO_VAL_BAD_FUNCTION:
			puts("Function not found");
			break;
		case SILIKO_VAL_BAD_ARGUMENTS:
			puts("Bad argument count");
			break;
		case SILIKO_VAL_DOMAIN_ERR:
			puts("Domain error");
			break;
		case SILIKO_VAL_RANGE_ERR:
			puts("Range error");
			break;
		default:
			puts("Unexpected error");
		}
	}

	SilikoFunctionCallerDelete(caller);

	if (ISATTY())
	{
		fputc('\n', stdout);
		fflush(stdout);
	}
	return 0;
}
