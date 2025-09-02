/* FunctionCaller.c: Function handling
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

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <SilikoCore/FunctionCaller.h>
#include <SilikoCore/Value.h>

#include "Functions.h"

#if defined _WIN32
#define strdup _strdup
#endif

struct SilikoFunctionChain
{
	char *id;
	SilikoFunctionPointer function;
	struct SilikoFunctionChain *next;
};

#define TableSize 127

static uint8_t hash(const char *input)
{
	uint16_t remainder = 0;
	for (const uint8_t *i = (const uint8_t *)input; *i; i++)
        	remainder = ((remainder << 1) + *i) % TableSize;
	return remainder;
}

struct SilikoFunctionCaller
{
	struct SilikoFunctionChain *table[TableSize];
};

SilikoFunctionCaller *SilikoFunctionCallerNew()
{
	return calloc(1, sizeof(SilikoFunctionCaller));
}

void SilikoFunctionCallerDelete(SilikoFunctionCaller *Caller)
{
	for (size_t i = 0; i < sizeof(Caller->table)/sizeof(Caller->table[0]); i++)
	{
		if (Caller->table[i])
		{
			struct SilikoFunctionChain *current = Caller->table[i];
			while (current)
			{
				struct SilikoFunctionChain *next = current->next;
				free(current->id);
				free(current);
				current = next;
			}
		}
	}

	free(Caller);
}

int SilikoFunctionCallerInstall(
	SilikoFunctionCaller *Caller,
	const char *name,
	SilikoFunctionPointer function)
{
	uint8_t bucket = hash(name);
	struct SilikoFunctionChain  *newNode;

	if (!(newNode = malloc(sizeof(struct SilikoFunctionChain))))
		return 0;

	newNode->id = strdup(name);
	newNode->function = function;
	newNode->next = NULL;

	if (Caller->table[bucket])
	{
		struct SilikoFunctionChain *currentNode = Caller->table[bucket];
		while (currentNode->next)
			currentNode = currentNode->next;
		currentNode->next = newNode;
	}
	else
	{
		Caller->table[bucket] = newNode;
	}

	return -1;
}

int SilikoFunctionCallerInstallOperators(SilikoFunctionCaller *Caller)
{
	return SilikoFunctionCallerInstall(Caller, "add", SilikoFunction_add)
		&& SilikoFunctionCallerInstall(Caller, "subtract", SilikoFunction_subtract)
		&& SilikoFunctionCallerInstall(Caller, "multiply", SilikoFunction_multiply)
		&& SilikoFunctionCallerInstall(Caller, "divide", SilikoFunction_divide)
		&& SilikoFunctionCallerInstall(Caller, "power", SilikoFunction_power)
		&& SilikoFunctionCallerInstall(Caller, "dice", SilikoFunction_dice);
}

int SilikoFunctionCallerInstallFunctions(SilikoFunctionCaller *Caller)
{
	return SilikoFunctionCallerInstall(Caller, "abs", SilikoFunction_abs)
		&& SilikoFunctionCallerInstall(Caller, "acos", SilikoFunction_acos)
		&& SilikoFunctionCallerInstall(Caller, "asin", SilikoFunction_asin)
		&& SilikoFunctionCallerInstall(Caller, "atan", SilikoFunction_atan)
		&& SilikoFunctionCallerInstall(Caller, "ceil", SilikoFunction_ceil)
		&& SilikoFunctionCallerInstall(Caller, "cos", SilikoFunction_cos)
		&& SilikoFunctionCallerInstall(Caller, "cosh", SilikoFunction_cosh)
		&& SilikoFunctionCallerInstall(Caller, "exp", SilikoFunction_exp)
		&& SilikoFunctionCallerInstall(Caller, "floor", SilikoFunction_floor)
		&& SilikoFunctionCallerInstall(Caller, "log", SilikoFunction_log)
		&& SilikoFunctionCallerInstall(Caller, "log10", SilikoFunction_log10)
		&& SilikoFunctionCallerInstall(Caller, "sin", SilikoFunction_sin)
		&& SilikoFunctionCallerInstall(Caller, "sinh", SilikoFunction_sinh)
		&& SilikoFunctionCallerInstall(Caller, "sqrt", SilikoFunction_sqrt)
		&& SilikoFunctionCallerInstall(Caller, "tan", SilikoFunction_tan)
		&& SilikoFunctionCallerInstall(Caller, "tanh", SilikoFunction_tanh);
}

static SilikoFunctionPointer GetFunction(SilikoFunctionCaller *Caller, const char *name)
{
	uint8_t index = hash(name);
	struct SilikoFunctionChain *current = Caller->table[index];

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

struct SilikoValue SilikoFunctionCallerCall(
	SilikoFunctionCaller *Caller,
	const char *name,
	int argc,
	struct SilikoValue *argv)
{
	SilikoFunctionPointer f;
	struct SilikoValue rVal;

	f = GetFunction(Caller, name);

	if (!f)
		rVal.Status = SILIKO_VAL_BAD_FUNCTION;
	else
		rVal = f(argc, argv);

	return rVal;
}
