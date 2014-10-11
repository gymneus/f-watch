/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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
 * http://www.gnu.org/licenses/gpl-3.0-standalone.html
 * or you may search the http://www.gnu.org website for the version 3 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @brief Spin box (to select a digit or letter).
 */

#include "spinbox.h"

#include <string.h>

// Default character sets.
const char *char_digits = "0123456789";
const char *char_letters = "abcdefghijklmnopqrstuvwxyz";

static void spinbox_event(struct ui_widget *w, const struct event *evt)
{
    struct spinbox *s = (struct spinbox*) w->data;

    if(s->active && evt->type == BUTTON_PRESSED) {
        if(evt->data.button == BUT_BR) {
            if(s->chars[s->value_idx + 1] != 0) {
                ++s->value_idx;
            } else {
                s->value_idx = 0;
            }
            w->flags |= WF_DIRTY;
        } else if(evt->data.button == BUT_BL) {
            if(s->value_idx > 0) {
                --s->value_idx;
            } else {
                s->value_idx = strlen(s->chars) - 1;
            }
            w->flags |= WF_DIRTY;
        }
    }
}

static void spinbox_redraw(struct ui_widget *w)
{
    struct spinbox *s = (struct spinbox*) w->data;
    const char buf[] = { spinbox_get_value(s), 0 };   // add the string terminator
    int width = gfx_text_width(s->font, buf);

    gfx_clear(&w->dc, 0);
    gfx_text(&w->dc, s->font, 0, 0, buf, 1);

    // underline
    if(s->active) {
        gfx_line(&w->dc, 0, s->font->height,
                         0 + width, s->font->height, 1);
        gfx_line(&w->dc, 0, s->font->height - 1,
                         0 + width, s->font->height - 1, 1);
    }
}

void spinbox_init_widget(struct spinbox *s, struct rect pos, const char *character_set)
{
    s->chars = character_set;
    s->value_idx = 0;
    s->active = false;

    s->widget.redraw = &spinbox_redraw;
    s->widget.event = &spinbox_event;
    s->widget.pos = pos;
    s->widget.order = 0;
    s->widget.flags = WF_ACTIVE | WF_VISIBLE;
    s->widget.data = (void*)s;
    spinbox_set_font(s, &font_helv38b);
    ui_init_widget(&s->widget);
}

void spinbox_set_font(struct spinbox *s, const struct font *f)
{
    s->font = f;
}

void spinbox_set_value(struct spinbox *s, char val)
{
    int idx = 0;

    while(s->chars[idx]) {
        if(s->chars[idx] == val) {
            s->value_idx = idx;
            break;
        }
        ++idx;
    }
}

