/* Lexer.h: Lexical analyzer
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

#if !defined SILIKO_LEXER_H
#define SILIKO_LEXER_H

#include <SilikoCore/Api.h>
#include <SilikoCore/DataSource.h>
#include <SilikoCore/Token.h>

#if defined __cplusplus
extern "C" {
#endif

struct SilikoLexer;
typedef struct SilikoLexer SilikoLexer;

SILIKOCORE_EXPORT SilikoLexer *SilikoLexerNew(SilikoDataSource *InputSource);
SILIKOCORE_EXPORT void SilikoLexerDelete(SilikoLexer *Lexer);

SILIKOCORE_EXPORT void SilikoLexerNext(SilikoLexer *Lexer);

SILIKOCORE_EXPORT SilikoToken SilikoLexerGetToken(SilikoLexer *Lexer);

#if defined __cplusplus
}
#endif

#endif /* SILIKO_LEXER_H */
