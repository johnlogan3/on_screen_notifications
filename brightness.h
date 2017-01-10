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

struct BrightnessData
{
	GtkWidget *level_bar;
	GtkWidget *window;
	bool isHidden;
	char* brightness;
	char* max_brightness;
};
static gdouble readBrightness (char fileLoc[]);
static gboolean print ();
static gboolean updateBar (GFileMonitor *monitor,
							GFile *file,
							GFile *other_file,
							GFileMonitorEvent event_type,
							gpointer user_data);
static gboolean timeout_window (gpointer user_data);
static gboolean fadeout_window (gpointer user_data);
struct BrightnessData* brightness_window(struct BrightnessData*);
void brightness_free (struct BrightnessData* brightness_data);

#endif /* BRIGHTNESS_H_ */
