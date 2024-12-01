/* Token.h: Token used by lexical analyzer
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

#if !defined SILIKO_TOKEN_H
#define SILIKO_TOKEN_H

#include <SilikoCore/Api.h>

#if defined __cplusplus
extern "C" {
#endif

enum SilikoTokenType
{
	SILIKO_TOK_ERROR = -1,
	SILIKO_TOK_UNSET = 0,
	SILIKO_TOK_LPAREN = '(',
	SILIKO_TOK_RPAREN = ')',
	SILIKO_TOK_MULTIPLY = '*',
	SILIKO_TOK_ADDITION = '+',
	SILIKO_TOK_COMMA = ',',
	SILIKO_TOK_SUBTRACT = '-',
	SILIKO_TOK_DIVISION = '/',
	SILIKO_TOK_EXPONENT = '^',
	SILIKO_TOK_DICE = 'd',
	SILIKO_TOK_INTEGER = 256,
	SILIKO_TOK_FLOAT,
	SILIKO_TOK_ID,
	SILIKO_TOK_EOL
};
typedef enum SilikoTokenType SilikoTokenType;

struct SilikoToken
{
	SilikoTokenType Type;
	union
	{
		char *Id;
		long long int Integer;
		double Float;
	};
};
typedef struct SilikoToken SilikoToken;

#if defined __cplusplus
}
#endif

#endif /* SILIKO_TOKEN_H */
