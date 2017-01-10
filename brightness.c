/*
 * brightness.c
 *
 *  Created on: Jan 6, 2017
 *      Author: john
 */

#include "popup.h"
#include "brightness.h"
#include <stdio.h>
#include <stdlib.h>

//****************************************************************************//
// Figure out how to find the locations of brightness and max_brightness
// 		automatically
// Figure out how to parse and use a config file, or maybe a config window
//
// Project Design:
//		Make brightness.c and volume.c which will contain all the necessary
//		instructions to set up the windows and callbacks and conclude in a
//		final function that can be called from main_window.c (which will also
//		set up the main application window and activate functions) which will
//		contain the main function.  Additionally break off the popup window
//		design into popup.c such that if I want to modify the design I can do
//		it from a single location.
//
//****************************************************************************//
//BUGS|
//*****
// When buttons are pressed and something is in full-screen, it seems
// to kick you out of that other thing
//***************************************************************************//

int counter;
int timeout = 0;
static double shown_opacity = 0.85;

static gdouble readBrightness (char *file_loc)
{
	FILE * fp;
	gdouble brightness;

	fp = fopen(file_loc, "r");
	if (fp == NULL)
	{
		perror("Error in opening file");
		return -1;
	}
	fscanf(fp, "%lf", &brightness);
	if (brightness < 3)
	{
		brightness=2;
	}
	printf("brightness is %f\n", brightness);
	fclose(fp);					//remember to actually close the stream
	return brightness;
}

static gboolean print (gpointer data)
{
	printf("timer called %d\n", counter);
	counter++;
	return TRUE;
}

static gboolean updateBar (GFileMonitor *monitor,
							GFile *file,
							GFile *other_file,
							GFileMonitorEvent event_type,
							gpointer user_data)
{
	struct BrightnessData *d_local_updateBar = user_data;

	if(d_local_updateBar->isHidden==true)
	{
		d_local_updateBar->isHidden=false;
		g_timeout_add (250, (GSourceFunc)timeout_window, d_local_updateBar);
	}
	gtk_widget_show(d_local_updateBar->window);
	gtk_widget_set_opacity(d_local_updateBar->window, shown_opacity);

	gdouble brightness = readBrightness(d_local_updateBar->brightness);
	gtk_level_bar_set_value (GTK_LEVEL_BAR (d_local_updateBar->level_bar), brightness);
	timeout=0;
	return TRUE;
}

static gboolean timeout_window(gpointer user_data)
{
	struct BrightnessData *d_local_timeout_window = user_data;

	if(timeout>=2)
	{
		g_timeout_add (25, (GSourceFunc)fadeout_window, d_local_timeout_window);
		d_local_timeout_window->isHidden = true;
		return FALSE;
	}
	timeout++;
	printf("timeout %d\n", timeout);
	//deleteData(d);
	return TRUE;
}

static gboolean fadeout_window(gpointer user_data)
{
	struct BrightnessData *d_local_fadeout_window = user_data;

	double opac = gtk_widget_get_opacity(d_local_fadeout_window->window);
	printf("opac value %f       ", opac);
	opac-=0.05;
	if(d_local_fadeout_window->isHidden==true)
	{
		if(opac < 0.05)
		{
			printf("final opac value %f\n", opac);
			gtk_widget_hide(d_local_fadeout_window->window);
			printf("closing window\n");
			return FALSE;
		}
		else
		{
			gtk_widget_set_opacity(d_local_fadeout_window->window, opac);
			printf("fading to %f\n", opac);
			return TRUE;
		}
	}
	else
	{	return FALSE;	}
}


// should return pointer or not?
struct BrightnessData* brightness_window(struct BrightnessData* data)
{
	GFile *file;
	GFileMonitor *mon;
	int maxBrightness;
	int Brightness;
	counter = 0;

	/*maxBrightness = readBrightness
			("/sys/class/backlight/intel_backlight/max_brightness");
	Brightness = readBrightness
				("/sys/class/backlight/intel_backlight/brightness");*/
	maxBrightness = readBrightness(data->brightness);
	Brightness = readBrightness(data->max_brightness);

	struct PopupData *d = (build_popup_window(maxBrightness, Brightness));
	data->window = d->window;
	data->level_bar = d->level_bar;

	// hide the window after a second if there has been no brightness manipulation
	//g_timeout_add (250, (GSourceFunc)timeout_window, d);

	// setup a file monitor of the file that gets modified when the brightness
	// keys are used, and then pass the level_bar to a callback function that
	// updates the visuals whenever the file is modified
	file = g_file_new_for_path("/sys/class/backlight/intel_backlight/brightness");
	mon = g_file_monitor_file(file, G_FILE_MONITOR_NONE, NULL, NULL);
	g_signal_connect (mon, "changed", G_CALLBACK (print), NULL);
	g_signal_connect (mon, "changed", G_CALLBACK (updateBar), data);

	gtk_widget_show_all (data->window);
	gtk_widget_set_opacity (GTK_WIDGET (data->window), shown_opacity);
	// make the window semi-transparent after all the widgets are shown

	// start with the window hidden
	gtk_widget_hide (data->window);
	data->isHidden = true;

	return data;
}

void brightness_free(struct BrightnessData* brightness_data)
{
	if(brightness_data != NULL)
	{
		free(brightness_data);
	}
}
