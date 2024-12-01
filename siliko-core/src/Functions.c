/* Functions.c: Built-in functions
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

#include "Functions.h"

SilikoValue SilikoFunction_add(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	int i;

	if (!argc)
	{
		rVal.Status = SILIKO_VAL_INTEGER;
		rVal.Integer = 0;
		return rVal;
	}

	rVal = argv[0];

	for(i = 1; i < argc; i++)
	{
		if (rVal.Status == SILIKO_VAL_FLOAT)
		{
			if (argv[i].Status == SILIKO_VAL_FLOAT)
			{
				rVal.Float += argv[i].Float;
			}
			else
			{
				rVal.Float += (double)argv[i].Integer;
			}
		}
		else
		{
			if (argv[i].Status == SILIKO_VAL_FLOAT)
			{
				rVal.Float = (double)rVal.Integer + argv[i].Float;
				rVal.Status = SILIKO_VAL_FLOAT;
			}
			else
			{
				rVal.Integer += argv[i].Integer;
			}
		}
	}

	return rVal;
}

SilikoValue SilikoFunction_subtract(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	int i;

	if (!argc)
	{
		rVal.Status = SILIKO_VAL_INTEGER;
		rVal.Integer = 0;
		return rVal;
	}

	rVal = argv[0];

	for(i = 1; i < argc; i++)
	{
		if (rVal.Status == SILIKO_VAL_FLOAT)
		{
			if (argv[i].Status == SILIKO_VAL_FLOAT)
			{
				rVal.Float -= argv[i].Float;
			}
			else
			{
				rVal.Float -= (double)argv[i].Integer;
			}
		}
		else
		{
			if (argv[i].Status == SILIKO_VAL_FLOAT)
			{
				rVal.Float = (double)rVal.Integer - argv[i].Float;
				rVal.Status = SILIKO_VAL_FLOAT;
			}
			else
			{
				rVal.Integer -= argv[i].Integer;
			}
		}
	}

	return rVal;
}

SilikoValue SilikoFunction_multiply(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	int i;

	if (!argc)
	{
		rVal.Status = SILIKO_VAL_INTEGER;
		rVal.Integer = 0;
		return rVal;
	}

	rVal = argv[0];

	for(i = 1; i < argc; i++)
	{
		if (rVal.Status == SILIKO_VAL_FLOAT)
		{
			if (argv[i].Status == SILIKO_VAL_FLOAT)
			{
				rVal.Float *= argv[i].Float;
			}
			else
			{
				rVal.Float *= (double)argv[i].Integer;
			}
		}
		else {
			if (argv[i].Status == SILIKO_VAL_FLOAT)
			{
				rVal.Float = (double)rVal.Integer * argv[i].Float;
				rVal.Status = SILIKO_VAL_FLOAT;
			}
			else
			{
				rVal.Integer *= argv[i].Integer;
			}
		}
	}

	return rVal;
}

SilikoValue SilikoFunction_divide(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	int i;

	if (argc < 2)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	rVal = argv[0];

	for (i = 1; i < argc; i++)
	{
		/* Division-by-Zero Error */
		if ((argv[i].Status == SILIKO_VAL_FLOAT && argv[i].Float == 0.0)
			|| (argv[i].Status == SILIKO_VAL_INTEGER && argv[i].Integer == 0))
		{
			rVal.Status = SILIKO_VAL_ZERO_DIV_ERR;
			return rVal;
		}

		if (rVal.Status == SILIKO_VAL_FLOAT)
		{
			if (argv[i].Status == SILIKO_VAL_FLOAT)
			{
				rVal.Float /= argv[i].Float;
			}
			else
			{
				rVal.Float /= (double) argv[i].Integer;
			}
		}
		else {
			if (argv[i].Status == SILIKO_VAL_FLOAT)
			{
				rVal.Status = SILIKO_VAL_FLOAT;
				rVal.Float = (double) rVal.Integer / argv[i].Float;
			}
			else if (rVal.Integer % argv[i].Integer == 0)
			{
				rVal.Integer /= argv[i].Integer;
			}
			else
			{
				rVal.Status = SILIKO_VAL_FLOAT;
				rVal.Float = (double) rVal.Integer / (double) argv[i].Integer;
			}
		}
	}
	return rVal;
}

SilikoValue SilikoFunction_power(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double runningValue;
	double nextValue;
	int i;

	runningValue = (argv[0].Status == SILIKO_VAL_INTEGER)
		? (double) argv[0].Integer
		: argv[0].Float;

	for (i = 1; i < argc; i++)
	{
		nextValue = (argv[i].Status == SILIKO_VAL_INTEGER)
			? (double) argv[i].Integer
			: argv[i].Float;

		runningValue = pow(runningValue, nextValue);
	}

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = runningValue;
	return rVal;
}


