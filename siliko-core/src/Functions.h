/* Functions.h: Built-in functions
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

#if !defined SILIKO_FUNCTIONS_H
#define SILIKO_FUNCTIONS_H

#include <SilikoCore/Value.h>

struct SilikoValue SilikoFunction_add(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_subtract(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_multiply(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_divide(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_power(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_dice(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_abs(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_acos(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_asin(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_atan(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_ceil(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_cos(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_cosh(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_exp(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_floor(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_log(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_log10(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_sin(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_sinh(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_sqrt(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_tan(int argc, struct SilikoValue *argv);
struct SilikoValue SilikoFunction_tanh(int argc, struct SilikoValue *argv);

#endif /* SILIKO_FUNCTIONS_H */
