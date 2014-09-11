/*
 * Copyright (C) 2014 Julian Lewis
 * @author Grzegorz Daniluk <grzegorz.daniluk@cern.ch>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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
#include <drivers/LSM303C/lsm303c.h>
#include <bitmaps.h>
#include <sincos.h>

#define COMPASS_R 50
#define COMPASS_X0 64
#define COMPASS_Y0 64

static int x_lcd, y_lcd;

static void compass_redraw(struct ui_widget *w)
{
	gfx_clear(&w->dc, 0);
	//gfx_draw_bitmap(&w->dc, 64, 64, &compass_arrow);
	gfx_line(&w->dc, COMPASS_X0, COMPASS_Y0, x_lcd, y_lcd, 1);
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
	int pitch, roll, xh, yh, x_comp, y_comp;
	unsigned int x2, y2, z2;
	float xy_mean;

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
			}
		}
		else {
			lsm303_get_sample(DEV_ACC, &acc);
			lsm303_get_sample(DEV_MAG, &mag);
			/* calculate pitch and roll from accelerometer */
			x2 = acc.x * acc.x;
			y2 = acc.y * acc.y;
			z2 = acc.z * acc.z;
			pitch = -small_atan(acc.x, small_sqrt(acc.y*acc.y + acc.z*acc.z));
			roll = small_atan(acc.y, small_sqrt(acc.x*acc.x + acc.z*acc.z));
			/* work with compass readout, first revert it because magnetic south is north */
			mag.x *= -1;
			mag.y *= -1;
			mag.z *= -1;
			/*now calculate heading based on MAG readout and pinch/roll*/
			xh = mag.x*small_cos(pitch) + mag.z*small_sin(pitch);
			yh = mag.x*small_sin(roll)*small_sin(pitch) + mag.y*small_cos(roll) -
				mag.z*small_sin(roll)*small_cos(pitch);
			/* calculate x,y for compass in cartesian */
			xy_mean = small_sqrt(xh*xh + yh*yh);
			x_comp = COMPASS_R*yh / xy_mean;
			y_comp = COMPASS_R*xh / xy_mean;
			/* transform to LCD coordinates */
			x_lcd = x_comp + COMPASS_X0;
			y_lcd = -y_comp + COMPASS_Y0;
			ui_update(&evt);
		}

	}
}

application compass = {
	.name = "Compass",
	.main = compass_main
};
