/* StringSource.h: Support for reading from a string
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

#if !defined SILIKO_STRING_SOURCE
#define SILIKO_STRING_SOURCE

#include <SilikoCore/DataSource.h>

#if defined __cplusplus
extern "C" {
#endif

SILIKOCORE_EXPORT SilikoDataSource *SilikoStringSourceNew(const char *InputString);

#if defined __cplusplus
}
#endif

#endif /* SILIKO_STRING_SOURCE */
