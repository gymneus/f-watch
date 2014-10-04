/*
 * Copyright (C) 2014 Julian Lewis
 * @author Theodor Stana <theodor.stana@gmail.com>
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

#include "settings.h"

#include <stdio.h>
#include <string.h>

setting_t setting_gps_on        = { "GPS on",           1, 2 };
setting_t setting_coord_style   = { "Coord style",      1, 3 };
struct tm setting_gmt_ofs;
setting_t setting_gps_sets_time = { "GPS sets time",    1, 2 };

void setting_change(setting_t *setting)
{
    int v = setting->val;
    char s[16];

    v++;
    v %= setting->nrvals;

    sprintf(s, ": %d", v);

    setting->val = v;
}
