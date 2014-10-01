#include "path_widget.h"
#include <stdlib.h>
#include <stdio.h>


void draw_path(struct surface *surf, path *p)
{
        uint8_t i;
        coord min;
        coord max;
        double x_scale, y_scale;
        int8_t y_pos, y_pos_next;
        int8_t x_pos, x_pos_next;
        char buf[20];

        min.lat = p->d[0].lat;
        max.lat = p->d[0].lat;
        min.lon = p->d[0].lon;
        max.lon = p->d[0].lon;
        min.alt = p->d[0].alt;
        max.alt = p->d[0].alt;


        for(i=0; i<p->d_size; i++)
        {
                DBG("data: %f, %f, %f\n",p->d[i].lat, p->d[i].lon, p->d[i].alt);
        }

        DBG("draw_path: d_size=%d, i=%d\n",p->d_size, i);


        // Find data min/max
        for(i=0; i<p->d_size; i++){
                if(p->d[i].lat < min.lat)
                        min.lat = p->d[i].lat;
                if(p->d[i].lat > max.lat)
                        max.lat = p->d[i].lat;

                if(p->d[i].lon < min.lon)
                        min.lon = p->d[i].lon;
                if(p->d[i].lon > max.lon)
                        max.lon = p->d[i].lon;

                if(p->d[i].alt < min.alt)
                        min.alt = p->d[i].alt;
                if(p->d[i].alt > max.alt)
                        max.alt = p->d[i].alt;
        }


        //find_min_max(p->d, p->d_size, p_min, p_max);
        DBG("lat: min: %f, max: %f, max-min: %f\n", min.lat, max.lat, (max.lat - min.lat));
        DBG("lon: min: %f, max: %f, max-min: %f\n", min.lon, max.lon, (max.lon - min.lon));
        DBG("alt: min: %f, max: %f, max-min: %f\n", min.alt, max.alt, (max.alt - min.alt));


        // Compute x scaling factor (rounded up by adding 1)
        x_scale = (max.lat - min.lat) / p->x_size + 1;

        // Compute y scaling factor (rounded up by adding 1)
        y_scale = (max.lon - min.lon) / p->y_size + 1;

        DBG("x_scale: %f\ny_scale: %f\n",x_scale, y_scale);

        // Rescale and plot data
        for(i=0; i<p->d_size-1; i++){
                x_pos = (p->d[i].lat - min.lat)/x_scale;
                x_pos_next = (p->d[i+1].lat - min.lat)/x_scale;
                y_pos = (p->d[i].lon - min.lon)/y_scale;
                y_pos_next = (p->d[i+1].lon - min.lon)/y_scale;
                DBG("x=%4d, y=%4d\n", x_pos, y_pos);
                if(p->line)
                        gfx_line(surf, x_pos, p->y_size-y_pos, x_pos_next, p->y_size-y_pos_next, 1);
                else
                        gfx_set_pixel(surf, x_pos, p->y_size-y_pos, 1);
        }

        // Print plot title
        DBG("title: %s",p->title);
        //gfx_text(surf, &font_xm5x8, 0, 0, p->title);
        //gfx_centered_text(surf, &font_helv11, 0, p->title);
        gfx_centered_text(surf, &font_xm4x6, p->y_size+1, p->title);

        /*
        // Print min max
        sprintf(buf,"min:%d",y_min);
        gfx_text(surf, &font_xm4x6, 1, p->y_size+1, buf);
        sprintf(buf,"max:%d",y_max);
        gfx_text(surf, &font_xm4x6, p->x_size-gfx_text_width(&font_xm4x6,buf), p->y_size+1, buf);
        */
        // Print frame, if enabled
        if(p->frame)
        {
                gfx_line(surf, 0, 0, p->x_size, 0, 1);
                gfx_line(surf, p->x_size, 0, p->x_size, p->y_size, 1);
                gfx_line(surf, p->x_size, p->y_size, 0, p->y_size, 1);
                gfx_line(surf, 0, p->y_size, 0, 0, 1);
        }

}


static void path_event(struct ui_widget *w, struct ui_event event)
{
        switch(event.type)
	{
		case EVT_NEXT_CYCLE:
			w->flags |= WF_DIRTY;
			break;
		default:
			break;
	}
}

static void path_redraw(struct ui_widget *w)
{
        #define D_SIZE 50
        coord data[D_SIZE];
        uint16_t i;
        char title[20];
        static float a=0;
        path p;
        int sign;

        #define DIV 1000000.0

        // Clear widget
        gfx_clear(&w->dc, 0);

        // Generate test data
        sprintf(title, "test path\n");

        data[0].lat = rand()/DIV;
        data[0].lon = rand()/DIV;
        data[0].alt = rand() % 1000;

        for(i=1; i<D_SIZE; i++)
        {
                sign = rand() % 2;
                if(sign)
                        data[i].lat = data[i-1].lat + rand()/(DIV);
                else
                        data[i].lat = data[i-1].lat - rand()/(DIV);
                sign = rand() % 2;
                if(sign)
                        data[i].lon = data[i-1].lon + rand()/(DIV);
                else
                        data[i].lon = data[i-1].lon - rand()/(DIV);
                data[i].alt = rand() % 1000;
                //DBG("data: %8d, %8d, %8d\n",data[i].lat, data[i].lon, data[i].alt);
        }

        // Draw plot
        p.d = &data[0];
        p.d_size = D_SIZE;
        p.x_size = 127;
        p.y_size = 100;
        p.title = title;
        p.frame = 1;
        p.line = 1;
        draw_path(&w->dc, &p);

}

struct ui_widget path_widget = {
	path_redraw,
	path_event,
	{ 0, 16, 127, 127 },
	0,
	WF_ACTIVE | WF_VISIBLE
};

