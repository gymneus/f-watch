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

#ifndef __UI_H
#define __UI_H

#include "gfx.h"
#include <event.h>

#define WF_DIRTY            (1<<0)
#define WF_VISIBLE          (1<<1)
#define WF_TRANSPARENT      (1<<2)
#define WF_ACTIVE           (1<<3)
#define WF_DOUBLE_BUFFER    (1<<4)

#define MAX_CHILD_WIDGETS 8

struct ui_widget {
    void (*redraw)(struct ui_widget *w);
    void (*event)(struct ui_widget *w, const struct event *evt);
    struct rect pos;
    uint8_t order;
    int flags;
    void *data;

    struct surface dc;
    struct ui_widget *next;
    struct ui_widget *children[MAX_CHILD_WIDGETS];
    int n_children;
};

extern struct surface screen;

void ui_init();
void ui_clear();
void ui_add_widget(struct ui_widget *w);
void ui_update(const struct event *evt);
//void ui_activate(struct ui_widget *w, int activate);
//void ui_show(struct ui_widget *w, int show);
//void ui_set_modal(struct ui_widget *w, int modal);
void ui_init_widget(struct ui_widget *w);
void ui_add_child(struct ui_widget *w, struct ui_widget *child);

#endif
