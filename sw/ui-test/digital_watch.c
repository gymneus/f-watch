#include "ui.h"
#include "plot_widget.h"
//#include "../bitmaps/bitmaps.h"


extern int64_t sys_get_tics();

void sys_get_time(int *h, int *m, int *cycs)
{
	int64_t t = sys_get_tics() / 1200;
	
	if(cycs)
		*cycs = t % 50;
	t /= 50;
	if(m)
		*m = t % 60;
	t /= 60;
	if(h)
		*h = t % 24;
}

static void digital_watch_redraw(struct ui_widget *w)
{
	int h,m,c;

	sys_get_time(&h, &m, &c);

	char buf[20];
	sprintf(buf,"%02d:%02d", h, m);

	gfx_clear(&w->dc, 0);
	gfx_text(&w->dc, &font_helv38b, 0, 0, buf);

	sprintf(buf,"%02d.%01d", c * 12 / 10, (c * 12) % 10);
	gfx_text(&w->dc, &font_helv22b, 84, 14, buf);


}

static void digital_watch_event(struct ui_widget *w, struct ui_event event)
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

struct ui_widget digital_watch = {
	digital_watch_redraw,
	digital_watch_event,
	{ 0, 20, 127, 59 },
	0,
	WF_ACTIVE | WF_VISIBLE
};


struct pls_cycle {
	uint8_t current;
	uint8_t ramp;
	uint8_t flat_top;
	uint8_t ejection;
	char *name;
};

static  struct pls_cycle cycles[] = {
	{ 20, 8, 5, 3, "TOF" },
	{ 22, 4, 14, 3, "EAST1" },
	{ 10, 2, 2, 5, "SFTPRO" },
	{ 6, 2, 20, 5, "LHCINDIV" },
	{ 30, 8, 5, 3, "MD6" },
	{ 0, 4, 14, 3, "ZERO" },
	{ 0, 2, 2, 5, "ZERO" },
	{ 15, 5, 2, 5, "ISOGPS" }
};

int pls_current = 0, pls_total = 8;

static void pls_redraw(struct ui_widget *w)
{
	gfx_clear(&w->dc, 0);

#define PERIOD 30

// crap!
	int n = -((sys_get_tics() / UI_TICK_RATE )) % PERIOD;
	int x = n, y = 0, yn, xn, x0;
	int i;

	if(n == PERIOD - 1)
		pls_current++;
	if(pls_current == pls_total)
		pls_current = 0;
	
	i = pls_current;
	gfx_set_clip(&w->dc, 70, 0, 127, 30);
	while(1)
	{
		yn = y;
		x0 = x;
		xn = x;
		//printf("x %d y %d xn %d yn %d\n", x, y, xn, yn);
		gfx_line(&w->dc, 30+x, 30-y, 30+xn, 30-yn, 1);x=xn;y=yn;
		gfx_line(&w->dc, 30+x, 30-y, 30+xn, 30-yn, 1);x=xn;y=yn;
		xn = x+cycles[i].ramp;
		yn = y+cycles[i].current;
		gfx_line(&w->dc, 30+x, 30-y, 30+xn, 30-yn, 1);x=xn;y=yn;
		xn = x+cycles[i].flat_top;
		gfx_line(&w->dc, 30+x, 30-y, 30+xn, 30-yn, 1);x=xn;y=yn;
		xn = x+cycles[i].ejection;
		yn  = 0;
		gfx_line(&w->dc, 30+x, 30-y, 30+xn, 30-yn, 1);x=xn;y=yn;
		xn= x0 + 30;
		gfx_line(&w->dc, 30+x, 30-y, 30+xn, 30-yn, 1);x=xn;y=yn;

		i++;
		if(i == pls_total)
			i = 0;
		if(i == pls_current)
			break;
	}
	gfx_reset_clip(&w->dc);

	gfx_line(&w->dc, 68, 0, 68, 30, 1);

	i = pls_current - 3;
	if (i < 0)
		i += pls_total;


	for(n=0;n<4;n++)
	{
		struct pls_cycle *cyc = &cycles[i];
		char buf[10];
		sprintf(buf,"%02d %s", i + 1, cyc->name);
		gfx_text(&w->dc, &font_xm5x8, 0, n * 8, buf);
		if(n == 3)
			gfx_box(&w->dc, 0, n*8-1, 60, n*8+7, COLOR_TOGGLE);
		i++; i %= pls_total;



	}


}

