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
 * Menu structure.
 */

#include "menu_struct.h"
#include "app_list.h"
#include "settings/settings.h"

int get_menu_size(const menu_list *menu) {
    int len = 0;
    const menu_entry* ptr = menu->entries;

    // Look for sentinel
    while((*ptr++).type != END) ++len;

    return len;
}

menu_list gps_settings_menu = {
    "GPS settings",
    {
        { SETTING,  NULL,               { .setting = &setting_gps_on } },
        { SETTING,  NULL,               { .setting = &setting_coord_style } },
        { SETTING,  NULL,               { .setting = &setting_gps_sets_time } },
        { APP,      &clock_icon,        { .app = &set_gmt_ofs } },
        { APP,      &clock_icon,        { .app = &set_time_fr_gps } },
        { END,      NULL,               { NULL } }
    }
};

menu_list settings_menu = {
    "Settings",
    {
        { APP,      &clock_icon,        { .app = &set_time } },
        { APP,      &date_icon,         { .app = &set_date } },
        { SUBMENU,  &gps_receiving,     { .submenu = &gps_settings_menu } },
        { END,      NULL,               { NULL } }
    }
};

menu_list main_menu = {
    "Main menu",
    {
       { APP,       &example_icon,      { .app = &example } },
       { APP,       &comp_ico,          { .app = &compass } },
       { APP,       &game_ico,          { .app = &game} },
       { APP,       &gps_receiving,     { .app = &gpscoord} },
       { SUBMENU,   &settings_icon,     { .submenu = &settings_menu } },
#ifndef DEBUG
       { APP,       &usb_ms_icon,       { .app = &usb_ms } },
#endif /* DEBUG */
       { APP,       NULL,               { .app = &reset} },
       { END,       NULL,               { NULL } }
    }
};

