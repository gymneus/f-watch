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
#include "menu_struct.h"
#include "clock.h"
#include "widgets/status_bar.h"

static int selected_item = 0;
static int menu_size = 0;

// Store states to navigate between menus
static menu_list *menu_stack[8] = { &main_menu, NULL, };
static menu_list **current_menu = &menu_stack[0];

static void menu_screen_redraw(struct ui_widget *w)
{
    const int LINE_HEIGHT = 17;
    const int LEFT_MARGIN = 17;
    int i;

    gfx_clear(&w->dc, 0);
    for(i = 0; i < menu_size; ++i)
    {
        if(i == selected_item) {
            gfx_box(&w->dc, LEFT_MARGIN, i * LINE_HEIGHT,
                    127, (i + 1) * LINE_HEIGHT, 1);
        }

        menu_entry *ent = &(*current_menu)->entries[i];
        if(ent->type == APP) {
            application *a = ent->data.app;

            gfx_text(&w->dc, &font_helv17, LEFT_MARGIN,
                     i * LINE_HEIGHT, a->name, i != selected_item);
        } else if(ent->type == SUBMENU) {
            menu_list *l = ent->data.submenu;

            gfx_text(&w->dc, &font_helv17, LEFT_MARGIN,
                     i * LINE_HEIGHT, l->name, i != selected_item);
        }
    }
}

static void menu_screen_event(struct ui_widget *w, const struct event *evt)
{
    if(evt->type == BUTTON_PRESSED) {
        if(evt->data.button == BUT_BR) {
            if(selected_item < menu_size - 1) {
                ++selected_item;
                w->flags |= WF_DIRTY;
            }
        } else if(evt->data.button == BUT_BL) {
            if(selected_item > 0) {
                --selected_item;
                w->flags |= WF_DIRTY;
            }
        }
    }
}

struct ui_widget menu_screen = {
    menu_screen_redraw,
    menu_screen_event,
    { 0, 20, 127, 107 },
    0,
    WF_ACTIVE | WF_VISIBLE
};

static void menu_ui_init() {
    ui_clear();

    ui_init_widget(&menu_screen);
    ui_add_widget(&menu_screen);

    ui_init_widget(&status_bar);
    ui_add_widget(&status_bar);

    ui_update(NULL);
}

static void run(menu_entry *entry) {
    if(entry->type == APP) {
        entry->data.app->main(NULL);
    } else if(entry->type == SUBMENU) {
        selected_item = 0;
        menu_size = 0;
        // keep the operation separate to avoid crashes
        // when an interrupt goes off between the two following lines
        *(current_menu + 1) = entry->data.submenu;
        ++current_menu;
        menu_size = get_menu_size(*current_menu);
    }

    menu_ui_init();
}

static void go_back() {
    if(current_menu == menu_stack) {
        clock.main(NULL);
    } else {
        menu_size = 0;
        selected_item = 0;
        --current_menu;
        menu_size = get_menu_size(*current_menu);
    }

    menu_ui_init();
}

void menu_main(void* params) {
    (void)(params);  // suppress unused parameter warning
    struct event evt;

    menu_size = get_menu_size(*current_menu);

    clock.main(NULL);
    menu_ui_init();

    // Once it is deactivated - display the menu
    while(1) {
        if(xQueueReceive(appQueue, &evt, 0)) {
            switch(evt.type) {
                case BUTTON_PRESSED:
                    if(evt.data.button == BUT_TL)
                        go_back();
                    else if(evt.data.button == BUT_TR)
                        // run the selected application or submenu
                        run(&(*current_menu)->entries[selected_item]);
                    else
                        ui_update(&evt);
                    break;

                default:    // suppress warnings
                    break;
            }
        }
    }
}

application menu = {
    .name = "Menu",
    .main = menu_main
};

