/*
 * mainWindow.c
 *
 *  Created on: Jan 8, 2017
 *      Author: john
 */

#include "mainWindow.h"
#include "brightness.h"
#include "popup.h"
#include <stdlib.h>
#include <stdio.h>

struct BrightnessData* brightness_data;

struct config read_config_file ()
{
	FILE *fp;
	struct config output;

	fp = fopen("on_screen_notifications.conf", "r");
	if (fp == NULL)
	{
		perror("Error in opening file");
	}
	fscanf(fp, " brightness: %63s", output.brightness_loc);
	fscanf(fp, " max_brightness: %63s", output.max_brightness_loc);

	fclose(fp);					//remember to actually close the stream
	return output;
}

static void activate (GtkApplication* app, gpointer user_data)

{
	GtkWidget *window = gtk_application_window_new(app);
	gtk_widget_show(window);

	struct ActivateData *activatedata = user_data;
	struct config *config = activatedata->config;

	brightness_data = malloc(sizeof(struct BrightnessData));
	brightness_data->brightness = (char*)config->brightness_loc;
	brightness_data->max_brightness = (char*)config->max_brightness_loc;

	brightness_data = brightness_window(brightness_data);
}

int main (int argc, char **argv)
{
	GtkApplication *app = malloc(sizeof(GtkApplication));
	int status;

	struct config config = read_config_file();

	struct ActivateData* activatedata = malloc(sizeof(struct ActivateData));
	activatedata->config = &config;
	//print(NULL);

	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), activatedata);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	brightness_free(brightness_data);
	free(activatedata);

	return status;
}
