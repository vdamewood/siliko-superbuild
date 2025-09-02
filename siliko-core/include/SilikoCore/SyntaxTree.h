/* SyntaxTree.h: Functions to manipulate abstract syntax trees
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

#if !defined SILIKO_SYNTAX_TREE_H
#define SILIKO_SYNTAX_TREE_H

#include <SilikoCore/Api.h>
#include <SilikoCore/FunctionCaller.h>
#include <SilikoCore/Value.h>

#if defined __cplusplus
extern "C" {
#endif
enum SilikoSyntaxTreeNodeType
{
	SILIKO_AST_NOTHING = 0,
	SILIKO_AST_BRANCH,
	SILIKO_AST_LEAF
};
typedef enum SilikoSyntaxTreeNodeType SilikoSyntaxTreeNodeType;

struct SilikoSyntaxTreeNode;
typedef struct SilikoSyntaxTreeNode SilikoSyntaxTreeNode;

SILIKOCORE_EXPORT SilikoSyntaxTreeNode *SilikoSyntaxTreeNewLeaf(struct SilikoValue);
SILIKOCORE_EXPORT SilikoSyntaxTreeNode *SilikoSyntaxTreeNewFromInteger(long long int IntegerValue);
SILIKOCORE_EXPORT SilikoSyntaxTreeNode *SilikoSyntaxTreeNewFromFloat(double FloatValue);
SILIKOCORE_EXPORT SilikoSyntaxTreeNode *SilikoSyntaxTreeNewBranch(char *Id);
SILIKOCORE_EXPORT SilikoSyntaxTreeNode *SilikoSyntaxTreeNewError(void);
SILIKOCORE_EXPORT SilikoSyntaxTreeNode *SilikoSyntaxTreeNewNothing(void);
SILIKOCORE_EXPORT void SilikoSyntaxTreeDelete(SilikoSyntaxTreeNode *SyntaxTree);

SILIKOCORE_EXPORT SilikoSyntaxTreeNodeType SilikoSyntaxTreeGetType(SilikoSyntaxTreeNode *SyntaxTree);
SILIKOCORE_EXPORT int SilikoSyntaxTreeIsError(SilikoSyntaxTreeNode *SyntaxTree);

SILIKOCORE_EXPORT struct SilikoValue SilikoSyntaxTreeEvaluate(SilikoSyntaxTreeNode *SyntaxTree, SilikoFunctionCaller *Caller);
SILIKOCORE_EXPORT int SilikoSyntaxTreeNegate(SilikoSyntaxTreeNode *SyntaxTree);

SILIKOCORE_EXPORT int SilikoSyntaxTreePushLeft(SilikoSyntaxTreeNode *BaseTree, SilikoSyntaxTreeNode *NewBranch);
SILIKOCORE_EXPORT int SilikoSyntaxTreePushRight(SilikoSyntaxTreeNode *BaseTree, SilikoSyntaxTreeNode *NewBranch);

#if defined __cplusplus
}
#endif

#endif // SILIKO_SYNTAX_TREE_H
