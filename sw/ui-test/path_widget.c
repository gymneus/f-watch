#include "path_widget.h"


void find_path_min_max(int32_t* d, uint16_t d_size, int32_t* min, int32_t* max)
{
        uint16_t i;
        uint8_t j;

        for(j=0; j<2; j++){
                min[j] = d[0][j];
                max[j] = d[0][j];
        }

        for(i=0; i<d_size; i++){
                for(j=0; j<2; j++){
                        if(d[i][j] < min[j])
                                min[j] = d[i][j];
                        if(d[i][j] > max[j])
                                max[j] = d[i][j];
                }
        }
}


void draw_path(struct surface* surf, path* p)
{
        uint8_t i;
        int32_t min[2];
        int32_t max[2];
        uint8_t x_scale, y_scale, x_win;
        int8_t y_pos, y_pos_next;
        char buf[20];


        for(i=0; i<p->d_size; i++)
        {
                DBG("data: %8d, %8d\n",p->d[i][0], p->d[i][1]);
        }

        // Find data min/max
        find_min_max(p->d, p->d_size, &min[0], &max[0]);
        for(i=0; i<2; i++){
                DBG("%d: min: %d\nmax: %d\nmax-min: %d\n",i , min[i], max[i],(max[i] - min[i]));
        }

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
        int32_t data[D_SIZE][2];
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
                data[i][0] = x*i;
                data[i][1] = 100+(i*x-50);
                //DBG("data: %8d, %8d\n",data[i][0], data[i][1]);
        }
        a+=0.8;

        // Draw plot
        p.d = &data[0][0];
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

