/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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

/**
 * List of settings subapps.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include "application.h"

#include <stdint.h>

typedef struct setting {
    char name[16];
    uint16_t val;		// uint16_t to comply to eeprom_emulation code
    uint16_t nrvals;
	uint16_t fladdr;
} setting_t;

extern application set_time;
extern application set_date;
extern application set_gmt_ofs;
extern application set_time_fr_gps;

extern setting_t setting_gps_on;
extern setting_t setting_coord_style;
extern setting_t setting_gmt_ofs_hr;
extern setting_t setting_gmt_ofs_min;
extern setting_t setting_gps_sets_time;

void setting_init();
void setting_change(setting_t *setting);
void setting_apply(setting_t *setting, int val);
int  setting_get(setting_t *setting);

#endif /* SETTINGS_H */


