/*
 * popup.h
 *
 *  Created on: Jan 7, 2017
 *      Author: john
 */

#ifndef POPUP_H_
#define POPUP_H_

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gio/gio.h>
#include <stdbool.h>

// Put the data that we want to be persistent in a struct so that it can be passed
// into a callback function as the argument gpointer user_data and then dissected
// inside the function
struct PopupData
{
	GtkWidget *level_bar;
	GtkWidget *window;
	bool isHidden;
};
struct PopupData* build_popup_window(gdouble max_value, gdouble init_value);

#endif /* POPUP_H_ */
