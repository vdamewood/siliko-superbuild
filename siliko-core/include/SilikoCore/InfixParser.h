/* InfixParser.h: Infix notation parser
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

#if !defined SILIKO_INFIX_PARSER_H
#define SILIKO_INFIX_PARSER_H

#include <SilikoCore/Api.h>
#include <SilikoCore/DataSource.h>
#include <SilikoCore/SyntaxTree.h>

#if defined __cplusplus
extern "C" {
#endif

SILIKOCORE_EXPORT SilikoSyntaxTreeNode *SilikoParseInfix(SilikoDataSource *input);

#if defined __cplusplus
}
#endif

#endif /* SILIKO_INFIX_PARSER_H */
