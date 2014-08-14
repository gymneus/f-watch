#include "ui.h"

static int dw_h, dw_min, dw_cycles;

static void digital_watch_redraw(struct ui_widget *w)
{
	char buf[20];
	sprintf(buf,"%02d:%02d", dw_h, dw_min);

	gfx_clear(&w->dc, 0);
	gfx_text(&w->dc, &font_helv38b, 0, 0, buf);

	sprintf(buf,"%02d.%01d", dw_cycles * 12 / 10, (dw_cycles * 12) % 10);
	gfx_text(&w->dc, &font_helv22b, 84, 14, buf);


}

static void digital_watch_event(struct ui_widget *w, struct ui_event event)
{
	switch(event.type)
	{
		case EVT_NEXT_CYCLE:
		{
			dw_cycles = event.data % 50;
			event.data /= 50;
			dw_min = event.data % 60;
			event.data /= 60;
			dw_h = event.data % 24;

			w->flags |= WF_DIRTY;
			break;
		}
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

void digtal_watch_create()
{
	ui_add_widget ( &digital_watch );
}

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

static int pls_current = 0, pls_total = 8;

static void pls_redraw(struct ui_widget *w)
{
	gfx_clear(&w->dc, 0);

//	fprintf(stderr, "PLS_redraw!");

	/*gfx_line(&w->dc, 64, 0, 127, 0, 1);
	gfx_line(&w->dc, 64, 30, 127, 30, 1);
	gfx_line(&w->dc, 64, 0, 64, 30, 1);
	gfx_line(&w->dc, 127, 0, 127, 30, 1);*/

#define PERIOD 30

	int n = -((sys_get_tics() / UI_TICK_RATE ) / 2) % PERIOD;
	int x = n, y = 0, yn, xn, x0;
	int i;

	//printf("x0 %d\n", x);

	printf("pls_current %d\n", pls_current);

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
		sprintf(buf,"%-02d %s", i + 1, cyc->name);
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

void pls_viewer_create()
{
	ui_add_widget ( &pls_viewer );
}


