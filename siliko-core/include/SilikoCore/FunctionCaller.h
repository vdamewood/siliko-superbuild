/* FunctionCaller.h: Function handling
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

#if !defined SILIKO_FUNCTION_CALLER_H
#define SILIKO_FUNCTION_CALLER_H

#include <SilikoCore/Api.h>
#include <SilikoCore/Value.h>

#if defined __cplusplus
extern "C" {
#endif

struct SilikoFunctionCaller;
typedef struct SilikoFunctionCaller SilikoFunctionCaller;

typedef struct SilikoValue (*SilikoFunctionPointer)(int, struct SilikoValue *);

SILIKOCORE_EXPORT SilikoFunctionCaller *SilikoFunctionCallerNew();
SILIKOCORE_EXPORT void SilikoFunctionCallerDelete(SilikoFunctionCaller *);

SILIKOCORE_EXPORT int SilikoFunctionCallerInstallOperators(SilikoFunctionCaller *Caller);
SILIKOCORE_EXPORT int SilikoFunctionCallerInstallFunctions(SilikoFunctionCaller *Caller);
SILIKOCORE_EXPORT int SilikoFunctionCallerInstallAllFunctions(SilikoFunctionCaller *Caller);
SILIKOCORE_EXPORT struct SilikoValue SilikoFunctionCallerCall(SilikoFunctionCaller *Caller, const char *FunctionName, int ArgumentCount, struct SilikoValue *Arguments);
SILIKOCORE_EXPORT int SilikoFunctionCallerInstall(SilikoFunctionCaller *Caller, const char *FunctionName, SilikoFunctionPointer Function);

#if defined __cplusplus
}
#endif

#endif // SILIKO_FUNCTION_CALLER_H
