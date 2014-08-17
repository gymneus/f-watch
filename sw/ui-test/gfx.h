#ifndef __GFX_H
#define __GFX_H

#include <stdint.h>
#include <stdio.h>

#include "font.h"

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

#define COLOR_WHITE 0
#define COLOR_BLACK 1
#define COLOR_TOGGLE 2


struct rect {
    uint8_t x0, y0, x1, y1;
};

struct surface
{
    uint8_t width, height, stride;
    struct rect clip;
    int client_x, client_y;
    void *data;
};

struct rle_bitmap
{
        uint8_t w;
        uint8_t h;
        uint8_t *data;
};


static inline void gfx_set_pixel ( struct surface *surf, int x, int y, int value ) 
{
    if(x < surf->clip.x0 || x > surf->clip.x1 || y < surf->clip.y0 || y > surf->clip.y1 )
     return;
    
    x += surf->client_x;
    y += surf->client_y;

    uint8_t mask = 1 << (x & 0x07);                   
    uint8_t *p = surf->data + (y * surf->stride) + (x >> 3);
    if(value == COLOR_BLACK)
    *p |= mask;
    else if (value == COLOR_WHITE)
    *p &= ~mask;
    else
    *p ^= mask;
}

void gfx_init_surface( struct surface *surf, struct surface *parent, int x0, int y0, int w, int h );
void gfx_box( struct surface *surf, int x0, int y0, int x1, int y1, int value);
void gfx_line( struct surface *surf, int x0, int y0, int x1, int y1, int value );
void gfx_text(struct surface *surf, const struct font *font, uint8_t x, uint8_t y, const char *str);
void gfx_centered_text(struct surface *surf, const struct font *font, uint8_t y, const char *str);
int gfx_text_width( const struct font *font, const char *str );
void gfx_clear(struct surface *surf, int value);
void gfx_set_clip (struct surface *surf, int x0, int y0, int x1, int y1 );
void gfx_reset_clip (struct surface *surf );
void gfx_fill_circle(struct surface *surf, int x0, int y0, int radius, int value);
void gfx_round_box(struct surface *surf, int x0, int y0, int x1, int y1, int radius, int value);
void gfx_draw_bitmap(struct surface *surf, int x0, int y0, const struct rle_bitmap *b);

#endif
