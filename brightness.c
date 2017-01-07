/*
 * brightness.c
 *
 *  Created on: Jan 6, 2017
 *      Author: john
 */

#include "brightness.h"
#include <stdio.h>
#include <stdlib.h>

//****************************************************************************//
// Figure out how to free the allocated data from the structs
// Figure out how to find the locations of brightness and max_brightness
// 		automatically
// Do we need to malloc for any other pointers?
//****************************************************************************//
//BUGS|
//*****
// When buttons are pressed and something is in full-screen, it seems
// to kick you out of that other thing
//***************************************************************************//

int counter = 0;
int timeout = 0;
static double shown_opacity = 0.85;

// Must declare struct outside of activate function because otherwise the memory
// location gets lost when activate exits and we SEGFAULT
struct Data d;

static int readBrightness (char fileLoc[])
{
	FILE * fp;
	int brightness;

	fp = fopen(fileLoc, "r");
	if (fp == NULL)
	{
		perror("Error in opening file");
		return -1;
	}
	fscanf(fp, "%d", &brightness);
	if (brightness < 3)
	{
		brightness=2;
	}
	printf("brightness is %d\n", brightness);
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
	if(d.isHidden==true)
	{
		d.isHidden=false;
		g_timeout_add (250, (GSourceFunc)timeout_window, &d);
	}

	int brightness;
	gtk_widget_show(d.window);
	gtk_widget_set_opacity(d.window, shown_opacity);

	brightness = readBrightness("/sys/class/backlight/intel_backlight/brightness");
	gtk_level_bar_set_value (GTK_LEVEL_BAR (d.level_bar), brightness);
	timeout=0;
	return TRUE;
}

static gboolean timeout_window(gpointer user_data)
{
	if(timeout>=2)
	{
		g_timeout_add (25, (GSourceFunc)fadeout_window, &d);
		d.isHidden = true;
		return FALSE;
	}
	timeout++;
	printf("timeout %d\n", timeout);
	//deleteData(d);
	return TRUE;
}

static gboolean fadeout_window(gpointer user_data)
{
	double opac = gtk_widget_get_opacity(d.window);
	printf("opac value %f       ", opac);
	opac-=0.05;
	if(d.isHidden==true)
	{
		if(opac < 0.05)
		{
			printf("final opac value %f\n", opac);
			gtk_widget_hide(d.window);
			printf("closing window\n");
			return FALSE;
		}
		else
		{
			gtk_widget_set_opacity(d.window, opac);
			printf("fading to %f\n", opac);
			return TRUE;
		}
	}
	else
	{	return FALSE;	}
}

static void activate (GtkApplication* app, gpointer user_data)

{
	GtkWidget *grid;
	GtkWidget *label;
	GFile *file;
	GFileMonitor *mon;
	int maxBrightness;
	int Brightness;

	maxBrightness = readBrightness
			("/sys/class/backlight/intel_backlight/max_brightness");
	Brightness = readBrightness
				("/sys/class/backlight/intel_backlight/brightness");

	d.window = gtk_application_window_new (app);
	// there should be some way to mark the window as a popup, but this feels wrong.
	d.window = gtk_window_new (GTK_WINDOW_POPUP);
	gtk_window_set_title (GTK_WINDOW (d.window), "Window");
	gtk_window_set_default_size (GTK_WINDOW (d.window), 200, 100);
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (d.window), TRUE);
	gtk_window_set_decorated (GTK_WINDOW (d.window), FALSE);
	gtk_window_set_keep_above (GTK_WINDOW (d.window), TRUE);
	gtk_window_set_position (GTK_WINDOW (d.window), GTK_WIN_POS_CENTER);
	// we have almost made it a popup, in that it doesn't appear on the taskbar
	// and it has no title bar or buttons, but it still gains focus when it
	// appears.  this seems to be unavoidable unless we change it from
	// a gtk_application_window to a regular gtk_window

	grid = gtk_grid_new ();
	gtk_container_add (GTK_CONTAINER (d.window), grid);

	label = gtk_label_new ("Brightness");
	gtk_widget_set_margin_top (GTK_WIDGET (label), 25);
	gtk_widget_set_margin_bottom (GTK_WIDGET (label), 25);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 2, 1);

	// initialize the widget into the global struct and access it from there
	// rather than the local pointer that gets lost when the function exits
	d.level_bar =  gtk_level_bar_new ();
	gtk_level_bar_set_min_value (GTK_LEVEL_BAR (d.level_bar), 0);
	gtk_level_bar_set_max_value (GTK_LEVEL_BAR (d.level_bar), maxBrightness);
	gtk_level_bar_set_value (GTK_LEVEL_BAR (d.level_bar), Brightness);
	gtk_widget_set_hexpand (GTK_WIDGET (d.level_bar), TRUE);
	gtk_widget_set_vexpand (GTK_WIDGET (d.level_bar), TRUE);
	gtk_widget_set_margin_start (GTK_WIDGET (d.level_bar), 10);
	gtk_widget_set_margin_top (GTK_WIDGET (d.level_bar), 10);
	gtk_widget_set_margin_end (GTK_WIDGET (d.level_bar), 10);
	gtk_grid_attach (GTK_GRID (grid), d.level_bar, 0, 0, 2, 1);

	// hide the window after a second if there has been no brightness manipulation
	g_timeout_add (250, (GSourceFunc)timeout_window, &d);

	// setup a file monitor of the file that gets modified when the brightness
	// keys are used, and then pass the level_bar to a callback function that
	// updates the visuals whenever the file is modified
	file = g_file_new_for_path("/sys/class/backlight/intel_backlight/brightness");
	mon = g_file_monitor(file, G_FILE_MONITOR_NONE, NULL, NULL);
	g_signal_connect (mon, "changed", G_CALLBACK (print), NULL);
	g_signal_connect (mon, "changed", G_CALLBACK (updateBar), &d);

	gtk_widget_show_all (d.window);
	gtk_widget_set_opacity (GTK_WIDGET (d.window), shown_opacity);
	// make the window semi-transparent after all the widgets are shown

	// start with the window hidden
	gtk_widget_hide (d.window);
	d.isHidden = true;

	//printf("d is %u\n", sizeof(d));
	//printf("level_bar is %u\n", sizeof(d.level_bar));
	//printf("window is %u\n", sizeof(d.window));
}

int main (int argc, char **argv)
{
	GtkApplication *app;
	int status;

	print(NULL);

	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);

	g_object_unref (app);
	//deleteData(&d);

	return status;
}

