/* Functions.h: Built-in functions
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

#if !defined SILIKO_FUNCTIONS_H
#define SILIKO_FUNCTIONS_H

#include <SilikoCore/Value.h>

SilikoValue SilikoFunction_add(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_subtract(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_multiply(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_divide(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_power(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_dice(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_abs(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_acos(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_asin(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_atan(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_ceil(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_cos(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_cosh(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_exp(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_floor(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_log(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_log10(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_sin(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_sinh(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_sqrt(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_tan(int argc, SilikoValue *argv);
SilikoValue SilikoFunction_tanh(int argc, SilikoValue *argv);

#endif /* SILIKO_FUNCTIONS_H */
