/* SyntaxTree.c: Functions to manipulate abstract syntax trees
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

#include <stdlib.h>
#include <string.h>

#include <SilikoCore/SyntaxTree.h>
#include <SilikoCore/FunctionCaller.h>

#if defined _WIN32
#define strdup _strdup
#endif

struct SilikoSyntaxTreeBranch
{
	char *Id;
	int Count;
	int Capacity;
	int IsNegated;
	SilikoSyntaxTreeNode **Children;
};
typedef struct SilikoSyntaxTreeBranch SilikoSyntaxTreeBranch;

struct SilikoSyntaxTreeNode
{
	SilikoSyntaxTreeNodeType Type;
	union
	{
		SilikoValue Leaf;
		SilikoSyntaxTreeBranch *Branch;
	};
};

SilikoSyntaxTreeNode *SilikoSyntaxTreeNewError(void)
{
	SilikoSyntaxTreeNode *rVal = NULL;

	if ((rVal = malloc(sizeof(SilikoSyntaxTreeNode))))
	{
		rVal->Type = SILIKO_AST_LEAF;
		rVal->Leaf.Status = SILIKO_VAL_SYNTAX_ERR;
	}

	return rVal;
}

SilikoSyntaxTreeNode *SilikoSyntaxTreeNewNothing(void)
{
	SilikoSyntaxTreeNode *rVal = NULL;

	if ((rVal = malloc(sizeof(SilikoSyntaxTreeNode))))
		rVal->Type = SILIKO_AST_NOTHING;

	return rVal;
}

SilikoSyntaxTreeNode *SilikoSyntaxTreeNewInteger(long long int NewValue)
{
	SilikoSyntaxTreeNode *rVal = NULL;

	if ((rVal = malloc(sizeof(SilikoSyntaxTreeNode))))
	{
		rVal->Type = SILIKO_AST_LEAF;
		rVal->Leaf.Status = SILIKO_VAL_INTEGER;
		rVal->Leaf.Integer = NewValue;
	}
	return rVal;
}

SilikoSyntaxTreeNode *SilikoSyntaxTreeNewFloat(double NewValue)
{
	SilikoSyntaxTreeNode *rVal = NULL;

	if ((rVal = malloc(sizeof(SilikoSyntaxTreeNode))))
	{
		rVal->Type = SILIKO_AST_LEAF;
		rVal->Leaf.Status = SILIKO_VAL_FLOAT;
		rVal->Leaf.Float = NewValue;
	}
	return rVal;
}

SilikoSyntaxTreeNode *SilikoSyntaxTreeNewBranch(char *NewId)
{
	const int DefaultSize = 4;
	SilikoSyntaxTreeNode *rVal = NULL;
	SilikoSyntaxTreeBranch *rValBranch = NULL;
	SilikoSyntaxTreeNode **rValChildren = NULL;
	char * rValId = NULL;
	if (!(rVal = malloc(sizeof(SilikoSyntaxTreeNode)))
		|| !(rValBranch = malloc(sizeof(SilikoSyntaxTreeBranch)))
		|| !(rValChildren = calloc(DefaultSize,
								sizeof(SilikoSyntaxTreeNode*)))
		|| !(rValId = strdup(NewId)))
	{
		free(rVal);
		free(rValBranch);
		free(rValChildren);
		free(rValId);
		return NULL;
	}

	rVal->Type = SILIKO_AST_BRANCH;
	rVal->Branch = rValBranch;
	rVal->Branch->Id = rValId;
	rVal->Branch->Count = 0;
	rVal->Branch->IsNegated = 0;
	rVal->Branch->Capacity = DefaultSize;
	rVal->Branch->Children = rValChildren;

	return rVal;
}

static int ExpandChildren(SilikoSyntaxTreeNode *Tree)
{
	const int Increment = 2;
	SilikoSyntaxTreeNode **NewChildren = NULL;
	int NewCapacity = Tree->Branch->Capacity + Increment;

	if (!(NewChildren = calloc(NewCapacity, sizeof(SilikoSyntaxTreeNode*))))
		return 0;

	memcpy(NewChildren, Tree->Branch->Children, Tree->Branch->Capacity * sizeof(SilikoSyntaxTreeNode*));
	free(Tree->Branch->Children);

	Tree->Branch->Children = NewChildren;
	Tree->Branch->Capacity = NewCapacity;
	return -1;
}

int SilikoSyntaxTreePushLeft(SilikoSyntaxTreeNode *Tree, SilikoSyntaxTreeNode *NewChild)
{
	if (Tree->Branch->Count == Tree->Branch->Capacity)
		if (!ExpandChildren(Tree))
			return 0;

	for (int i = Tree->Branch->Count; i >= 1; i--)
		Tree->Branch->Children[i] = Tree->Branch->Children[i-1];

	Tree->Branch->Children[0] = NewChild;
	Tree->Branch->Count++;
	return -1;
}

int SilikoSyntaxTreePushRight(SilikoSyntaxTreeNode *Tree, SilikoSyntaxTreeNode *NewChild)
{
	if (Tree->Branch->Count == Tree->Branch->Capacity)
		if (!ExpandChildren(Tree))
			return 0;

	Tree->Branch->Children[Tree->Branch->Count] = NewChild;
	Tree->Branch->Count++;
	return -1;
}

int SilikoSyntaxTreeGraftLeft(SilikoSyntaxTreeNode *Tree, SilikoSyntaxTreeNode *NewBranch)
{
	if (Tree->Type == SILIKO_AST_BRANCH)
	{
		if (Tree->Branch->Count == 0)
		{
			return 0;
		}
		else if (Tree->Branch->Children[0] == NULL)
		{
			Tree->Branch->Children[0] = NewBranch;
			return -1;
		}
		else
		{
			return SilikoSyntaxTreeGraftLeft
				(Tree->Branch->Children[0], NewBranch);
		}
	}
	else
	{
		return 0;
	}
}

int SilikoSyntaxTreeGraftRight(SilikoSyntaxTreeNode *Tree, SilikoSyntaxTreeNode *NewBranch)
{
	if (Tree->Type == SILIKO_AST_BRANCH)
	{
		if (Tree->Branch->Count == 0)
		{
			return 0;
		}
		else if (Tree->Branch->Children[Tree->Branch->Count - 1] == NULL)
		{
			Tree->Branch->Children[Tree->Branch->Count - 1] = NewBranch;
			return -1;
		}
		else
		{
			return SilikoSyntaxTreeGraftRight(
				Tree->Branch->Children[Tree->Branch->Count - 1],
				NewBranch);
		}
	}
	else
	{
		return 0;
	}
}

SilikoSyntaxTreeNodeType SilikoSyntaxTreeGetType(SilikoSyntaxTreeNode *SyntaxTree)
{
	return SyntaxTree->Type;
}

int SilikoSyntaxTreeIsError(SilikoSyntaxTreeNode *SyntaxTree)
{
	return
		SyntaxTree->Type == SILIKO_AST_LEAF
		&& SyntaxTree->Leaf.Status == SILIKO_VAL_SYNTAX_ERR;
}

int SilikoSyntaxTreeNegate(SilikoSyntaxTreeNode *Tree)
{
	if (Tree == NULL)
		return 0;

	switch (Tree->Type)
	{
	case SILIKO_AST_LEAF:
		switch(Tree->Leaf.Status)
		{
		case SILIKO_VAL_INTEGER:
			Tree->Leaf.Integer *= -1;
			return -1;
		case SILIKO_VAL_FLOAT:
			Tree->Leaf.Float *= -1.0;
			return -1;
		default:
			return 0;
		}
	case SILIKO_AST_BRANCH:
		Tree->Branch->IsNegated = !Tree->Branch->IsNegated;
		return -1;
	default:
		return 0;
	}
}

void SilikoSyntaxTreeDelete(SilikoSyntaxTreeNode *Node)
{
	if (Node)
		if (Node->Type == SILIKO_AST_BRANCH)
		{
			free(Node->Branch->Id);
			for (int i = 0; i < Node->Branch->Count; i++)
				SilikoSyntaxTreeDelete(Node->Branch->Children[i]);
			free(Node->Branch->Children);
			free(Node->Branch);
		}
	free(Node);
}

static int IsNumber(SilikoValue n)
{
	return n.Status == SILIKO_VAL_INTEGER || n.Status == SILIKO_VAL_FLOAT;
}

static SilikoValue EvaluateBranch(SilikoSyntaxTreeBranch *Branch)
{
	SilikoValue rVal;
	SilikoValue *Arguments = NULL;

	if (Branch->Count)
	{
		if (!(Arguments =
			calloc(Branch->Count, sizeof(SilikoValue))))
		{
			rVal.Status = SILIKO_VAL_MEMORY_ERR;
			return rVal;
		}

		for(int i = 0; i < Branch->Count; i++)
		{
			Arguments[i] = SilikoSyntaxTreeEvaluate(Branch->Children[i]);
			if(!IsNumber(Arguments[i]))
			{
				rVal = Arguments[i];
				free(Arguments);
				return rVal;
			}
		}
	}

	rVal = SilikoFunctionCallerCall(Branch->Id, Branch->Count, Arguments);
	free(Arguments);

	if (Branch->IsNegated)
	{
		if (rVal.Status == SILIKO_VAL_INTEGER)
			rVal.Integer *= -1;
		else if (rVal.Status == SILIKO_VAL_FLOAT)
			rVal.Float *= -1.0;
	}

	return rVal;
}

SilikoValue SilikoSyntaxTreeEvaluate(SilikoSyntaxTreeNode *Node)
{
	SilikoValue rVal;

	if (!Node)
	{
		rVal.Status = SILIKO_VAL_SYNTAX_ERR;
		return rVal;
	}

	switch (Node->Type)
	{
	case SILIKO_AST_LEAF:
		return Node->Leaf;
	case SILIKO_AST_BRANCH:
		return EvaluateBranch(Node->Branch);
	default: // Shouldn't happen, but just in case.
		rVal.Status = SILIKO_VAL_SYNTAX_ERR;
		return rVal;
	}
}
