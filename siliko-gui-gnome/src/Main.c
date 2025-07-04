/* Main.c: Entry point for Siliko GUI
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

#include <gtk/gtk.h>

#include <SilikoCore/FunctionCaller.h>

#include "EvalWindow.h"

static void activate(GtkApplication *app, gpointer user_data)
{
	GtkBuilder *Builder = EvalWindowNewBuilder();
	GObject *MainWindow = gtk_builder_get_object(Builder, "EvalWindow");
	gtk_window_set_application(GTK_WINDOW(MainWindow), app);
	gtk_window_present(GTK_WINDOW(MainWindow));
}

int main(int argc, char *argv[])
{
	GtkApplication *app = gtk_application_new("com.vdamewood.SilikoGuiGnome", G_APPLICATION_DEFAULT_FLAGS);
	SilikoFunctionCallerSetUp();

	g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
	int status = g_application_run(G_APPLICATION (app), argc, argv);

	SilikoFunctionCallerTearDown();
	g_object_unref(app);

	return status;
}
