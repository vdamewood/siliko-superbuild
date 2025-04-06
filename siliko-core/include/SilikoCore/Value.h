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

#if !defined SILIKO_VALUE_H
#define SILIKO_VALUE_H

#include <SilikoCore/Api.h>

#if defined __cplusplus
extern "C" {
#endif

enum SilikoValueStatus
{
	SILIKO_VAL_INTEGER,       /* Success: The value is an integer. */
	SILIKO_VAL_FLOAT,         /* Success: The valis is a float. */
	SILIKO_VAL_MEMORY_ERR,    /* Error: An attempt to allocate memory
				    failed. */
	SILIKO_VAL_SYNTAX_ERR,    /* Error: A syntax error was
				    encountered. */
	SILIKO_VAL_ZERO_DIV_ERR,  /* Error: Division by zero was
				    attempted. */
	SILIKO_VAL_BAD_FUNCTION,  /* Error: A function call could not be
				    resolved to a valid function. */
	SILIKO_VAL_BAD_ARGUMENTS, /* Error: A function call was made with
				    a number of arguments that the
				    function can't accept. */
	SILIKO_VAL_DOMAIN_ERR,    /* Error: A function call resulted in a
				    domain error. */
	SILIKO_VAL_RANGE_ERR      /* Error: A function call resulted in a
				    range error. */
};
typedef enum SilikoValueStatus SilikoValueStatus;

struct SilikoValue
{
	SilikoValueStatus Status;
	union
	{
		long long int Integer;
		double Float;
	};
};
typedef struct SilikoValue SilikoValue;

SILIKOCORE_EXPORT char *SilikoValueToString(SilikoValue InputValue);
SILIKOCORE_EXPORT void SilikoValueDeleteString(char *string);

#if defined __cplusplus
}
#endif

#endif /* SILIKO_VALUE_H */
