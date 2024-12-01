/* FunctionCaller.c: Function handling
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

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SilikoCore/FunctionCaller.h>
#include <SilikoCore/Value.h>

#include "Functions.h"
#include "Hash.h"

#if defined _WIN32
#define strdup _strdup
#endif

struct SilikoFunctionChain
{
	char *id;
	SilikoFunctionPointer function;
	struct SilikoFunctionChain *next;
};
typedef struct SilikoFunctionChain SilikoFunctionChain;

static SilikoFunctionChain *functionTable[256];

int SilikoFunctionCallerInstall(const char *name, SilikoFunctionPointer function)
{
	int bucket = SilikoHash((const unsigned char *)name, strlen(name));
	SilikoFunctionChain  *newNode;

	if (!(newNode = malloc(sizeof(SilikoFunctionChain))))
		return 0;

	newNode->id = strdup(name);
	newNode->function = function;
	newNode->next = NULL;

	if (functionTable[bucket])
	{
		SilikoFunctionChain *currentNode = functionTable[bucket];
		while (currentNode->next)
			currentNode = currentNode->next;
		currentNode->next = newNode;
	}
	else
	{
		functionTable[bucket] = newNode;
	}

	return -1;
}

int SilikoFunctionCallerSetUp()
{
	SilikoFunctionCallerInstall("add", SilikoFunction_add);
	SilikoFunctionCallerInstall("subtract", SilikoFunction_subtract);
	SilikoFunctionCallerInstall("multiply", SilikoFunction_multiply);
	SilikoFunctionCallerInstall("divide", SilikoFunction_divide);
	SilikoFunctionCallerInstall("power", SilikoFunction_power);
	SilikoFunctionCallerInstall("dice", SilikoFunction_dice);
	SilikoFunctionCallerInstall("abs", SilikoFunction_abs);
	SilikoFunctionCallerInstall("acos", SilikoFunction_acos);
	SilikoFunctionCallerInstall("asin", SilikoFunction_asin);
	SilikoFunctionCallerInstall("atan", SilikoFunction_atan);
	SilikoFunctionCallerInstall("ceil", SilikoFunction_ceil);
	SilikoFunctionCallerInstall("cos", SilikoFunction_cos);
	SilikoFunctionCallerInstall("cosh", SilikoFunction_cosh);
	SilikoFunctionCallerInstall("exp", SilikoFunction_exp);
	SilikoFunctionCallerInstall("floor", SilikoFunction_floor);
	SilikoFunctionCallerInstall("log", SilikoFunction_log);
	SilikoFunctionCallerInstall("log10", SilikoFunction_log10);
	SilikoFunctionCallerInstall("sin", SilikoFunction_sin);
	SilikoFunctionCallerInstall("sinh", SilikoFunction_sinh);
	SilikoFunctionCallerInstall("sqrt", SilikoFunction_sqrt);
	SilikoFunctionCallerInstall("tan", SilikoFunction_tan);
	SilikoFunctionCallerInstall("tanh", SilikoFunction_tanh);
	return -1;
}

void SilikoFunctionCallerTearDown(void)
{
	for (size_t i = 0; i < sizeof(functionTable)/sizeof(functionTable[0]); i++)
	{
		if (functionTable[i])
		{
			SilikoFunctionChain *current = functionTable[i];
			while (current)
			{
				SilikoFunctionChain *next = current->next;
				free(current->id);
				free(current);
				current = next;
			}
		}
	}
	memset(functionTable, 0, sizeof(functionTable));
}

static SilikoFunctionPointer GetFunction(const char *name)
{
	int index;
	SilikoFunctionChain *current;

	index = SilikoHash((const unsigned char *)name, strlen(name));
	current = functionTable[index];

	while (current)
		if (strcmp(name, current->id) != 0)
			current = current->next;
		else
			break;

	if (current)
		return current->function;
	else
		return NULL;
}

SilikoValue SilikoFunctionCallerCall(const char *name, int argc, SilikoValue *argv)
{
	SilikoFunctionPointer f;
	SilikoValue rVal;

	f = GetFunction(name);

	if (!f)
		rVal.Status = SILIKO_VAL_BAD_FUNCTION;
	else
		rVal = f(argc, argv);

	return rVal;
}
