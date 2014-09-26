#include "path_widget.h"


void find_path_min_max(struct coord *d, uint16_t d_size, struct coord *min, struct coord *max)
{
        uint16_t i;
        uint8_t j;

        min->lat = d[0].lat;
        max->lat = d[0].lat;

        min->lon = d[0].lon;
        max->lon = d[0].lon;

        min->alt = d[0].alt;
        max->alt = d_size;

        for(i=0; i<d_size; i++){
                if(d[i].lat < min->lat)
                        min->lat = d[i].lat;
                if(d[i].lat > max->lat)
                        max->lat = d[i].lat;
        }

        DBG("find_min_max: d_size=%d\n",d_size);

}


void draw_path(struct surface *surf, path *p)
{
        uint8_t i;
        struct coord min;
        struct coord max;
        struct coord *p_min;
        struct coord *p_max;
        uint8_t x_scale, y_scale, x_win;
        int8_t y_pos, y_pos_next;
        char buf[20];

        p_min = &min;
        p_max = &max;

        for(i=0; i<p->d_size; i++)
        {
                DBG("data: %8d, %8d, %8d\n",p->d[i].lat, p->d[i].lon, p->d[i].alt);
        }

        DBG("draw_path: d_size=%d, i=%d\n",p->d_size, i);


        // Find data min/max
        find_min_max(p->d, p->d_size, p_min, p_max);
        DBG("lat: min: %8d, max: %8d, max-min: %8d\n", min.lat, max.lat, (max.lat - min.lat));
        DBG("lon: min: %8d, max: %8d, max-min: %8d\n", min.lon, max.lon, (max.lon - min.lon));
        DBG("alt: min: %8d, max: %8d, max-min: %8d\n", min.alt, max.alt, (max.alt - min.alt));

        /*
        // Compute x scaling factors
        // If data size is smaller than the plot size, don't stretch data
        if(p->d_size < p->x_size){
                x_scale = 1;
                x_win = p->d_size;
        }
        else{
                x_scale = p->d_size / p->x_size;
                x_win = p->x_size;
        }

        // Compute y scaling factor (rounded up by adding 1)
        y_scale = (y_max - y_min) / p->y_size + 1;

        DBG("x_scale: %d\ny_scale: %d\n",x_scale, y_scale);

        // Rescale and plot data
        for(i=0; i<x_win-1; i++){
                //y_pos = (d[i*x_scale])/y_scale  + y_size/2;
                //y_pos_next = (d[(i+1)*x_scale])/y_scale + y_size/2;
                y_pos = (p->d[i*x_scale] - y_min)/y_scale;
                y_pos_next = (p->d[(i+1)*x_scale] - y_min)/y_scale;
                //DBG("x=%3d, y=%3d (%d)\n", i, y_pos, d[i*x_scale]);
                if(p->line)
                        gfx_line(surf, i, p->y_size-y_pos, i+1, p->y_size-y_pos_next, 1);
                else
                        gfx_set_pixel(surf, i, p->y_size-y_pos, 1);
        }

        // Print plot title
        DBG("title: %s",p->title);
        //gfx_text(surf, &font_xm5x8, 0, 0, p->title);
        //gfx_centered_text(surf, &font_helv11, 0, p->title);
         gfx_centered_text(surf, &font_xm4x6, p->y_size+1, p->title);

        // Print min max
        sprintf(buf,"min:%d",y_min);
        gfx_text(surf, &font_xm4x6, 1, p->y_size+1, buf);
        sprintf(buf,"max:%d",y_max);
        gfx_text(surf, &font_xm4x6, p->x_size-gfx_text_width(&font_xm4x6,buf), p->y_size+1, buf);

        // Print frame, if enabled
        if(p->frame)
        {
                gfx_line(surf, 0, 0, p->x_size, 0, 1);
                gfx_line(surf, p->x_size, 0, p->x_size, p->y_size, 1);
                gfx_line(surf, p->x_size, p->y_size, 0, p->y_size, 1);
                gfx_line(surf, 0, p->y_size, 0, 0, 1);
        }
        */
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
        float x;
        static float a=0;
        path p;

        // Clear widget
        gfx_clear(&w->dc, 0);

        // Generate test data
        sprintf(title, "test path\n");

        x = 0;
        for(i=0; i<D_SIZE; i++)
        {
                x+=100;
                data[i].lat = x*i;
                data[i].lon = 100+(i*x-50);
                data[i].alt = 10+i;
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

