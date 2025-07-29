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
	SilikoValue Result;

	Ast = SilikoParseInfix(SilikoStringSourceNew([[self.input stringValue] UTF8String]));
	Result = SilikoSyntaxTreeEvaluate(Ast, self.caller);
	SilikoSyntaxTreeDelete(Ast);

	char *ResultCString = SilikoValueToString(Result);
	NSString *ResultString = [[NSString alloc] initWithUTF8String: ResultCString];
	free(ResultCString);
	[self.output setStringValue: ResultString];
}
@end
