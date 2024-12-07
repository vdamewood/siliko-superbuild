/* main.c: Entry point for Silikilo
 * Copyright 2012-2021 Vincent Damewood
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
#include <SilikoCore/Value.h>
#include <SilikoCore/StringSource.h>

int main(int argc, char *argv[])
{
	SilikoValue value;
	SilikoSyntaxTreeNode *tree;
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

	SilikoFunctionCallerSetUp();

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
		old_expression = NULL;

		tree = SilikoParseInfix(
			SilikoStringSourceNew(expression));
		value = SilikoSyntaxTreeEvaluate(tree);
		SilikoSyntaxTreeDelete(tree);

		char *ResultString = SilikoValueToString(value);
		printf("%s\n", ResultString);
		free(ResultString);

		old_expression = expression;
	}

	SilikoFunctionCallerTearDown();

	if (ISATTY())
	{
		fputc('\n', stdout);
		fflush(stdout);
	}
	return 0;
}