static void pls_event(struct ui_widget *w, struct ui_event event)
{
	switch(event.type)
	{
		case EVT_TICK:
		{
			if(event.data % 3 == 0) // 15 refreshes/second
				w->flags |= WF_DIRTY;
			break;
		}
		default:
			break;
	}
}




struct ui_widget pls_viewer = {
	pls_redraw,
	pls_event,
	{ 0, 60, 127, 120 },
	0,
	WF_ACTIVE | WF_VISIBLE
};


struct ui_widget home_screen = {
	NULL,
	NULL,
	{ 0, 0, 127, 127 },
	0,
	WF_ACTIVE | WF_VISIBLE
};

static void status_bar_event(struct ui_widget *w, struct ui_event event)
{
        /*
        static int8_t level;

	switch(event.type)
        {
        case EVT_UP:
        {
                level += 10;
                if(level > 100)
                        level = 100;
                printf("UP  : level %3d\n",level);
                gfx_box(&w->dc, 127-15, 1, 127-3, 11, COLOR_WHITE);
                gfx_box(&w->dc, 127-15, 1, 127-15+((level * 12)/100), 11, COLOR_BLACK);
                gfx_box(&w->dc, 0, 0, 15, 15, COLOR_WHITE);
                //gfx_draw_bitmap(&w->dc, 0, 0, &gps_disconnected);
                w->flags |= WF_DIRTY;
                break;
        }
        case EVT_DOWN:
        {
                level -= 10;
                if(level < 0)
                        level = 0;
                printf("DOWN: level %3d\n",level);
                gfx_box(&w->dc, 127-15, 1, 127-3, 11, COLOR_WHITE);
                gfx_box(&w->dc, 127-15, 1, 127-15+((level * 12)/100), 11, COLOR_BLACK);
                gfx_box(&w->dc, 0, 0, 15, 15, COLOR_WHITE);
                //gfx_draw_bitmap(&w->dc, 0, 0, &gps_receiving);
                w->flags |= WF_DIRTY;
                break;
        }
        case EVT_RIGHT:
        {
                gfx_box(&w->dc, 127-15, 1, 127-3, 11, COLOR_WHITE);
                gfx_draw_bitmap(&w->dc, 127-15, 0, &battery_charging);
                gfx_box(&w->dc, 0, 0, 15, 15, COLOR_WHITE);
                //gfx_draw_bitmap(&w->dc, 0, 0, &gps_searching);
                w->flags |= WF_DIRTY;
                break;
        }
        default:
                break;
	}
        */
}

static void status_bar_redraw(struct ui_widget *w)
{
	//gfx_line(&w->dc, 0, w->pos.y1, 127, w->pos.y1, COLOR_BLACK);
	gfx_round_box(&w->dc, 30, -10, 127-30, 10, 9, COLOR_BLACK);
	gfx_centered_text(&w->dc, &font_helv11, 0, "Home");
        //gfx_draw_bitmap(&w->dc, 127-15, 0, &battery);
}

struct ui_widget status_bar = {
	status_bar_redraw,
	status_bar_event,
	{ 0, 0, 127, 15 },
	0,
	WF_ACTIVE | WF_VISIBLE
};


void home_screen_create()
{
	ui_init_widget (&home_screen);
	ui_init_widget (&digital_watch);
        //ui_init_widget (&pls_viewer);
        ui_init_widget (&hist_plot);

	ui_add_widget(&digital_watch);
	//ui_add_widget(&pls_viewer);
	ui_add_widget (&hist_plot);

	ui_add_child(&home_screen, &digital_watch);
	//ui_add_child(&home_screen, &pls_viewer);
        ui_add_child (&home_screen, &hist_plot);
	ui_add_widget(&home_screen);

	ui_init_widget (&status_bar);
	ui_add_widget (&status_bar);
}
