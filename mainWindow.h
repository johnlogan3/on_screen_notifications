/*
 * mainWindow.h
 *
 *  Created on: Jan 8, 2017
 *      Author: john
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

struct config
{
	char brightness_loc[64];
	char max_brightness_loc[64];
};

struct ActivateData
{
	struct BrightnessData* brightnessdata;
	struct config* config;
};

#endif /* MAINWINDOW_H_ */
