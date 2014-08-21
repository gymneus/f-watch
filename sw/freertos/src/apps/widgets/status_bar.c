/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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

/**
 * @brief Status bar widget.
 */

#include "status_bar.h"

static void status_bar_event(struct ui_widget *w, const struct event *evt)
{
    // TODO update info about GPS signal, time, etc.
}

static void status_bar_redraw(struct ui_widget *w)
{
    gfx_round_box(&w->dc, 30, -10, 127-30, 10, 9, COLOR_BLACK);
    gfx_centered_text(&w->dc, &font_helv11, 0, "Home", 1);
}

struct ui_widget status_bar = {
    status_bar_redraw,
    status_bar_event,
    { 0, 0, 127, 15 },
    0,
    WF_ACTIVE | WF_VISIBLE
};

