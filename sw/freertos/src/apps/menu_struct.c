/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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
 * Menu structure.
 */

#include "menu_struct.h"
#include "app_list.h"

int get_menu_size(const menu_list *menu) {
    int len = 0;
    const menu_entry* ptr = menu->entries;

    // Look for sentinel
    while((*ptr++).type != END) ++len;

    return len;
}

menu_list sub_menu = {
    "Submenu",
    {
        { APP, &gps_receiving,      { .app = &clock } },
        { APP, &battery,            { .app = &clock } },
        { APP, &gps_disconnected,   { .app = &clock } },
        { END, &gps_searching,      { NULL } }
    }
};

menu_list main_menu = {
    "Main menu",
    {
       { APP,       &example_icon,      { .app = &example } },
       { APP,       &gps_receiving,     { .app = &gpscoord } },
       { APP,       &battery_charging,  { .app = &clock } },
       { SUBMENU,   NULL,               { .submenu = &sub_menu } },
       { APP,       NULL,               { .app = &reset } },
       { APP,       NULL,               { .app = &clock } },
       { APP,       &gps_disconnected,  { .app = &clock } },
       { APP,       NULL,               { .app = &clock } },
       { SUBMENU,   &battery_charging,  { .submenu = &sub_menu } },
       { APP,       NULL,               { .app = &clock } },
       { APP,       NULL,               { .app = &clock } },
       { APP,       NULL,               { .app = &clock } },
       { APP,       NULL,               { .app = &clock } },
       { SUBMENU,   NULL,               { .submenu = &sub_menu } },
       { SUBMENU,   NULL,               { .submenu = &sub_menu } },
       { SUBMENU,   NULL,               { .submenu = &sub_menu } },
       { APP,       NULL,               { .app = &clock } },
       { APP,       NULL,               { .app = &clock } },
       { END,       NULL,               { NULL } }
    }
};

