#include <string.h>

#include "gfx.h"

#define GFX_POOL_SIZE 4096

static uint32_t gfx_pool_mem[GFX_POOL_SIZE/4];
static int gfx_pool_pos = 0;

void *gfx_alloc(int count)
{
    count += 3;
    count *= 4;
    count /= 4; // align to 32 bits

    if(count + gfx_pool_pos >= GFX_POOL_SIZE)
	return NULL;

    void *p = gfx_pool_mem + gfx_pool_pos;
    gfx_pool_pos += count;
    return p;
}

static struct widget *widget_list;

void gfx_init_surface( struct surface *surf, struct surface *parent, int x0, int y0, int w, int h )
{
    if(!parent)
    {
            surf->width = w;
            surf->height = h;
            surf->clip.x0 = 0;
            surf->clip.y0 = 0;
            surf->clip.x1 = w - 1;
            surf->clip.y1 = h - 1;
            surf->stride = (w + 7) / 8;
            surf->data = gfx_alloc(surf->stride * h);
            memset( surf->data, 0, surf->stride * h);
    } else {
            surf->width = parent->width;
            surf->height= parent->height;
            surf->stride = parent->stride;
            surf->clip.x0 = 0;
            surf->clip.y0 = 0;
            surf->clip.x1 = w - 1;
            surf->clip.y1 = h - 1;
            surf->client_x = x0;
            surf->client_y = y0;
            surf->data = parent->data;
    }
}

void gfx_set_clip (struct surface *surf, int x0, int y0, int x1, int y1 )
{
    surf->clip.x0 = x0;
    surf->clip.y0 = y0;
    surf->clip.x1 = x1;
    surf->clip.y1 = y1;
}

void gfx_reset_clip (struct surface *surf)
{
    surf->clip.x0 = 0;
    surf->clip.y0 = 0;
    surf->clip.x1 = surf->width - 1;
    surf->clip.y1 = surf->height - 1;
}

void gfx_box( struct surface *surf, int x0, int y0, int x1, int y1, int value)
{
    uint8_t x, y;

    x0 = MAX(x0, surf->clip.x0);
    x1 = MIN(x1, surf->clip.x1);
    y0 = MAX(y0, surf->clip.y0);
    y1 = MIN(y1, surf->clip.y1);
    
    for(x = x0; x <= x1; x++)
    {
        for(y = y0; y <= y1; y++)
        {
            gfx_set_pixel(surf, x, y, value);
        }
    }
}

void gfx_clear(struct surface *surf, int value)
{
    gfx_box( surf, 0, 0, surf->width-1, surf->height-1, value);
}


void gfx_line( struct surface *surf, int x0, int y0, int x1, int y1, int value )
{
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    for(;;)
    {
        gfx_set_pixel(surf, x0, y0, value);

        if(x0 == x1 && y0 == y1)
            break;

        e2 = err;
        if(e2 > -dx) { err -= dy; x0 += sx; }
        if(e2 < dy)  { err += dx; y0 += sy; }
    }
}

static uint8_t draw_glyph(const struct font *font, struct surface *surf, uint8_t x0, uint8_t y0, char c)
{
    if(c < font->min_char || c > font->max_char)
        return 0;

    const uint8_t *buf = font->glyph_data + font->offset_table[c - font->min_char];
    uint8_t w = font->width_table[c - font->min_char];
    uint8_t x, y;

    //printf("DrawGlyph: %d %d '%c'\n", x0, y0, c);

    for(y = 0; y < font->height; y++)
    {
        for(x = 0; x < w; x++)
        {
            if(buf[((w + 7) >> 3) * y + (x >> 3)] & (1 << (7 - (x & 7))))
                gfx_set_pixel(surf, x + x0, y + y0, c);
        }
    }

    return w;
}

void gfx_text(struct surface *surf, const struct font *font, uint8_t x, uint8_t y, const char *str)
{
    char c;

    while((c = *str++))
        x += draw_glyph(font, surf, x, y, c);
}

void gfx_centered_text(struct surface *surf, const struct font *font, uint8_t y, const char *str)
{
    int w = gfx_text_width(font, str);

    gfx_text(surf, font, surf->width / 2 - w / 2, y, str);
}

int gfx_text_width( const struct font *font, const char *str)
{
    int c;
    int w = 0;
    while(c = *str++)
    {
        if(c < font->min_char || c > font->max_char)
            continue;
        const uint8_t *buf = font->glyph_data + font->offset_table[c - font->min_char];
        w += font->width_table[c - font->min_char];
    }

    printf("Width %d\n", w);

    return w;
}



