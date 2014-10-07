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
 * Very simple "game"
 */

#include "application.h"
#include "widgets/status_bar.h"
#include <drivers/lsm303c.h>
#include <drivers/buzzer.h>
#include <drivers/vibra.h>
#include <sincos.h>
#include <stdio.h>

#define S_WIN_X0 5
#define S_WIN_Y0 25
#define S_WIN_X1 122
#define S_WIN_Y1 122
#define BALL_R 5
#define STEP 10
#define FACT_MOD 4
#define SCSTEP 500

#define MODE_NORM 0
#define MODE_HIGH 1

unsigned int win_x0, win_y0, win_x1, win_y1;
unsigned int score, highscore;
int mode;
unsigned int ball_x, ball_y;

static void game_redraw(struct ui_widget *w)
{
	char buf[30];

	gfx_clear(&w->dc, 0);
	if(mode == MODE_HIGH) {
		sprintf(buf, "Highscore:");
		gfx_centered_text(&w->dc, &font_helv17b, 30, buf, COLOR_BLACK);
		sprintf(buf, "%u", highscore);
		gfx_centered_text(&w->dc, &font_helv17, 50, buf, COLOR_BLACK);
	} else {
		sprintf(buf, "Score: %u", score);
		gfx_centered_text(&w->dc, &font_helv17, 5, buf, COLOR_BLACK);
		gfx_line(&w->dc, win_x0, win_y0, win_x0, win_y1, COLOR_BLACK);
		gfx_line(&w->dc, win_x0, win_y0, win_x1, win_y0, COLOR_BLACK);
		gfx_line(&w->dc, win_x0, win_y1, win_x1, win_y1, COLOR_BLACK);
		gfx_line(&w->dc, win_x1, win_y0, win_x1, win_y1, COLOR_BLACK);
		gfx_fill_circle(&w->dc, ball_x, ball_y, BALL_R, COLOR_BLACK);
	}
}

static void game_event(struct ui_widget *w, const struct event *evt)
{
	(void) evt;     // suppress warnings
	w->flags |= WF_DIRTY;
}

struct ui_widget game_widget = {
	game_redraw,
	game_event,
	{ 0, 0, 127, 127 },
	0,
	WF_ACTIVE | WF_VISIBLE
};

struct ui_widget game_screen = {
	NULL,
	NULL,
	{ 0, 0, 127, 127},
	0,
	WF_ACTIVE | WF_VISIBLE
};

void game_main(void *params)
{
	(void)(params);
	struct event evt;
	lsm303_smpl acc;
	int fakap = 0;
	int old_x, old_y;
	int fact;

	lsm303_init();
	buzzer_set_freq(4000);
	ball_x = 64;
	ball_y = 64;
	old_x = 64;
	old_y = 64;
	score = 0;
	highscore = 0;
	win_x0 = S_WIN_X0;
	win_x1 = S_WIN_X1;
	win_y0 = S_WIN_Y0;
	win_y1 = S_WIN_Y1;

	ui_clear();
	ui_init_widget(&game_screen);
	ui_init_widget(&game_widget);
	ui_add_widget(&game_widget);
	ui_add_child(&game_screen, &game_widget);
	ui_add_widget(&game_screen);

	/*draw screen for the first time*/
	ui_update(NULL);

	/*main loop*/
	while(1) {
		if(xQueueReceive(appQueue, &evt, 0)) {
			switch(evt.type) {
			case BUTTON_PRESSED:
				if(evt.data.button == BUT_TR) {
					vibra_disable();
					buzzer_disable();
					return;
				}
				if(evt.data.button == BUT_BR)
					mode = MODE_HIGH;
				if(evt.data.button == BUT_BL)
					mode = MODE_NORM;
			}
		}
		lsm303_get_sample(DEV_ACC, &acc);
		ball_x -= acc.x/100;
		ball_y -= acc.y/100;

		if(ball_x >= win_x1 - BALL_R) {
			ball_x = win_x1 - BALL_R;
			fakap = 1;
		}
		if(ball_x <= win_x0 + BALL_R) {
			ball_x = win_x0 + BALL_R;
			fakap = 1;
		}
		if(ball_y >= win_y1 - BALL_R) {
			ball_y = win_y1 - BALL_R;
			fakap = 1;
		}
		if(ball_y <= win_y0 + BALL_R) {
			ball_y = win_y0 + BALL_R;
			fakap = 1;
		}
		if(ball_x > win_x0+BALL_R && ball_x < win_x1-BALL_R &&
			 ball_y > win_y0+BALL_R && ball_y < win_y1-BALL_R )
			fakap = 0;

		if(fakap) {
			/* uhh.. that's bad, PUNISHMENT !!! */
			score = 0;
			win_x0 = S_WIN_X0;
			win_x1 = S_WIN_X1;
			win_y0 = S_WIN_Y0;
			win_y1 = S_WIN_Y1;
			vibra_enable();
			buzzer_enable();
			vTaskDelay(100);
			vibra_disable();
			buzzer_disable();
		} else {
			/*1. the more you move the more points you get*/
			/*2. the smaller box is, the more points you get */
			fact = (score/SCSTEP)%FACT_MOD;
			score += (fact+1)*(ABS(ball_x-old_x) + ABS(ball_y-old_y));
			if(score > highscore)
				highscore = score;
			vibra_disable();
			buzzer_disable();
			/*update box based on score*/
			win_x0 = S_WIN_X0 + fact*STEP;
			win_y0 = S_WIN_Y0 + fact*STEP;
			win_x1 = S_WIN_X1 - fact*STEP;
			win_y1 = S_WIN_Y1 - fact*STEP;
		}
		old_x = ball_x;
		old_y = ball_y;
		ui_update(&evt);
	}
}

application game = {
	.name = "Ball game",
	.main = game_main
};
