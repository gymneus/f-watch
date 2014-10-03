/*
 * Copyright (C) 2014 Julian Lewis
 * @author Grzegorz Daniluk <grzegorz.daniluk@cern.ch>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * Compass application.
 */

#include "application.h"
#include "widgets/status_bar.h"
#include <drivers/lsm303c.h>
#include <bitmaps.h>
#include <sincos.h>
#include <usbdbg.h>

#define COMP_R 50.0
#define COMP_X0 64
#define COMP_Y0 64

static int x_lcd, y_lcd;
static float sina, cosa;
static int calib;
lsm303_smpl iron;

static void compass_redraw(struct ui_widget *w)
{
	static int angle, ff;
	int arr_h, arr_w, cir_h, cir_w;
	char buf[20];

	arr_h = comp_arrow2.h;
	arr_w = comp_arrow2.w;
	cir_h = comp_circle2.h;
	cir_w = comp_circle2.w;

	//if(angle == 0) ff=1;
	//else if(angle == 180) ff=0;
	//if(ff) angle += 10;
	//else angle -= 10;
	//sina = 2; cosa = 2;
	//angle +=1;
	//if(angle == 180) angle=-180;

	gfx_clear(&w->dc, 0);
	//gfx_line(&w->dc, COMP_X0, COMP_Y0, x_lcd, y_lcd, 1);
	if(!calib) {
		gfx_draw_bitmap(&w->dc, COMP_X0-cir_w/2, COMP_Y0-cir_h/2, &comp_circle2);
		gfx_draw_bitmap_rotate(&w->dc, COMP_X0-arr_w/2, COMP_Y0-arr_h/2, &comp_arrow2,
				COMP_X0, COMP_Y0, angle, sina, cosa);
	}
	if(calib==1) {
		sprintf(buf, "Calibration");
		gfx_centered_text(&w->dc, &font_helv17, 60, buf, COLOR_BLACK);
	}
	if(calib==2) {
		sprintf(buf, "ix: %d", iron.x);
		gfx_text(&w->dc, &font_helv17, 10, 30, buf, COLOR_BLACK);
		sprintf(buf, "iy: %d", iron.y);
		gfx_text(&w->dc, &font_helv17, 10, 60, buf, COLOR_BLACK);
		sprintf(buf, "iz: %d", iron.z);
		gfx_text(&w->dc, &font_helv17, 10, 90, buf, COLOR_BLACK);
	}
}

static void compass_event(struct ui_widget *w, const struct event *evt)
{
	w->flags |= WF_DIRTY;
}

struct ui_widget compass_widget = {
	compass_redraw,
	compass_event,
	{ 0, 0, 127, 127 },
	0,
	WF_ACTIVE | WF_VISIBLE
};

struct ui_widget compass_screen = {
	NULL,
	NULL,
	{ 0, 0, 127, 127 },
	0,
	WF_ACTIVE | WF_VISIBLE
};

void compass_main(void *params)
{
	(void)(params);
	struct event evt;
	lsm303_smpl acc, mag;
	lsm303_smpl c_max, c_min;
	int pitch, roll, xh, yh, th, x_comp, y_comp;
	unsigned int x2, y2, z2;
	float xy_mean;
	char buf[50];

	/*lsm303 init, we need both ACC & MAG for tilt/roll compensation*/
	lsm303_init();

	ui_clear();
	ui_init_widget(&compass_screen);
	ui_init_widget(&compass_widget);
	ui_add_widget(&compass_widget);
	ui_add_child(&compass_screen, &compass_widget);
	ui_add_widget(&compass_screen);

	/*TODO: comment this out ?*/
	ui_init_widget(&status_bar);
	ui_add_widget(&status_bar);

	/*draw screen for the first time*/
	ui_update(NULL);

	/*main loop*/
	while(1) {
		if(xQueueReceive(appQueue, &evt, 0)) {
			switch(evt.type) {
			case BUTTON_PRESSED:
				if(evt.data.button == BUT_TR)
					return;
				if(evt.data.button == BUT_BR) {
					c_min.x = 0x7fff;
					c_min.y = 0x7fff;
					c_min.z = 0x7fff;
					c_max.x = -0x7fff;
					c_max.y = -0x7fff;
					c_max.z = -0x7fff;
					calib = 1;
				}
				if(evt.data.button == BUT_BL && calib) {
					iron = lsm303_calib_iron(&c_max, &c_min);
					calib = 2;
				}
				if(evt.data.button == BUT_TL && calib==2) {
					calib = 0;
				}
			}
		}
		else {
			lsm303_get_sample(DEV_ACC, &acc);
			lsm303_get_sample(DEV_MAG, &mag);
			if(calib==1) {
				/* we try to find min and max for axis to calibrate */
				if(mag.x > c_max.x) c_max.x = mag.x;
				if(mag.x < c_min.x) c_min.x = mag.x;
				if(mag.y > c_max.y) c_max.y = mag.y;
				if(mag.y < c_min.y) c_min.y = mag.y;
				if(mag.z > c_max.z) c_max.z = mag.z;
				if(mag.z < c_min.z) c_min.z = mag.z;
			}
			if(!calib) {
				/* calculate pitch and roll from accelerometer */
				x2 = acc.x * acc.x;
				y2 = acc.y * acc.y;
				z2 = acc.z * acc.z;
				pitch = -small_atan(acc.x, small_sqrt(acc.y*acc.y + acc.z*acc.z));
				roll = small_atan(acc.y, small_sqrt(acc.x*acc.x + acc.z*acc.z));
				sprintf(buf, "p: %d, r: %d\n\r", pitch, roll);
                usbdbg_puts(buf);
				/* work with compass readout, first revert it because magnetic south is north */
				mag.x *= -1;
				mag.y *= -1;
				mag.z *= -1;
				/*now calculate heading based on MAG readout and pinch/roll*/
				xh = (int)((float)mag.x*small_cos(pitch) + (float)mag.z*small_sin(pitch));
				yh = (int)((float)mag.x*small_sin(roll)*small_sin(pitch) + (float)mag.y*small_cos(roll) -
					(float)mag.z*small_sin(roll)*small_cos(pitch));
				/* LCD is rotated 90 degrees, so xh = yh and yh=-xh */
				th = xh;
				xh = yh;
				yh = -th;
				/* calculate x,y for compass in cartesian */
				xy_mean = small_sqrt(xh*xh + yh*yh);
				x_comp = (int)((float)COMP_R*yh / xy_mean);
				y_comp = (int)((float)COMP_R*xh / xy_mean);
				/* transform to LCD coordinates */
				x_lcd = x_comp + COMP_X0;
				y_lcd = -y_comp + COMP_Y0;
				/* we need sina and cosa to rotate compass arrow */
				sina = (float)yh/xy_mean;
				cosa = (float)xh/xy_mean;
			}
			ui_update(&evt);
		}

	}
}

application compass = {
	.name = "Compass",
	.main = compass_main
};