SilikoValue SilikoFunction_dice(int argc, SilikoValue *argv)
{
	/* TODO: Make this function handle fractional dice. */
	static int hasSeeded = 0;
	long long int runningTotal = 0;
	SilikoValue rVal;
	long long int count;
	long long int faces;
	int i;

	if(argc != 2)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	count = (argv[0].Status == SILIKO_VAL_INTEGER)
		? argv[0].Integer
		: (long long int) argv[0].Float;

	faces = (argv[1].Status == SILIKO_VAL_INTEGER)
		? argv[1].Integer
		: (long long int) argv[1].Float;

	if (!hasSeeded)
	{
		hasSeeded = 1;
		srand((unsigned int)time(NULL));
	}

	for (i = 1; i <= count; i++) runningTotal += (rand() % faces) + 1;
	rVal.Status = SILIKO_VAL_INTEGER;
	rVal.Integer = runningTotal;
	return rVal;
}

SilikoValue SilikoFunction_abs(int argc, SilikoValue *argv)
{
	SilikoValue rVal;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	rVal = argv[0];
	if (rVal.Status == SILIKO_VAL_FLOAT)
		rVal.Float = fabs(rVal.Float);
	else if (rVal.Status == SILIKO_VAL_INTEGER)
		rVal.Integer = llabs(rVal.Integer);
	return rVal;
}

SilikoValue SilikoFunction_acos(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double) argv[0].Integer;
	else
		input = argv[0].Float;

	if (input < -1 || input > 1)
	{
		rVal.Status = SILIKO_VAL_DOMAIN_ERR;
		return rVal;
	}

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = acos(input);
	return rVal;
}

SilikoValue SilikoFunction_asin(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double) argv[0].Integer;
	else
		input = argv[0].Float;

	if (input < -1 || input > 1)
	{
		rVal.Status = SILIKO_VAL_DOMAIN_ERR;
		return rVal;
	}

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = asin(input);
	return rVal;
}

SilikoValue SilikoFunction_atan(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double) argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = atan(input);
	return rVal;
}

SilikoValue SilikoFunction_ceil(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;
	double result;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double)argv[0].Integer;
	else
		input = argv[0].Float;

	result = ceil(input);
	if (result <= INT_MAX && result >= INT_MIN)
	{
		rVal.Status = SILIKO_VAL_INTEGER;
		rVal.Integer = (long long int)result;
	}
	else
	{
		rVal.Status = SILIKO_VAL_FLOAT;
		rVal.Float = result;
	}
	return rVal;
}

SilikoValue SilikoFunction_cos(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = cos(input);
	return rVal;
}

SilikoValue SilikoFunction_cosh(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = cosh(input);
	return rVal;
}

SilikoValue SilikoFunction_exp(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = exp(input);
	return rVal;
}

SilikoValue SilikoFunction_floor(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;
	double result;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double)argv[0].Integer;
	else
		input = argv[0].Float;

	result = floor(input);
	if (result <= INT_MAX && result >= INT_MIN)
	{
		rVal.Status = SILIKO_VAL_INTEGER;
		rVal.Integer = (long long int)result;
	}
	else
	{
		rVal.Status = SILIKO_VAL_FLOAT;
		rVal.Float = result;
	}
	return rVal;
}

SilikoValue SilikoFunction_log(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = log(input);
	return rVal;
}

SilikoValue SilikoFunction_log10(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = (double) log10(input);
	return rVal;
}

SilikoValue SilikoFunction_sin(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = sin(input);
	return rVal;
}

SilikoValue SilikoFunction_sinh(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = sinh(input);
	return rVal;
}

SilikoValue SilikoFunction_sqrt(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double inVal;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if ((argv[0].Status == SILIKO_VAL_INTEGER && argv[0].Integer < 0)
		|| (argv[0].Status == SILIKO_VAL_FLOAT && argv[0].Float < 0.0))
	{
			rVal.Status = SILIKO_VAL_DOMAIN_ERR;
			return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_FLOAT)
	{
		inVal = argv[0].Float;
	}
	else
	{
		inVal = (double) argv[0].Integer;
	}

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = sqrt(inVal);

	return rVal;
}

SilikoValue SilikoFunction_tan(int argc, SilikoValue *argv)
{
	// For some reason the C version of tan() doesn't throw an error for
	// input of pi/2 or 3*pi/2. Probably due to the imprecision of
	// floating point numbers.
	SilikoValue rVal;
	double input;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER) input = (double)argv[0].Integer;
	else input = argv[0].Float;

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = tan(input);
	return rVal;
}

SilikoValue SilikoFunction_tanh(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	double input;

	if (argc != 1)
	{
		rVal.Status = SILIKO_VAL_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == SILIKO_VAL_INTEGER)
		input = (double)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = SILIKO_VAL_FLOAT;
	rVal.Float = tanh(input);
	return rVal;
}
