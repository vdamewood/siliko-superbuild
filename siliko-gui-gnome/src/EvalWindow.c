/* EvalWindow.c: Expression evaluation window
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

#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include <SilikoCore/FunctionCaller.h>
#include <SilikoCore/InfixParser.h>
#include <SilikoCore/StringSource.h>
#include <SilikoCore/SyntaxTree.h>
#include <SilikoCore/Value.h>

static const char GladeFile[] = "/com/vdamewood/SilikoGuiGnome/EvalWindow.ui";

static void Calculate(GtkWidget *Widget, gpointer EvalWindow)
{
	SilikoFunctionCaller *caller = g_object_get_data(G_OBJECT(EvalWindow), "Caller");

	SilikoSyntaxTreeNode *ResultTree =
		SilikoParseInfix(
		SilikoStringSourceNew(
		gtk_editable_get_text(
		GTK_EDITABLE(
		gtk_builder_get_object(GTK_BUILDER(EvalWindow), "Input")))));
	struct SilikoValue Value = SilikoSyntaxTreeEvaluate(ResultTree, caller);
	SilikoSyntaxTreeDelete(ResultTree);

	char *ResultString = SilikoValueToString(Value);
	gtk_label_set_text(
		GTK_LABEL(
			gtk_builder_get_object(
			GTK_BUILDER(EvalWindow), "Output")),
		ResultString);
	free(ResultString);
}

static gboolean Cleanup(GtkWidget *Widget, gpointer EvalWindow)
{
	SilikoFunctionCaller *caller = g_object_get_data(G_OBJECT(EvalWindow), "Caller");
	SilikoFunctionCallerDelete(caller);

	g_object_unref(EvalWindow);
	return FALSE;
}

GtkBuilder *EvalWindowNewBuilder(void)
{
	GtkBuilder *EvalWindow = gtk_builder_new_from_resource(GladeFile);

	SilikoFunctionCaller *caller = SilikoFunctionCallerNew();
	SilikoFunctionCallerInstallAllFunctions(caller);
	g_object_set_data(
		G_OBJECT(EvalWindow),
		"Caller",
		caller);

	g_signal_connect(
		gtk_builder_get_object(EvalWindow, "CalculateButton"),
		"clicked",
		G_CALLBACK(Calculate),
		EvalWindow);
	gtk_window_set_default_widget(
		GTK_WINDOW(gtk_builder_get_object(EvalWindow, "EvalWindow")),
		GTK_WIDGET(gtk_builder_get_object(EvalWindow, "CalculateButton"))
	);
	gtk_entry_set_activates_default(
		GTK_ENTRY(gtk_builder_get_object(EvalWindow, "Input")),
		TRUE
	);
	g_signal_connect(
		gtk_builder_get_object(EvalWindow, "EvalWindow"),
		"close-request",
		G_CALLBACK(Cleanup),
		EvalWindow);
	return EvalWindow;
}
