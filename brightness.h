/*
 * brightness.h
 *
 *  Created on: Jan 6, 2017
 *      Author: john
 */

#ifndef BRIGHTNESS_H_
#define BRIGHTNESS_H_

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gio/gio.h>
#include <stdbool.h>


// Put the data that we want to be persistent in a struct so that it can be passed
// into a callback function as the argument gpointer user_data and then dissected
// inside the function
struct Data
{
	GtkWidget *level_bar;
	GtkWidget *window;
	bool isHidden;
};

static int readBrightness (char fileLoc[]);
static gboolean updateBar (GFileMonitor *monitor,
							GFile *file,
							GFile *other_file,
							GFileMonitorEvent event_type,
							gpointer user_data);
static gboolean timeout_window (gpointer user_data);
static gboolean fadeout_window (gpointer user_data);


#endif /* BRIGHTNESS_H_ */
