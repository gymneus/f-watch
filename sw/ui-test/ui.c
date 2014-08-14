#include "gfx.h"
#include "ui.h"

struct surface screen;

extern void lcd_update(struct surface *surf);

#define MAX_QUEUE_EVENTS 16

struct {
	int head, tail, count;
	struct ui_event q[MAX_QUEUE_EVENTS];
} event_queue;

static struct ui_widget *widget_list = NULL;

void ui_init()
{
	gfx_init_surface( &screen, NULL, 0, 0, 128, 128 );

	event_queue.head = 0;
	event_queue.tail = 0;
	event_queue.count = 0;

	digtal_watch_create();
	pls_viewer_create(); // move outsidem
}

void ui_post_event( int type, int data )
{
	struct ui_event evt;
	evt.type = type;
	evt.data = data;

	//DBG("Post %x/%x\n", type, data);
	if(event_queue.count == MAX_QUEUE_EVENTS)
	{
		DBG("Dropping event %x/%x (queue full)\n", type, data);
		return;
	}

	event_queue.q[event_queue.head] = evt;
	event_queue.head = (event_queue.head + 1) % MAX_QUEUE_EVENTS;
	event_queue.count++;
}

int ui_poll_events( struct ui_event *evt )
{
	if(event_queue.count)
	{
		*evt = event_queue.q[event_queue.tail];
		return 1;
	} 
	return 0;
}

int ui_discard_event()
{
	//DBG("discard\n");
	event_queue.count--;
	event_queue.tail = (event_queue.tail + 1) % MAX_QUEUE_EVENTS;
}

void ui_add_widget(struct ui_widget *w)
{
	w->next = NULL;

	if(!widget_list)
		widget_list = w;
	else {
		struct ui_widget *wl;

		for(wl = widget_list; wl->next; wl = wl->next);
		wl->next = w;
		
	}


	if(! (w->flags & WF_DOUBLE_BUFFER))
		gfx_init_surface(&w->dc, &screen, w->pos.x0, w->pos.y0, w->pos.x1 - w->pos.x0 + 1, w->pos.y1 - w->pos.y0 + 1);
	else {
		DBG ("NonImpl\n");
	}

	w->flags |= WF_DIRTY;

}

void ui_update()
{
	struct ui_event evt;
	int screen_dirty = 0;

	if(ui_poll_events(&evt))
	{
		ui_discard_event();
		struct ui_widget *w;
		for(w = widget_list; w; w = w->next)
		{
			//DBG("w %p flags %x\n", w, w->flags);
			if((w->flags & WF_ACTIVE) && (w->event))
			{
				w->event(w, evt);
			}

			if((w->flags & WF_VISIBLE) && (w->flags & WF_DIRTY))
			{
				w->redraw(w);
				w->flags &= ~WF_DIRTY;
				screen_dirty = 1;
			}
		}
	}

	if(screen_dirty)
		lcd_update( &screen );
}