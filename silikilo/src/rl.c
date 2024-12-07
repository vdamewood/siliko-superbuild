/* rl.c: Alternative user input code
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

char *readline(const char *prompt)
{
	size_t inputChar = 0;
	int currentPosition = 0;
	size_t bufSize = 10;
	char *rVal;
	char *newVal;

	fputs(prompt, stdout);
	fflush(stdout);

	if (!(rVal = malloc(bufSize)))
	{
		return NULL;
	}

	while(-1)
	{
		inputChar = fgetc(stdin);

		if (inputChar == EOF)
		{
			free(rVal);
			rVal = NULL;
			break;
		}
		else if (inputChar == '\n')
		{
			rVal[currentPosition] = '\0';
			break;
		}

		rVal[currentPosition++] = (char) inputChar;

		if(currentPosition == bufSize)
		{
			bufSize += 10;
			newVal = realloc(rVal, bufSize);
			if (newVal)
			{
				rVal = newVal;
			}
			else
			{
				free(rVal);
				rVal = NULL;
				break;
			}
		}
	}

	return rVal;
}

void add_history(const char *command)
{
}
