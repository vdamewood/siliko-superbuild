/* DataSource.c: Abstract interface for input data
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

#include <stdlib.h>

#include <SilikoCore/DataSource.h>

SilikoDataSource *SilikoDataSourceNew(
	void *NewState,
	int (*NewAdvanceFunction)(void *),
	char (*NewGetFunction)(void *),
	void (*NewDeleteFunction)(void *)
)
{
	SilikoDataSource *rVal = malloc(sizeof(*rVal));
	if (rVal)
	{
		rVal->State = NewState;
		rVal->AdvanceFunction = NewAdvanceFunction;
		rVal->GetFunction = NewGetFunction;
		rVal->DeleteFunction = NewDeleteFunction;
	}
	return rVal;
}

int SilikoDataSourceAdvance(SilikoDataSource *Source)
{
	return Source->AdvanceFunction(Source->State);
}

char SilikoDataSourceGet(SilikoDataSource *Source)
{
	return Source->GetFunction(Source->State);
}

void SilikoDataSourceDelete(SilikoDataSource *Source)
{
	Source->DeleteFunction(Source->State);
	free(Source);
}
