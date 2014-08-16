/*
 * Copyright (C) 2014 Julian Lewis
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch> 
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

#include "gfx.h"
#include "ui.h"
#include <drivers/lcd.h>

#include <stdlib.h>

#define DBG

struct surface screen;

static struct ui_widget *widget_list = NULL;

void ui_init() {
    gfx_init_surface(&screen, NULL, 0, 0, LCD_WIDTH, LCD_HEIGHT);
}

void ui_add_widget(struct ui_widget *w) {
    w->next = NULL;

    if(!widget_list) {
        widget_list = w;
    } else {
        struct ui_widget *wl;

        // find the last widget (the ending semicolon in the for loop is ok)
        for(wl = widget_list; wl->next; wl = wl->next);
        wl->next = w;
    }

    if(!(w->flags & WF_DOUBLE_BUFFER)) {
        gfx_init_surface(&w->dc, &screen, w->pos.x0, w->pos.y0, 
                         w->pos.x1 - w->pos.x0 + 1, w->pos.y1 - w->pos.y0 + 1);
    }
    else {
        DBG("NonImpl\n");
    }

    w->flags |= WF_DIRTY;
}

static int update_widget(struct ui_widget *w, struct event evt) {
    if((w->flags & WF_ACTIVE) && (w->event)) {
        w->event(w, evt);
    }

    if((w->flags & WF_VISIBLE) && (w->flags & WF_DIRTY))
    {
        if(w->redraw) {
            w->redraw(w);
            w->flags &= ~WF_DIRTY;
        }

        return 1;
    }

    return 0;
}

void draw_surface(struct surface *surf)
{
    int y, x;
    lcd_clear();

    // TODO that sucks a little bit..
    for(y = 0; y < surf->height; y++) {
        for(x = 0; x < surf->width; x++) {
            uint8_t mask = 1 << (x & 0x07);
            uint8_t *p = surf->data + (y * surf->stride) + (x >> 3);

            lcd_set_pixel(x, y, (*p & mask));
        }
    }

    lcd_update();
}

void ui_update(struct event evt) {
    int screen_dirty = 0;
    struct ui_widget *w;

    for(w = widget_list; w; w = w->next)
    {
        screen_dirty |= update_widget(w, evt);
    }

    if(screen_dirty)
        draw_surface(&screen);
}

/*void ui_activate(struct ui_widget *w, int activate) {
}

void ui_show(struct ui_widget *w, int show) {
}

void ui_set_modal(struct ui_widget *w, int modal) {
}*/

void ui_init_widget(struct ui_widget *w) {
    w->n_children = 0;
}

void ui_add_child(struct ui_widget *w, struct ui_widget *child) {
    w->children[ w->n_children ] = child;
    w->flags |= WF_DIRTY;
    w->n_children++;
}

