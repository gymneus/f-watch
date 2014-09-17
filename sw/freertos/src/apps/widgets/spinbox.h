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
 * @brief Spin box (to select a digit or letter).
 */

#ifndef SPINBOX_H
#define SPINBOX_H

#include <gfx/ui.h>
#include <stdbool.h>

///> Set of digits that can be used in a spinbox.
extern const char *char_digits;

///> Set of letters that can be used in a spinbox.
extern const char *char_letters;

struct spinbox {
    ///> UI widget that represents the spinbox.
    struct ui_widget widget;

    ///> Possible values to choose from.
    const char *chars;

    ///> Font used to display the value.
    const struct font *font;

    ///> Current index in the chars[] array.
    int value_idx;

    ///> Is it active spinbox?
    bool active;
};

/**
 * Function that initializes a spinbox structure. It is required to initialize
 * a spinbox structure in order to use it.
 * @param s is a pointer to the spinbox that should be initialized.
 * @param character_set is a set of characters that will be selectable in
 * the initalized spinbox.
 */
void spinbox_init_widget(struct spinbox *s, struct rect pos, const char *character_set);

/**
 * Changes the font used in the spinbox.
 * @param s is a pointer to the spinbox that will have the font changed.
 * @param f is a pointer to the new font.
 */
void spinbox_set_font(struct spinbox *s, const struct font *f);

/**
 * Returns the current value set in the spinbox. Please note that it returns
 * an ASCII character, so in case of digits you should subtract '0' to get
 * the digit.
 * @param s is a pointer to the spinbox that is queried for its value.
 * @return Currently set ASCII character.
 */
inline static char spinbox_get_value(const struct spinbox *s)
{
    return s->chars[s->value_idx];
}

/**
 * Sets value for a spinbox. If value is not available in the available set of
 * values, it will not be changed.
 * @param s is a pointer to the spinbox.
 * @param val is the new value.
 */
void spinbox_set_value(struct spinbox *s, char val);

/**
 * Changes the active property for a spinbox. By default spinbox is inactive.
 * When it is active, it reacts to button events and displays an underline to
 * indicate the active state. You can have multiple active spinboxes at the
 * same time.
 * @param s is a pointer to the spinbox to set.
 * @param active decides if it should be active or not.
 */
inline static void spinbox_set_active(struct spinbox *s, bool active)
{
    s->active = active;
    s->widget.flags |= WF_DIRTY;
}

#endif /* SPINBOX_H */
