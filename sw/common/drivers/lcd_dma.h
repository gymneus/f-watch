/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
 * @author Bartosz Bielawski <bartosz.bielawski@cern.ch>
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
 * @brief Routines for DMA transfer of the frame buffer.
 */

#ifndef LCD_DMA_H
#define LCD_DMA_H

/**
 * Initialize DMA operation.
 * Enables the clock and sets up descriptors and
 * registers for dma frame buffer transfer.
 */
void lcd_dma_init(void);

/**
 * Sends the current frame buffer to the display contents using DMA.
 */
void lcd_dma_send_frame(void);

/**
 * Checks if there is a transfer in progress.
 * @return true if DMA is currently used.
 */
bool lcd_dma_is_active(void);

#endif /* LCD_DMA_H */
