/* Value.c: Data structure to represent a single value
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

#if defined _WIN32 && defined _MSC_VER
 #define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SilikoCore/Value.h>

#if defined _WIN32
#define strdup _strdup
#endif

char *SilikoValueToString(SilikoValue InputValue)
{
	size_t size;
	char * rValue;
	switch (InputValue.Status)
	{
	case (SILIKO_VAL_INTEGER):
		size = snprintf(NULL, 0, "%lli", InputValue.Integer) + 1;
		rValue = malloc(size);
		snprintf(rValue, size, "%lli", InputValue.Integer);
		break;
	case (SILIKO_VAL_FLOAT):
		size = snprintf(NULL, 0, "%f", InputValue.Float) + 1;
		rValue = malloc(size);
		snprintf(rValue, size, "%f", InputValue.Float);
		break;
	case(SILIKO_VAL_MEMORY_ERR):
		rValue = strdup("Error: Out of memory");
		break;
	case SILIKO_VAL_SYNTAX_ERR:
		rValue = strdup("Error: Syntax error");
		break;
	case SILIKO_VAL_ZERO_DIV_ERR:
		rValue = strdup("Error: Division by zero");
		break;
	case SILIKO_VAL_BAD_FUNCTION:
		rValue = strdup("Error: Function not found");
		break;
	case SILIKO_VAL_BAD_ARGUMENTS:
		rValue = strdup("Error: Bad argument count");
		break;
	case SILIKO_VAL_DOMAIN_ERR:
		rValue = strdup("Error: Domain error");
		break;
	case SILIKO_VAL_RANGE_ERR:
		rValue = strdup("Error: Range error");
	}
	return rValue;
}

void SilikoValueDeleteString(char *string)
{
	free(string);
}
