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
 * http://www.gnu.org/licenses/gpl-3.0-standalone.html
 * or you may search the http://www.gnu.org website for the version 3 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include "settings.h"

#include <stdio.h>
#include <string.h>

#include <task.h>

#include <eeprom_emulation.h>

setting_t setting_gps_on        = { "GPS on",           1, 2, 0 };
setting_t setting_coord_style   = { "Coord style",      1, 3, 0 };
setting_t setting_gps_sets_time = { "GPS sets time",    1, 2, 0 };

/* Settings with nrvals == 0 do not wrap around, they are set externally */
setting_t setting_gmt_ofs_hr    = { "GMT ofs hrs",      0, 0, 0 };
setting_t setting_gmt_ofs_min   = { "GMT ofs mins",     0, 0, 0 };
setting_t setting_tracking      = { "Tracking",         0, 2, 0 };

static EE_Variable_TypeDef gps_on, coord_style, gps_sets_time,
                            gmt_ofs_hr, gmt_ofs_min, tracking;

void setting_init()
{
    /* Start adding virtual addresses to allocated variables */
    EE_DeclareVariable(&gps_on);
    EE_DeclareVariable(&coord_style);
    EE_DeclareVariable(&gps_sets_time);
    EE_DeclareVariable(&gmt_ofs_hr);
    EE_DeclareVariable(&gmt_ofs_min);
    EE_DeclareVariable(&tracking);

    /*
     * Place these virtual addresses to the RAM-stored structs, so we can pass
     * them to setting_apply() when called
     */
    setting_gps_on.fladdr = gps_on.virtualAddress;
    setting_coord_style.fladdr = coord_style.virtualAddress;
    setting_gps_sets_time.fladdr = gps_sets_time.virtualAddress;
    setting_gmt_ofs_hr.fladdr = gmt_ofs_hr.virtualAddress;
    setting_gmt_ofs_min.fladdr = gmt_ofs_min.virtualAddress;
    setting_tracking.fladdr = tracking.virtualAddress;

    /*
     * EE_Init() fails if the virtual addresses are not found in the flash. If
     * it fails, we write the default setting values. Otherwise, we apply the
     * settings in the flash to the setting values in RAM
     */
    if (!EE_Init(2)) {
        EE_Format(2);
        EE_Write(&gps_on, setting_gps_on.val);
        EE_Write(&coord_style, setting_coord_style.val);
        EE_Write(&gps_sets_time, setting_gps_sets_time.val);
        EE_Write(&gmt_ofs_hr, setting_gmt_ofs_hr.val);
        EE_Write(&gmt_ofs_min, setting_gmt_ofs_min.val);
        EE_Write(&tracking, setting_tracking.val);
    } else {
        EE_Read(&gps_on, &setting_gps_on.val);
        EE_Read(&coord_style, &setting_coord_style.val);
        EE_Read(&gps_sets_time, &setting_gps_sets_time.val);
        EE_Read(&gmt_ofs_hr, &setting_gmt_ofs_hr.val);
        EE_Read(&gmt_ofs_min, &setting_gmt_ofs_min.val);
        EE_Read(&tracking, &setting_tracking.val);
    }
}

void setting_change(setting_t *setting)
{
    int v = setting->val;

    v++;
    v %= setting->nrvals;

    setting_apply(setting, v);
}

void setting_apply(setting_t *setting, int val)
{
    EE_Variable_TypeDef eevar;
    eevar.virtualAddress = setting->fladdr;

    taskENTER_CRITICAL();
    setting->val = val;
    EE_Write(&eevar, val);
    taskEXIT_CRITICAL();
}

int setting_get(setting_t *setting)
{
    return setting->val;
}
