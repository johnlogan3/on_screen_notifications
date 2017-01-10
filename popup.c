/*
 * popup.c
 *
 *  Created on: Jan 7, 2017
 *      Author: john
 */

#include "popup.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gio/gio.h>

struct PopupData* build_popup_window(gdouble max_value, gdouble init_value)
{
	//GtkWidget *level_bar = malloc(sizeof(GtkWidget));
	//GtkWidget *window = malloc(sizeof(GtkWidget));
	GtkWidget *grid;
	GtkWidget *label;

	struct PopupData *data = malloc(sizeof(struct PopupData));
	//data->window = window;
	//data->level_bar = level_bar;

	//data->window = gtk_application_window_new (app);
	// there should be some way to mark the window as a popup, but this feels wrong.
	data->window = gtk_window_new (GTK_WINDOW_POPUP);
	gtk_window_set_title (GTK_WINDOW (data->window), "Window");
	gtk_window_set_default_size (GTK_WINDOW (data->window), 200, 100);
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (data->window), TRUE);
	gtk_window_set_decorated (GTK_WINDOW (data->window), FALSE);
	gtk_window_set_keep_above (GTK_WINDOW (data->window), TRUE);
	gtk_window_set_position (GTK_WINDOW (data->window), GTK_WIN_POS_CENTER);

	grid = gtk_grid_new ();
	gtk_container_add (GTK_CONTAINER (data->window), grid);

	label = gtk_label_new ("Brightness");
	gtk_widget_set_margin_top (GTK_WIDGET (label), 25);
	gtk_widget_set_margin_bottom (GTK_WIDGET (label), 25);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 2, 1);

	// initialize the widget into the global struct and access it from there
	// rather than the local pointer that gets lost when the function exits
	data->level_bar =  gtk_level_bar_new ();
	gtk_level_bar_set_min_value (GTK_LEVEL_BAR (data->level_bar), 0);
	gtk_level_bar_set_max_value (GTK_LEVEL_BAR (data->level_bar), max_value);
	gtk_level_bar_set_value (GTK_LEVEL_BAR (data->level_bar), init_value);
	gtk_widget_set_hexpand (GTK_WIDGET (data->level_bar), TRUE);
	gtk_widget_set_vexpand (GTK_WIDGET (data->level_bar), TRUE);
	gtk_widget_set_margin_start (GTK_WIDGET (data->level_bar), 10);
	gtk_widget_set_margin_top (GTK_WIDGET (data->level_bar), 10);
	gtk_widget_set_margin_end (GTK_WIDGET (data->level_bar), 10);
	gtk_grid_attach (GTK_GRID (grid), data->level_bar, 0, 0, 2, 1);

	return data;
}
