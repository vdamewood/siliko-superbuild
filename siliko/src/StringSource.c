/* StringSource.c: Support for reading from a string
 * Copyright 2012-2024 Vincent Damewood
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
#include <string.h>

#include <Siliko/StringSource.h>

#if defined _WIN32
#define strdup _strdup
#endif

struct SilikoStringSourceState
{
	char *string;
	char *current;
};

typedef struct SilikoStringSourceState SilikoStringSourceState;

static int SilikoStringSourceAdvance(void *State)
{
	if (((SilikoStringSourceState *)State)->current)
	{
		((SilikoStringSourceState *)State)->current++;
		return -1;
	}
	else
	{
		return 0;
	}
}

static char SilikoStringSourceGet(void *State)
{
	return *((SilikoStringSourceState *)State)->current;
}

static void SilikoStringSourceDelete(void *State)
{
	if (State)
		free(((SilikoStringSourceState *)State)->string);
	free(State);
}

SilikoDataSource *SilikoStringSourceNew(const char *NewInput)
{
	SilikoStringSourceState *state = NULL;
	SilikoDataSource *source = NULL;

	if (!(state = malloc(sizeof(SilikoStringSourceState))))
		return NULL;

	if (!(state->string = strdup(NewInput)))
	{
		free(state);
		return NULL;
	}
	state->current = state->string;

	if (!(source = SilikoDataSourceNew(
		state,
		SilikoStringSourceAdvance,
		SilikoStringSourceGet,
		SilikoStringSourceDelete)))
	{
		free(state->string);
		free(state);
		return NULL;
	}
	
	return source;
}
