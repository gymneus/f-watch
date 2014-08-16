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
 * Menu application.
 */

#include "menu.h"
#include "clock.h"
#include "event.h"

#include <gfx/graphics.h>

void menu_main(void* params) {
    (void)(params);  // suppress unused parameter warning
    struct event evt;

    while( 1 ) {
        // Run the clock application as the default one
        clock.main(NULL);

        // Once it is deactivated - display the menu
        // TODO
        if(xQueueReceive(appQueue, &evt, 0)) {
            switch(evt.type) {
                default:    // suppress warnings
                break;
            }
        }
    }
}

Application menu = {
    .name = "Menu",
    .main = menu_main
};

