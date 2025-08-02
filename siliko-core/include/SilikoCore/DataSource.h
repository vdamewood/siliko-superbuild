/* DataSource.h: Abstract interface for input data
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

#if !defined SILIKO_DATA_SOURCE
#define SILIKO_DATA_SOURCE

#include <SilikoCore/Api.h>

#if defined __cplusplus
extern "C" {
#endif

struct SilikoDataSource;
typedef struct SilikoDataSource SilikoDataSource;

SILIKOCORE_EXPORT SilikoDataSource *SilikoDataSourceNew(
	void *NewState,
	int (*NewAdvanceFunction)(void *),
	char (*NewGetFunction)(void *),
	void (*NewDeleteFunction)(void *)
);
SILIKOCORE_EXPORT int SilikoDataSourceAdvance(SilikoDataSource *Source);
SILIKOCORE_EXPORT char SilikoDataSourceGet(SilikoDataSource *Source);
SILIKOCORE_EXPORT void SilikoDataSourceDelete(SilikoDataSource *Source);

#if defined __cplusplus
}
#endif

#endif /* SILIKO_DATA_SOURCE */
