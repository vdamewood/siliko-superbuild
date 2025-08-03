/* Delegate.m: Delegate for Cocoa
 * Copyright 2012-2025 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <SilikoCore/InfixParser.h>
#include <SilikoCore/StringSource.h>
#include <SilikoCore/SyntaxTree.h>
#include <SilikoCore/Value.h>

#import "Delegate.h"

@implementation SilikoGuiDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	self.caller = SilikoFunctionCallerNew();
	SilikoFunctionCallerInstallAllFunctions(self.caller);
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	SilikoFunctionCallerDelete(self.caller);
}

- (IBAction) Calculate:(id)sender
{
	SilikoSyntaxTreeNode *Ast;
	struct SilikoValue Result;

	Ast = SilikoParseInfix(SilikoStringSourceNew([[self.input stringValue] UTF8String]));
	Result = SilikoSyntaxTreeEvaluate(Ast, self.caller);
	SilikoSyntaxTreeDelete(Ast);

	switch (Result.Status)
	{
	case (SILIKO_VAL_INTEGER):
		[self.output setIntegerValue: Result.Integer];
		break;
	case (SILIKO_VAL_FLOAT):
		[self.output setDoubleValue: Result.Float];
		break;
	case(SILIKO_VAL_MEMORY_ERR):
		[self.output setStringValue: @"Out of memory"];
		break;
	case SILIKO_VAL_SYNTAX_ERR:
		[self.output setStringValue: @"Syntax error"];
		break;
	case SILIKO_VAL_ZERO_DIV_ERR:
		[self.output setStringValue: @"Division by zero"];
		break;
	case SILIKO_VAL_BAD_FUNCTION:
		[self.output setStringValue: @"Function not found"];
		break;
	case SILIKO_VAL_BAD_ARGUMENTS:
		[self.output setStringValue: @"Bad argument count"];
		break;
	case SILIKO_VAL_DOMAIN_ERR:
		[self.output setStringValue: @"Domain error"];
		break;
	case SILIKO_VAL_RANGE_ERR:
		[self.output setStringValue: @"Range error"];
		break;
	default:
		[self.output setStringValue: @"Unexpected error"];
	}
}
@end
